/*
 * Copyright (C) 2019-2024 Apple Inc. All rights reserved.
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

#pragma once

#if ENABLE(GPU_PROCESS)

#include "Connection.h"
#include "GPUConnectionToWebProcessMessages.h"
#include "GPUProcessPreferencesForWebProcess.h"
#include "MessageReceiverMap.h"
#include "RemoteAudioHardwareListenerIdentifier.h"
#include "RemoteAudioSessionIdentifier.h"
#include "RemoteGPU.h"
#include "RemoteRemoteCommandListenerIdentifier.h"
#include "RemoteSerializedImageBufferIdentifier.h"
#include "RenderingBackendIdentifier.h"
#include "ScopedActiveMessageReceiveQueue.h"
#include "ThreadSafeObjectHeap.h"
#include "WebGPUIdentifier.h"
#include <WebCore/ImageBuffer.h>
#include <WebCore/IntDegrees.h>
#include <WebCore/LibWebRTCEnumTraits.h>
#include <WebCore/NowPlayingManager.h>
#include <WebCore/PageIdentifier.h>
#include <WebCore/ProcessIdentifier.h>
#include <WebCore/ProcessIdentity.h>
#include <WebCore/RenderingResourceIdentifier.h>
#include <pal/SessionID.h>
#include <wtf/Logger.h>
#include <wtf/MachSendRight.h>
#include <wtf/ThreadSafeRefCounted.h>

#if PLATFORM(COCOA) && ENABLE(MEDIA_STREAM)
#include "SampleBufferDisplayLayerIdentifier.h"
#endif

#if ENABLE(WEBGL)
#include "GraphicsContextGLIdentifier.h"
#include <WebCore/GraphicsContextGLAttributes.h>
#endif

#if PLATFORM(MAC)
#include <CoreGraphics/CGDisplayConfiguration.h>
#endif

#if PLATFORM(COCOA)
#include <pal/spi/cocoa/TCCSPI.h>
#endif

#if USE(GRAPHICS_LAYER_WC)
#include "WCLayerTreeHostIdentifier.h"
#endif

#if ENABLE(IPC_TESTING_API)
#include "IPCTester.h"
#endif

namespace WTF {
enum class Critical : bool;
enum class Synchronous : bool;
}

namespace WebCore {
class SecurityOrigin;
class SecurityOriginData;
}

namespace WebKit {

class GPUProcess;
class LayerHostingContext;
class LibWebRTCCodecsProxy;
class LocalAudioSessionRoutingArbitrator;
class RemoteAudioDestinationManager;
class RemoteAudioHardwareListenerProxy;
class RemoteAudioMediaStreamTrackRendererInternalUnitManager;
class RemoteAudioSessionProxy;
class RemoteAudioSessionProxyManager;
class RemoteCDMFactoryProxy;
class RemoteImageDecoderAVFProxy;
class RemoteLegacyCDMFactoryProxy;
class RemoteMediaEngineConfigurationFactoryProxy;
class RemoteMediaSessionHelperProxy;
class RemoteRemoteCommandListenerProxy;
class RemoteRenderingBackend;
class RemoteSampleBufferDisplayLayerManager;
class UserMediaCaptureManagerProxy;
struct GPUProcessConnectionParameters;
struct MediaOverridesForTesting;
struct RemoteAudioSessionConfiguration;
struct RemoteRenderingBackendCreationParameters;

#if USE(GRAPHICS_LAYER_WC)
class RemoteWCLayerTreeHost;
#endif

#if ENABLE(VIDEO)
class RemoteMediaPlayerManagerProxy;
class RemoteMediaRecorderManager;
class RemoteMediaResourceManager;
class RemoteVideoFrameObjectHeap;
#endif

#if ENABLE(WEBGL)
class RemoteGraphicsContextGL;
#endif

class GPUConnectionToWebProcess
    : public ThreadSafeRefCounted<GPUConnectionToWebProcess, WTF::DestructionThread::Main>
    , public WebCore::NowPlayingManager::Client
    , IPC::Connection::Client {
public:
    static Ref<GPUConnectionToWebProcess> create(GPUProcess&, WebCore::ProcessIdentifier, PAL::SessionID, IPC::Connection::Handle&&, GPUProcessConnectionParameters&&);
    virtual ~GPUConnectionToWebProcess();

    bool isWebGPUEnabled() const { return m_preferences.isWebGPUEnabled; }
    bool isWebGLEnabled() const { return m_preferences.isWebGLEnabled; }

#if ENABLE(RE_DYNAMIC_CONTENT_SCALING)
    bool isDynamicContentScalingEnabled() const { return m_preferences.isDynamicContentScalingEnabled; }
#endif

    using WebCore::NowPlayingManager::Client::weakPtrFactory;
    using WebCore::NowPlayingManager::Client::WeakValueType;
    using WebCore::NowPlayingManager::Client::WeakPtrImplType;

    IPC::Connection& connection() { return m_connection.get(); }
    Ref<IPC::Connection> protectedConnection() { return m_connection; }
    IPC::MessageReceiverMap& messageReceiverMap() { return m_messageReceiverMap; }
    GPUProcess& gpuProcess() { return m_gpuProcess.get(); }
    WebCore::ProcessIdentifier webProcessIdentifier() const { return m_webProcessIdentifier; }

#if ENABLE(VIDEO)
    RemoteMediaResourceManager& remoteMediaResourceManager();
#endif

    PAL::SessionID sessionID() const { return m_sessionID; }

    bool isLockdownModeEnabled() const { return m_isLockdownModeEnabled; }
    bool allowTestOnlyIPC() const { return m_preferences.allowTestOnlyIPC; }

    Logger& logger();

    const String& mediaCacheDirectory() const;
#if ENABLE(LEGACY_ENCRYPTED_MEDIA)
    const String& mediaKeysStorageDirectory() const;
#endif

#if ENABLE(MEDIA_STREAM)
    void setOrientationForMediaCapture(WebCore::IntDegrees);
    void updateCaptureAccess(bool allowAudioCapture, bool allowVideoCapture, bool allowDisplayCapture);
    void updateCaptureOrigin(const WebCore::SecurityOriginData&);
    bool setCaptureAttributionString();
    bool allowsAudioCapture() const { return m_allowsAudioCapture; }
    bool allowsVideoCapture() const { return m_allowsVideoCapture; }
    bool allowsDisplayCapture() const { return m_allowsDisplayCapture; }
#endif
#if ENABLE(VIDEO)
    RemoteVideoFrameObjectHeap& videoFrameObjectHeap() const;
#endif
#if PLATFORM(COCOA) && ENABLE(MEDIA_STREAM)
    void startCapturingAudio();
    void processIsStartingToCaptureAudio(GPUConnectionToWebProcess&);
    bool isLastToCaptureAudio() const { return m_isLastToCaptureAudio; }
#endif

#if ENABLE(APP_PRIVACY_REPORT)
    void setTCCIdentity();
#endif

#if PLATFORM(MAC)
    void displayConfigurationChanged(CGDirectDisplayID, CGDisplayChangeSummaryFlags);
#endif
#if PLATFORM(MAC) && ENABLE(WEBGL)
    void dispatchDisplayWasReconfiguredForTesting() { dispatchDisplayWasReconfigured(); };
#endif

    const WebCore::ProcessIdentity& webProcessIdentity() const { return m_webProcessIdentity; }
#if ENABLE(ENCRYPTED_MEDIA)
    RemoteCDMFactoryProxy& cdmFactoryProxy();
#endif
#if ENABLE(LEGACY_ENCRYPTED_MEDIA)
    RemoteLegacyCDMFactoryProxy& legacyCdmFactoryProxy();
#endif
    RemoteMediaEngineConfigurationFactoryProxy& mediaEngineConfigurationFactoryProxy();
#if ENABLE(VIDEO)
    RemoteMediaPlayerManagerProxy& remoteMediaPlayerManagerProxy() { return m_remoteMediaPlayerManagerProxy.get(); }
#endif
#if USE(AUDIO_SESSION)
    RemoteAudioSessionProxyManager& audioSessionManager();
#endif

#if HAVE(AVASSETREADER)
    RemoteImageDecoderAVFProxy& imageDecoderAVFProxy();
#endif

    void updateSupportedRemoteCommands();

    bool allowsExitUnderMemoryPressure() const;
    void terminateWebProcess();

    void lowMemoryHandler(WTF::Critical, WTF::Synchronous);

    IPC::ThreadSafeObjectHeap<RemoteSerializedImageBufferIdentifier, RefPtr<WebCore::ImageBuffer>>& serializedImageBufferHeap() { return m_remoteSerializedImageBufferObjectHeap; }

#if ENABLE(WEBGL)
    void releaseGraphicsContextGLForTesting(GraphicsContextGLIdentifier);
#endif

    static uint64_t objectCountForTesting() { return gObjectCountForTesting; }

    using RemoteRenderingBackendMap = HashMap<RenderingBackendIdentifier, IPC::ScopedActiveMessageReceiveQueue<RemoteRenderingBackend>>;
    const RemoteRenderingBackendMap& remoteRenderingBackendMap() const { return m_remoteRenderingBackendMap; }

    RemoteRenderingBackend* remoteRenderingBackend(RenderingBackendIdentifier);

#if HAVE(AUDIT_TOKEN)
    const std::optional<audit_token_t>& presentingApplicationAuditToken() const { return m_presentingApplicationAuditToken; }
#endif

#if ENABLE(VIDEO)
    void performWithMediaPlayerOnMainThread(WebCore::MediaPlayerIdentifier, Function<void(WebCore::MediaPlayer&)>&&);
#endif

#if PLATFORM(IOS_FAMILY)
    void overridePresentingApplicationPIDIfNeeded();
#endif

#if ENABLE(EXTENSION_CAPABILITIES)
    String mediaEnvironment(WebCore::PageIdentifier);
    void setMediaEnvironment(WebCore::PageIdentifier, const String&);
#endif

private:
    GPUConnectionToWebProcess(GPUProcess&, WebCore::ProcessIdentifier, PAL::SessionID, IPC::Connection::Handle&&, GPUProcessConnectionParameters&&);

#if ENABLE(WEB_AUDIO)
    RemoteAudioDestinationManager& remoteAudioDestinationManager();
#endif
#if PLATFORM(COCOA) && ENABLE(MEDIA_STREAM)
    UserMediaCaptureManagerProxy& userMediaCaptureManagerProxy();
    RemoteAudioMediaStreamTrackRendererInternalUnitManager& audioMediaStreamTrackRendererInternalUnitManager();
#endif
#if PLATFORM(COCOA) && ENABLE(MEDIA_STREAM)
    RemoteMediaRecorderManager& mediaRecorderManager();
#endif

    void createRenderingBackend(RemoteRenderingBackendCreationParameters&&, IPC::StreamServerConnection::Handle&&);
    void releaseRenderingBackend(RenderingBackendIdentifier);
    void releaseSerializedImageBuffer(WebCore::RenderingResourceIdentifier);

#if ENABLE(WEBGL)
    void createGraphicsContextGL(WebCore::GraphicsContextGLAttributes, GraphicsContextGLIdentifier, RenderingBackendIdentifier, IPC::StreamServerConnection::Handle&&);
    void releaseGraphicsContextGL(GraphicsContextGLIdentifier);
#endif

    void createRemoteGPU(WebGPUIdentifier, RenderingBackendIdentifier, IPC::StreamServerConnection::Handle&&);
    void releaseRemoteGPU(WebGPUIdentifier);

    void clearNowPlayingInfo();
    void setNowPlayingInfo(WebCore::NowPlayingInfo&&);

#if ENABLE(MEDIA_SOURCE)
    void enableMockMediaSource();
#endif
#if PLATFORM(COCOA) && ENABLE(MEDIA_STREAM)
    void updateSampleBufferDisplayLayerBoundsAndPosition(WebKit::SampleBufferDisplayLayerIdentifier, WebCore::FloatRect, std::optional<MachSendRight>&&);
#endif

#if HAVE(VISIBILITY_PROPAGATION_VIEW)
    void createVisibilityPropagationContextForPage(WebPageProxyIdentifier, WebCore::PageIdentifier, bool canShowWhileLocked);
    void destroyVisibilityPropagationContextForPage(WebPageProxyIdentifier, WebCore::PageIdentifier);
#endif

#if USE(AUDIO_SESSION)
    RemoteAudioSessionProxy& audioSessionProxy();
    using EnsureAudioSessionCompletion = CompletionHandler<void(const RemoteAudioSessionConfiguration&)>;
    void ensureAudioSession(EnsureAudioSessionCompletion&&);
#endif

#if PLATFORM(IOS_FAMILY)
    RemoteMediaSessionHelperProxy& mediaSessionHelperProxy();
    void ensureMediaSessionHelper();
#endif

    void createAudioHardwareListener(RemoteAudioHardwareListenerIdentifier);
    void releaseAudioHardwareListener(RemoteAudioHardwareListenerIdentifier);
    void createRemoteCommandListener(RemoteRemoteCommandListenerIdentifier);
    void releaseRemoteCommandListener(RemoteRemoteCommandListenerIdentifier);
    void setMediaOverridesForTesting(MediaOverridesForTesting);
    void configureLoggingChannel(const String&, WTFLogChannelState, WTFLogLevel);

#if USE(GRAPHICS_LAYER_WC)
    void createWCLayerTreeHost(WebKit::WCLayerTreeHostIdentifier, uint64_t nativeWindow, bool usesOffscreenRendering);
    void releaseWCLayerTreeHost(WebKit::WCLayerTreeHostIdentifier);
#endif

    // IPC::Connection::Client
    void didClose(IPC::Connection&) final;
    void didReceiveInvalidMessage(IPC::Connection&, IPC::MessageName) final;
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;
    bool didReceiveSyncMessage(IPC::Connection&, IPC::Decoder&, UniqueRef<IPC::Encoder>&) final;

    bool dispatchMessage(IPC::Connection&, IPC::Decoder&);
    bool dispatchSyncMessage(IPC::Connection&, IPC::Decoder&, UniqueRef<IPC::Encoder>&);

    // NowPlayingManager::Client
    void didReceiveRemoteControlCommand(WebCore::PlatformMediaSession::RemoteControlCommandType, const WebCore::PlatformMediaSession::RemoteCommandArgument&) final;

#if PLATFORM(MAC) && ENABLE(WEBGL)
    void dispatchDisplayWasReconfigured();
#endif

    static uint64_t gObjectCountForTesting;

    RefPtr<Logger> m_logger;

    Ref<IPC::Connection> m_connection;
    IPC::MessageReceiverMap m_messageReceiverMap;
    Ref<GPUProcess> m_gpuProcess;
    const WebCore::ProcessIdentifier m_webProcessIdentifier;
    const WebCore::ProcessIdentity m_webProcessIdentity;
#if ENABLE(WEB_AUDIO)
    std::unique_ptr<RemoteAudioDestinationManager> m_remoteAudioDestinationManager;
#endif
#if ENABLE(VIDEO)
    std::unique_ptr<RemoteMediaResourceManager> m_remoteMediaResourceManager;
    UniqueRef<RemoteMediaPlayerManagerProxy> m_remoteMediaPlayerManagerProxy;
#endif
    PAL::SessionID m_sessionID;
#if PLATFORM(COCOA) && ENABLE(MEDIA_STREAM)
    std::unique_ptr<UserMediaCaptureManagerProxy> m_userMediaCaptureManagerProxy;
    std::unique_ptr<RemoteAudioMediaStreamTrackRendererInternalUnitManager> m_audioMediaStreamTrackRendererInternalUnitManager;
    bool m_isLastToCaptureAudio { false };

    Ref<RemoteSampleBufferDisplayLayerManager> m_sampleBufferDisplayLayerManager;

    std::unique_ptr<RemoteMediaRecorderManager> m_remoteMediaRecorderManager;
#endif
#if ENABLE(MEDIA_STREAM)
    Ref<WebCore::SecurityOrigin> m_captureOrigin;
    bool m_allowsAudioCapture { false };
    bool m_allowsVideoCapture { false };
    bool m_allowsDisplayCapture { false };
#endif
#if ENABLE(VIDEO)
    IPC::ScopedActiveMessageReceiveQueue<RemoteVideoFrameObjectHeap> m_videoFrameObjectHeap;
#endif
#if PLATFORM(COCOA) && USE(LIBWEBRTC)
    IPC::ScopedActiveMessageReceiveQueue<LibWebRTCCodecsProxy> m_libWebRTCCodecsProxy;
#endif
#if HAVE(AUDIT_TOKEN)
    const std::optional<audit_token_t> m_presentingApplicationAuditToken;
#endif

    RemoteRenderingBackendMap m_remoteRenderingBackendMap;
#if ENABLE(WEBGL)
    using RemoteGraphicsContextGLMap = HashMap<GraphicsContextGLIdentifier, IPC::ScopedActiveMessageReceiveQueue<RemoteGraphicsContextGL>>;
    RemoteGraphicsContextGLMap m_remoteGraphicsContextGLMap;
#endif
    IPC::ThreadSafeObjectHeap<RemoteSerializedImageBufferIdentifier, RefPtr<WebCore::ImageBuffer>> m_remoteSerializedImageBufferObjectHeap;
    using RemoteGPUMap = HashMap<WebGPUIdentifier, IPC::ScopedActiveMessageReceiveQueue<RemoteGPU>>;
    RemoteGPUMap m_remoteGPUMap;
#if ENABLE(ENCRYPTED_MEDIA)
    std::unique_ptr<RemoteCDMFactoryProxy> m_cdmFactoryProxy;
#endif
#if USE(AUDIO_SESSION)
    std::unique_ptr<RemoteAudioSessionProxy> m_audioSessionProxy;
#endif
#if PLATFORM(IOS_FAMILY)
    std::unique_ptr<RemoteMediaSessionHelperProxy> m_mediaSessionHelperProxy;
#endif
#if ENABLE(LEGACY_ENCRYPTED_MEDIA)
    std::unique_ptr<RemoteLegacyCDMFactoryProxy> m_legacyCdmFactoryProxy;
#endif
#if HAVE(AVASSETREADER)
    std::unique_ptr<RemoteImageDecoderAVFProxy> m_imageDecoderAVFProxy;
#endif

    std::unique_ptr<RemoteMediaEngineConfigurationFactoryProxy> m_mediaEngineConfigurationFactoryProxy;

#if HAVE(VISIBILITY_PROPAGATION_VIEW)
    HashMap<std::pair<WebPageProxyIdentifier, WebCore::PageIdentifier>, std::unique_ptr<LayerHostingContext>> m_visibilityPropagationContexts;
#endif

    using RemoteAudioHardwareListenerMap = HashMap<RemoteAudioHardwareListenerIdentifier, std::unique_ptr<RemoteAudioHardwareListenerProxy>>;
    RemoteAudioHardwareListenerMap m_remoteAudioHardwareListenerMap;

#if USE(GRAPHICS_LAYER_WC)
    using RemoteWCLayerTreeHostMap = HashMap<WCLayerTreeHostIdentifier, std::unique_ptr<RemoteWCLayerTreeHost>>;
    RemoteWCLayerTreeHostMap m_remoteWCLayerTreeHostMap;
#endif

    RefPtr<RemoteRemoteCommandListenerProxy> m_remoteRemoteCommandListener;
    bool m_isActiveNowPlayingProcess { false };
    const bool m_isLockdownModeEnabled { false };
#if ENABLE(MEDIA_SOURCE)
    bool m_mockMediaSourceEnabled { false };
#endif

#if ENABLE(EXTENSION_CAPABILITIES)
    HashMap<WebCore::PageIdentifier, String> m_mediaEnvironments;
#endif

#if ENABLE(ROUTING_ARBITRATION) && HAVE(AVAUDIO_ROUTING_ARBITER)
    UniqueRef<LocalAudioSessionRoutingArbitrator> m_routingArbitrator;
#endif
#if ENABLE(IPC_TESTING_API)
    IPCTester m_ipcTester;
#endif
    // GPU preferences don't change for a given WebProcess. Pages that use different GPUProcessPreferences
    // cannot be in the same WebProcess.
    const GPUProcessPreferencesForWebProcess m_preferences;
};

} // namespace WebKit

#endif // ENABLE(GPU_PROCESS)
