/*
 * Copyright (C) 2023 Igalia S.L
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * aint with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "VideoDecoderGStreamer.h"

#if ENABLE(WEB_CODECS) && USE(GSTREAMER)

#include "GStreamerCodecUtilities.h"
#include "GStreamerCommon.h"
#include "GStreamerElementHarness.h"
#include "GStreamerRegistryScanner.h"
#include "HEVCUtilities.h"
#include "VP9Utilities.h"
#include "VideoFrameGStreamer.h"
#include <wtf/WorkQueue.h>

namespace WebCore {

GST_DEBUG_CATEGORY(webkit_video_decoder_debug);
#define GST_CAT_DEFAULT webkit_video_decoder_debug

static WorkQueue& gstDecoderWorkQueue()
{
    static NeverDestroyed<Ref<WorkQueue>> queue(WorkQueue::create("GStreamer VideoDecoder Queue"));
    return queue.get();
}

class GStreamerInternalVideoDecoder : public ThreadSafeRefCounted<GStreamerInternalVideoDecoder>
    , public CanMakeWeakPtr<GStreamerInternalVideoDecoder, WeakPtrFactoryInitialization::Eager> {
    WTF_MAKE_FAST_ALLOCATED;

public:
    static Ref<GStreamerInternalVideoDecoder> create(const String& codecName, const VideoDecoder::Config& config, VideoDecoder::OutputCallback&& outputCallback, VideoDecoder::PostTaskCallback&& postTaskCallback, GRefPtr<GstElement>&& element)
    {
        return adoptRef(*new GStreamerInternalVideoDecoder(codecName, config, WTFMove(outputCallback), WTFMove(postTaskCallback), WTFMove(element)));
    }
    ~GStreamerInternalVideoDecoder()
    {
        if (isConfigured()) {
            GST_DEBUG_OBJECT(harnessedElement(), "Disposing video decoder");
            return;
        }
        GST_DEBUG("Disposing un-configured video decoder");
    }

    void postTask(Function<void()>&& task) { m_postTaskCallback(WTFMove(task)); }
    void decode(std::span<const uint8_t>, bool isKeyFrame, int64_t timestamp, std::optional<uint64_t> duration,  VideoDecoder::DecodeCallback&&);
    void flush(Function<void()>&&);
    void close() { m_isClosed = true; }

    bool isConfigured() const { return !!m_inputCaps; }

    GstElement* harnessedElement() const { return m_harness->element(); }

private:
    GStreamerInternalVideoDecoder(const String& codecName, const VideoDecoder::Config&, VideoDecoder::OutputCallback&&, VideoDecoder::PostTaskCallback&&, GRefPtr<GstElement>&&);

    VideoDecoder::OutputCallback m_outputCallback;
    VideoDecoder::PostTaskCallback m_postTaskCallback;

    RefPtr<GStreamerElementHarness> m_harness;
    FloatSize m_presentationSize;
    bool m_isClosed { false };
    GRefPtr<GstCaps> m_inputCaps;
};

static bool validateCodecString(const StringView& codecString)
{
    if (codecString.startsWith("avc1"_s)) {
        auto parameters = parseAVCCodecParameters(codecString);
        // Limit to High Profile, level 5.2.
        return parameters && parameters->profileIDC <= 100 && parameters->levelIDC <= 52;
    }
    if (codecString.startsWith("vp09"_s)) {
        auto parameters = parseVPCodecParameters(codecString);
        return parameters.has_value();
    }
    if (codecString.startsWith("hvc1"_s) || codecString.startsWith("hev1"_s))
        return GStreamerCodecUtilities::parseHEVCProfile(codecString.toString());
    if (codecString.startsWith("av01."_s)) {
        auto components = codecString.split('.');
        auto nextElement = components.begin();
        ++nextElement;

        auto profile = *nextElement;
        return profile == "0"_s || profile == "1"_s || profile == "2"_s;
    }

    return true;
}

bool GStreamerVideoDecoder::create(const String& codecName, const Config& config, CreateCallback&& callback, OutputCallback&& outputCallback, PostTaskCallback&& postTaskCallback)
{
    static std::once_flag debugRegisteredFlag;
    std::call_once(debugRegisteredFlag, [] {
        GST_DEBUG_CATEGORY_INIT(webkit_video_decoder_debug, "webkitvideodecoder", 0, "WebKit WebCodecs Video Decoder");
    });

    if (!validateCodecString(codecName))
        return false;

    auto& scanner = GStreamerRegistryScanner::singleton();
    auto lookupResult = scanner.isCodecSupported(GStreamerRegistryScanner::Configuration::Decoding, codecName);
    if (!lookupResult) {
        GST_WARNING("No decoder found for codec %s", codecName.ascii().data());
        return false;
    }

    GRefPtr<GstElement> element = gst_element_factory_create(lookupResult.factory.get(), nullptr);
    auto decoder = makeUniqueRef<GStreamerVideoDecoder>(codecName, config, WTFMove(outputCallback), WTFMove(postTaskCallback), WTFMove(element));
    if (!decoder->m_internalDecoder->isConfigured()) {
        GST_WARNING("Internal video decoder failed to configure for codec %s", codecName.ascii().data());
        return false;
    }

    gstDecoderWorkQueue().dispatch([callback = WTFMove(callback), decoder = WTFMove(decoder)]() mutable {
        auto internalDecoder = decoder->m_internalDecoder;
        internalDecoder->postTask([callback = WTFMove(callback), decoder = WTFMove(decoder)]() mutable {
            GST_DEBUG_OBJECT(decoder->m_internalDecoder->harnessedElement(), "Video decoder created");
            callback(UniqueRef<VideoDecoder> { WTFMove(decoder) });
        });
    });

    return true;
}

GStreamerVideoDecoder::GStreamerVideoDecoder(const String& codecName, const Config& config, OutputCallback&& outputCallback, PostTaskCallback&& postTaskCallback, GRefPtr<GstElement>&& element)
    : m_internalDecoder(GStreamerInternalVideoDecoder::create(codecName, config, WTFMove(outputCallback), WTFMove(postTaskCallback), WTFMove(element)))
{
}

GStreamerVideoDecoder::~GStreamerVideoDecoder()
{
    close();
}

void GStreamerVideoDecoder::decode(EncodedFrame&& frame, DecodeCallback&& callback)
{
    gstDecoderWorkQueue().dispatch([value = Vector<uint8_t> { frame.data }, isKeyFrame = frame.isKeyFrame, timestamp = frame.timestamp, duration = frame.duration, decoder = m_internalDecoder, callback = WTFMove(callback)]() mutable {
        decoder->decode({ value.data(), value.size() }, isKeyFrame, timestamp, duration, WTFMove(callback));
    });
}

void GStreamerVideoDecoder::flush(Function<void()>&& callback)
{
    gstDecoderWorkQueue().dispatch([decoder = m_internalDecoder, callback = WTFMove(callback)]() mutable {
        decoder->flush(WTFMove(callback));
    });
}

void GStreamerVideoDecoder::reset()
{
    m_internalDecoder->close();
}

void GStreamerVideoDecoder::close()
{
    m_internalDecoder->close();
}

GStreamerInternalVideoDecoder::GStreamerInternalVideoDecoder(const String& codecName, const VideoDecoder::Config& config, VideoDecoder::OutputCallback&& outputCallback, VideoDecoder::PostTaskCallback&& postTaskCallback, GRefPtr<GstElement>&& element)
    : m_outputCallback(WTFMove(outputCallback))
    , m_postTaskCallback(WTFMove(postTaskCallback))
{
    configureVideoDecoderForHarnessing(element);

    GST_DEBUG_OBJECT(element.get(), "Configuring decoder for codec %s", codecName.ascii().data());
    const char* parser = nullptr;
    if (codecName.startsWith("avc1"_s)) {
        m_inputCaps = adoptGRef(gst_caps_new_simple("video/x-h264", "stream-format", G_TYPE_STRING, "avc", "alignment", G_TYPE_STRING, "au", nullptr));
        parser = "h264parse";
        if (auto codecData = wrapSpanData(config.description))
            gst_caps_set_simple(m_inputCaps.get(), "codec_data", GST_TYPE_BUFFER, codecData.get(), nullptr);
    } else if (codecName.startsWith("av01"_s)) {
        m_inputCaps = adoptGRef(gst_caps_new_simple("video/x-av1", "stream-format", G_TYPE_STRING, "obu-stream", "alignment", G_TYPE_STRING, "frame", nullptr));
        parser = "av1parse";
    } else if (codecName.startsWith("vp8"_s))
        m_inputCaps = adoptGRef(gst_caps_new_empty_simple("video/x-vp8"));
    else if (codecName.startsWith("vp09"_s)) {
        m_inputCaps = adoptGRef(gst_caps_new_empty_simple("video/x-vp9"));
        parser = "vp9parse";
    } else if (codecName.startsWith("hvc1"_s)) {
        m_inputCaps = adoptGRef(gst_caps_new_simple("video/x-h265", "stream-format", G_TYPE_STRING, "hvc1", "alignment", G_TYPE_STRING, "au", nullptr));
        parser = "h265parse";
        if (auto codecData = wrapSpanData(config.description))
            gst_caps_set_simple(m_inputCaps.get(), "codec_data", GST_TYPE_BUFFER, codecData.get(), nullptr);
    } else if (codecName.startsWith("hev1"_s)) {
        m_inputCaps = adoptGRef(gst_caps_new_simple("video/x-h265", "stream-format", G_TYPE_STRING, "hev1", "alignment", G_TYPE_STRING, "au", nullptr));
        parser = "h265parse";
        if (auto codecData = wrapSpanData(config.description))
            gst_caps_set_simple(m_inputCaps.get(), "codec_data", GST_TYPE_BUFFER, codecData.get(), nullptr);
    } else {
        WTFLogAlways("Codec %s not wired in yet", codecName.ascii().data());
        return;
    }

    if (config.width && config.height)
        gst_caps_set_simple(m_inputCaps.get(), "width", G_TYPE_INT, config.width, "height", G_TYPE_INT, config.height, nullptr);

    GRefPtr<GstElement> harnessedElement;
    auto* factory = gst_element_get_factory(element.get());
    if (parser && !gst_element_factory_can_sink_all_caps(factory, m_inputCaps.get())) {
        // The decoder won't accept the input caps, so put a parser in front.
        auto* parserElement = makeGStreamerElement(parser, nullptr);
        if (!parserElement) {
            GST_WARNING_OBJECT(element.get(), "Required parser %s not found, decoding will fail", parser);
            m_inputCaps.clear();
            return;
        }
        harnessedElement = gst_bin_new(nullptr);
        gst_bin_add_many(GST_BIN_CAST(harnessedElement.get()), parserElement, element.get(), nullptr);
        gst_element_link(parserElement, element.get());
        auto sinkPad = adoptGRef(gst_element_get_static_pad(parserElement, "sink"));
        gst_element_add_pad(harnessedElement.get(), gst_ghost_pad_new("sink", sinkPad.get()));
        auto srcPad = adoptGRef(gst_element_get_static_pad(element.get(), "src"));
        gst_element_add_pad(harnessedElement.get(), gst_ghost_pad_new("src", srcPad.get()));
    } else
        harnessedElement = WTFMove(element);

    m_harness = GStreamerElementHarness::create(WTFMove(harnessedElement), [weakThis = WeakPtr { *this }, this](auto& stream, const GRefPtr<GstBuffer>& outputBuffer) {
        if (!weakThis)
            return;

        if (m_isClosed)
            return;

        GST_TRACE_OBJECT(m_harness->element(), "Got frame with PTS: %" GST_TIME_FORMAT, GST_TIME_ARGS(GST_BUFFER_PTS(outputBuffer.get())));

        if (m_presentationSize.isEmpty())
            m_presentationSize = getVideoResolutionFromCaps(stream.outputCaps().get()).value_or(FloatSize { 0, 0 });

        m_postTaskCallback([weakThis = WeakPtr { *this }, this, outputBuffer, outputCaps = stream.outputCaps()]() mutable {
            if (!weakThis)
                return;

            if (m_isClosed)
                return;

            auto sample = adoptGRef(gst_sample_new(outputBuffer.get(), outputCaps.get(), nullptr, nullptr));
            auto videoFrame = VideoFrameGStreamer::create(WTFMove(sample), m_presentationSize, fromGstClockTime(GST_BUFFER_PTS(outputBuffer.get())));

            m_outputCallback(VideoDecoder::DecodedFrame { WTFMove(videoFrame), static_cast<int64_t>(GST_BUFFER_PTS(outputBuffer.get())), GST_BUFFER_DURATION(outputBuffer.get()) });
        });
    });
}

void GStreamerInternalVideoDecoder::decode(std::span<const uint8_t> frameData, bool isKeyFrame, int64_t timestamp, std::optional<uint64_t> duration, VideoDecoder::DecodeCallback&& callback)
{
    GST_DEBUG_OBJECT(m_harness->element(), "Decoding%s frame", isKeyFrame ? " key" : "");
    auto buffer = wrapSpanData(frameData);
    if (!buffer) {
        m_postTaskCallback([weakThis = WeakPtr { *this }, this, callback = WTFMove(callback)]() mutable {
            if (!weakThis)
                return;

            if (m_isClosed)
                return;

            m_outputCallback(makeUnexpected("Empty frame"_s));
            callback({ });
        });
        return;
    }

    GST_BUFFER_DTS(buffer.get()) = GST_BUFFER_PTS(buffer.get()) = timestamp;
    if (duration)
        GST_BUFFER_DURATION(buffer.get()) = *duration;

    // FIXME: Maybe configure segment here, could be useful for reverse playback.
    auto result = m_harness->pushSample(adoptGRef(gst_sample_new(buffer.get(), m_inputCaps.get(), nullptr, nullptr)));
    m_postTaskCallback([weakThis = WeakPtr { *this }, this, callback = WTFMove(callback), result]() mutable {
        if (!weakThis)
            return;

        if (weakThis->m_isClosed)
            return;

        if (result)
            m_harness->processOutputBuffers();
        else
            m_outputCallback(makeUnexpected("Decode error"_s));

        callback({ });
    });
}

void GStreamerInternalVideoDecoder::flush(Function<void()>&& callback)
{
    if (m_isClosed) {
        GST_DEBUG_OBJECT(m_harness->element(), "Decoder closed, nothing to flush");
        m_postTaskCallback(WTFMove(callback));
        return;
    }

    m_harness->flushBuffers();
    m_postTaskCallback(WTFMove(callback));
}

#undef GST_CAT_DEFAULT

} // namespace WebCore

#endif // ENABLE(WEB_CODECS) && USE(GSTREAMER)
