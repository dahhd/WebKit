/*
 * Copyright (C) 2021-2022 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#import "config.h"
#import "CGDisplayListImageBufferBackend.h"

#if ENABLE(CG_DISPLAY_LIST_BACKED_IMAGE_BUFFER)

#import "CGDisplayList.h"
#import "Logging.h"
#import <WebCore/GraphicsContextCG.h>
#import <WebCore/PixelBuffer.h>
#import <WebKitAdditions/CGDisplayListImageBufferAdditions.h>
#import <pal/spi/cocoa/QuartzCoreSPI.h>
#import <wtf/IsoMallocInlines.h>
#import <wtf/MachSendRight.h>
#import <wtf/cocoa/TypeCastsCocoa.h>
#import <wtf/cocoa/VectorCocoa.h>

template<> struct WTF::CFTypeTrait<CAMachPortRef> {
    static inline CFTypeID typeID(void) { return CAMachPortGetTypeID(); }
};

namespace WebKit {

static CFDictionaryRef makeContextOptions(const CGDisplayListImageBufferBackend::Parameters& parameters)
{
    RetainPtr colorSpace = parameters.colorSpace.platformColorSpace();
    if (!colorSpace)
        return nil;
    return (CFDictionaryRef)@{
        @"colorspace" : (id)colorSpace.get()
    };
}

class GraphicsContextCGDisplayList : public WebCore::GraphicsContextCG {
public:
    GraphicsContextCGDisplayList(const CGDisplayListImageBufferBackend::Parameters& parameters, WebCore::IntSize backendSize, WebCore::RenderingMode renderingMode)
        : GraphicsContextCG(adoptCF(WKCGCommandsContextCreate(backendSize, makeContextOptions(parameters))).autorelease(), GraphicsContextCG::Unknown, renderingMode)
    {
    }

    bool canUseShadowBlur() const final { return false; }

    bool needsCachedNativeImageInvalidationWorkaround(WebCore::RenderingMode) override { return true; }
};

WTF_MAKE_ISO_ALLOCATED_IMPL(CGDisplayListImageBufferBackend);

size_t CGDisplayListImageBufferBackend::calculateMemoryCost(const Parameters& parameters)
{
    // FIXME: This is fairly meaningless, because we don't actually have a bitmap, and
    // should really be based on the encoded data size.
    auto backendSize = calculateBackendSize(parameters);
    return WebCore::ImageBufferBackend::calculateMemoryCost(backendSize, calculateBytesPerRow(backendSize));
}

std::unique_ptr<CGDisplayListImageBufferBackend> CGDisplayListImageBufferBackend::create(const Parameters& parameters, const WebCore::ImageBufferCreationContext& creationContext)
{
    if (parameters.logicalSize.isEmpty())
        return nullptr;

    return std::unique_ptr<CGDisplayListImageBufferBackend>(new CGDisplayListImageBufferBackend(parameters, creationContext, WebCore::RenderingMode::Unaccelerated));
}

CGDisplayListImageBufferBackend::CGDisplayListImageBufferBackend(const Parameters& parameters, const WebCore::ImageBufferCreationContext& creationContext, WebCore::RenderingMode renderingMode)
    : ImageBufferCGBackend { parameters }
    , m_renderingMode(renderingMode)
{
    if (creationContext.useCGDisplayListImageCache == UseCGDisplayListImageCache::Yes)
        m_resourceCache = bridge_id_cast(adoptCF(WKCGCommandsCacheCreate(nullptr)));
}

ImageBufferBackendHandle CGDisplayListImageBufferBackend::createBackendHandle(SharedMemory::Protection) const
{
    ASSERT(m_context);

    RetainPtr<NSDictionary> options;
    RetainPtr<NSMutableArray> ports;
    if (m_resourceCache) {
        ports = adoptNS([[NSMutableArray alloc] init]);
        options = @{
            @"ports": ports.get(),
            @"cache": m_resourceCache.get()
        };
    }

    auto data = adoptCF(WKCGCommandsContextCopyEncodedDataWithOptions(m_context->platformContext(), bridge_cast(options.get())));
    ASSERT(data);

#if !RELEASE_LOG_DISABLED
    auto size = backendSize();
    RELEASE_LOG(RemoteLayerTree, "CGDisplayListImageBufferBackend of size %dx%d encoded display list of %ld bytes with %ld surfaces (useImageCache: %s)", size.width(), size.height(), CFDataGetLength(data.get()), [ports count], m_resourceCache ? "yes" : "no");
#endif

    Vector<MachSendRight> sendRights;
    if (m_resourceCache) {
        sendRights = makeVector(ports.get(), [] (id port) -> std::optional<MachSendRight> {
            // We `create` instead of `adopt` because CAMachPort has no API to leak its reference.
            return { MachSendRight::create(CAMachPortGetPort(checked_cf_cast<CAMachPortRef>(port))) };
        });
    }

    return CGDisplayList { WebCore::SharedBuffer::create(data.get()), WTFMove(sendRights) };
}

WebCore::GraphicsContext& CGDisplayListImageBufferBackend::context()
{
    if (!m_context) {
        m_context = makeUnique<GraphicsContextCGDisplayList>(m_parameters, backendSize(), m_renderingMode);
        applyBaseTransform(*m_context);
    }
    return *m_context;
}

WebCore::IntSize CGDisplayListImageBufferBackend::backendSize() const
{
    return calculateBackendSize(m_parameters);
}

unsigned CGDisplayListImageBufferBackend::bytesPerRow() const
{
    return calculateBytesPerRow(backendSize());
}

void CGDisplayListImageBufferBackend::releaseGraphicsContext()
{
    m_context = nullptr;
}

RefPtr<WebCore::NativeImage> CGDisplayListImageBufferBackend::copyNativeImage(WebCore::BackingStoreCopy)
{
    ASSERT_NOT_REACHED();
    return nullptr;
}

void CGDisplayListImageBufferBackend::getPixelBuffer(const WebCore::IntRect&, WebCore::PixelBuffer&)
{
    ASSERT_NOT_REACHED();
}

void CGDisplayListImageBufferBackend::putPixelBuffer(const WebCore::PixelBuffer&, const WebCore::IntRect&, const WebCore::IntPoint&, WebCore::AlphaPremultiplication)
{
    ASSERT_NOT_REACHED();
}

String CGDisplayListImageBufferBackend::debugDescription() const
{
    TextStream stream;
    stream << "CGDisplayListImageBufferBackend " << this;
    return stream.release();
}

#pragma mark - CGDisplayListAcceleratedImageBufferBackend

WTF_MAKE_ISO_ALLOCATED_IMPL(CGDisplayListAcceleratedImageBufferBackend);

std::unique_ptr<CGDisplayListAcceleratedImageBufferBackend> CGDisplayListAcceleratedImageBufferBackend::create(const Parameters& parameters, const WebCore::ImageBufferCreationContext& creationContext)
{
    if (parameters.logicalSize.isEmpty())
        return nullptr;

    return std::unique_ptr<CGDisplayListAcceleratedImageBufferBackend>(new CGDisplayListAcceleratedImageBufferBackend(parameters, creationContext, WebCore::RenderingMode::Accelerated));
}

CGDisplayListAcceleratedImageBufferBackend::CGDisplayListAcceleratedImageBufferBackend(const Parameters& parameters, const WebCore::ImageBufferCreationContext& creationContext, WebCore::RenderingMode renderingMode)
    : CGDisplayListImageBufferBackend(parameters, creationContext, renderingMode)
{
}

}

#endif // ENABLE(CG_DISPLAY_LIST_BACKED_IMAGE_BUFFER)
