/*
 *  Copyright (c) 2015, Canon Inc. All rights reserved.
 *  Copyright (C) 2018-2023 Apple Inc. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1.  Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *  2.  Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *  3.  Neither the name of Canon Inc. nor the names of
 *      its contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *  THIS SOFTWARE IS PROVIDED BY CANON INC. AND ITS CONTRIBUTORS "AS IS" AND ANY
 *  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL CANON INC. AND ITS CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <JavaScriptCore/BuiltinUtils.h>

#if USE(APPLE_INTERNAL_SDK)
#include <WebKitAdditions/WebCoreBuiltinNamesAdditions.h>
#endif

namespace WebCore {

#if !defined(WEBCORE_ADDITIONAL_PRIVATE_IDENTIFIERS)
#define WEBCORE_ADDITIONAL_PRIVATE_IDENTIFIERS(macro)
#endif

#if !defined(WEBCORE_COMMON_PRIVATE_IDENTIFIERS_FOR_SHADOWREALM_EACH_PROPERTY_NAME)
// Dom APIs exposed to ShadowRealm are temporarily enabled via the WebAPIsInShadowRealmEnabled
// experimental feature. When this is resolved, these can be deleted.
#define WEBCORE_COMMON_PRIVATE_IDENTIFIERS_FOR_SHADOWREALM_EACH_PROPERTY_NAME(macro) \
    macro(AbortController) \
    macro(ByteLengthQueuingStrategy) \
    macro(CountQueuingStrategy) \
    macro(CustomEvent) \
    macro(ErrorEvent) \
    macro(Event) \
    macro(EventTarget) \
    macro(Performance) \
    macro(PromiseRejectionEvent) \
    macro(TextDecoder) \
    macro(TextEncoder) \
    macro(URL) \
    macro(URLSearchParams) \
    macro(UserActivation)
#endif

#define WEBCORE_COMMON_PRIVATE_IDENTIFIERS_EACH_PROPERTY_NAME(macro) \
    macro(AbortSignal) \
    macro(AbstractRange) \
    macro(AnalyserNode) \
    macro(Animation) \
    macro(AnimationEffect) \
    macro(AnimationPlaybackEvent) \
    macro(AnimationTimeline) \
    macro(ApplePaySession) \
    macro(ApplePaySetup) \
    macro(ApplePaySetupFeature) \
    macro(ApplicationCache) \
    macro(AttachmentElement) \
    macro(Audio) \
    macro(AudioBuffer) \
    macro(AudioBufferSourceNode) \
    macro(AudioContext) \
    macro(AudioData) \
    macro(AudioDecoder) \
    macro(AudioEncoder) \
    macro(AudioDestinationNode) \
    macro(AudioListener) \
    macro(AudioNode) \
    macro(AudioParam) \
    macro(AudioParamMap) \
    macro(AudioProcessingEvent) \
    macro(AudioScheduledSourceNode) \
    macro(AudioSession) \
    macro(AudioWorklet) \
    macro(AudioWorkletGlobalScope) \
    macro(AudioWorkletNode) \
    macro(AudioWorkletProcessor) \
    macro(AuthenticatorAssertionResponse) \
    macro(AuthenticatorAttestationResponse) \
    macro(AuthenticatorResponse) \
    macro(BackgroundFetchEvent) \
    macro(BackgroundFetchManager) \
    macro(BackgroundFetchRecord) \
    macro(BackgroundFetchRegistration) \
    macro(BackgroundFetchUpdateUIEvent) \
    macro(BarcodeDetector) \
    macro(BaseAudioContext) \
    macro(BiquadFilterNode) \
    macro(BlobEvent) \
    macro(BroadcastChannel) \
    macro(BufferedChangeEvent) \
    macro(Cache) \
    macro(CacheStorage) \
    macro(ChannelMergerNode) \
    macro(ChannelSplitterNode) \
    macro(Client) \
    macro(Clients) \
    macro(Clipboard) \
    macro(ClipboardItem) \
    macro(CompressionStream) \
    macro(CompressionStreamEncoder) \
    macro(CompressionStreamTransform) \
    macro(ConstantSourceNode) \
    macro(ContactsManager) \
    macro(ContentVisibilityAutoStateChangeEvent) \
    macro(ConvolverNode) \
    macro(Credential) \
    macro(CredentialsContainer) \
    macro(CryptoKey) \
    macro(CSPViolationReportBody) \
    macro(CSSAnimation) \
    macro(CSSCounterStyleRule) \
    macro(CSSColor) \
    macro(CSSColorValue) \
    macro(CSSHSL) \
    macro(CSSHWB) \
    macro(CSSImageValue) \
    macro(CSSKeywordValue) \
    macro(CSSLCH) \
    macro(CSSLab) \
    macro(CSSMathClamp) \
    macro(CSSMathInvert) \
    macro(CSSMathMax) \
    macro(CSSMathMin) \
    macro(CSSMathNegate) \
    macro(CSSMathProduct) \
    macro(CSSMathSum) \
    macro(CSSMathValue) \
    macro(CSSMatrixComponent) \
    macro(CSSNumericArray) \
    macro(CSSNumericValue) \
    macro(CSSOKLCH) \
    macro(CSSOKLab) \
    macro(CSSPaintSize) \
    macro(CSSPerspective) \
    macro(CSSRGB) \
    macro(CSSRotate) \
    macro(CSSScale) \
    macro(CSSScopeRule) \
    macro(CSSSkew) \
    macro(CSSSkewX) \
    macro(CSSSkewY) \
    macro(CSSStyleValue) \
    macro(CSSTransformComponent) \
    macro(CSSTransformValue) \
    macro(CSSTransition) \
    macro(CSSTranslate) \
    macro(CSSUnitValue) \
    macro(CSSUnparsedValue) \
    macro(CSSVariableReferenceValue) \
    macro(CookieChangeEvent) \
    macro(CookieStore) \
    macro(CookieStoreManager) \
    macro(CustomElementRegistry) \
    macro(CustomEffect) \
    macro(CustomStateSet) \
    macro(Database) \
    macro(DataTransferItem) \
    macro(DataTransferItemList) \
    macro(DecompressionStream) \
    macro(DecompressionStreamDecoder) \
    macro(DecompressionStreamTransform) \
    macro(DelayNode) \
    macro(DeprecationReportBody) \
    macro(DocumentTimeline) \
    macro(DynamicsCompressorNode) \
    macro(ElementInternals) \
    macro(EncodedAudioChunk) \
    macro(EncodedVideoChunk) \
    macro(ExtendableCookieChangeEvent) \
    macro(ExtendableEvent) \
    macro(ExtendableMessageEvent) \
    macro(FakeXRDevice) \
    macro(FakeXRInputController) \
    macro(FaceDetector) \
    macro(FetchEvent) \
    macro(FileSystem) \
    macro(FileSystemDirectoryEntry) \
    macro(FileSystemDirectoryHandle) \
    macro(FileSystemDirectoryHandleIterator) \
    macro(FileSystemDirectoryReader) \
    macro(FileSystemEntry) \
    macro(FileSystemFileEntry) \
    macro(FileSystemFileHandle) \
    macro(FileSystemHandle) \
    macro(FileSystemSyncAccessHandle) \
    macro(GainNode) \
    macro(GPU) \
    macro(GPUAdapter) \
    macro(GPUAdapterInfo) \
    macro(GPUBindGroup) \
    macro(GPUBindGroupLayout) \
    macro(GPUBuffer) \
    macro(GPUBufferUsage) \
    macro(GPUCanvasContext) \
    macro(GPUColorWrite) \
    macro(GPUCommandBuffer) \
    macro(GPUCommandEncoder) \
    macro(GPUCompilationInfo) \
    macro(GPUCompilationMessage) \
    macro(GPUComputePassEncoder) \
    macro(GPUComputePipeline) \
    macro(GPUDevice) \
    macro(GPUDeviceLostInfo) \
    macro(GPUExternalTexture) \
    macro(GPUInternalError) \
    macro(GPUMapMode) \
    macro(GPUOutOfMemoryError) \
    macro(GPUPipelineLayout) \
    macro(GPUQuerySet) \
    macro(GPUQueue) \
    macro(GPUPipelineError) \
    macro(GPURenderBundle) \
    macro(GPURenderBundleEncoder) \
    macro(GPURenderPassEncoder) \
    macro(GPURenderPipeline) \
    macro(GPUSampler) \
    macro(GPUShaderModule) \
    macro(GPUShaderStage) \
    macro(GPUSupportedFeatures) \
    macro(GPUSupportedLimits) \
    macro(GPUTexture) \
    macro(GPUTextureUsage) \
    macro(GPUTextureView) \
    macro(GPUUncapturedErrorEvent) \
    macro(GPUValidationError) \
    macro(Gamepad) \
    macro(GamepadButton) \
    macro(GamepadEvent) \
    macro(GamepadHapticActuator) \
    macro(HighlightRegistry) \
    macro(Highlight) \
    macro(HTMLAttachmentElement) \
    macro(HTMLAudioElement) \
    macro(HTMLDialogElement) \
    macro(HTMLDataListElement) \
    macro(HTMLEmbedElement) \
    macro(HTMLModelElement) \
    macro(Headers) \
    macro(IDBCursor) \
    macro(IDBCursorWithValue) \
    macro(IDBDatabase) \
    macro(IDBFactory) \
    macro(IDBIndex) \
    macro(IDBKeyRange) \
    macro(IDBObjectStore) \
    macro(IDBOpenDBRequest) \
    macro(IDBRequest) \
    macro(IDBTransaction) \
    macro(IDBVersionChangeEvent) \
    macro(IIRFilterNode) \
    macro(ImageBitmap) \
    macro(ImageBitmapRenderingContext) \
    macro(ImageCapture) \
    macro(IdleDeadline) \
    macro(InputDeviceInfo) \
    macro(InputEvent) \
    macro(IntersectionObserver) \
    macro(IntersectionObserverEntry) \
    macro(InvokeEvent) \
    macro(KeyframeEffect) \
    macro(Lock) \
    macro(LockManager) \
    macro(ManagedMediaSource) \
    macro(ManagedSourceBuffer) \
    macro(MathMLElement) \
    macro(MathMLMathElement) \
    macro(MediaCapabilities) \
    macro(MediaCapabilitiesInfo) \
    macro(MediaDevices) \
    macro(MediaElementAudioSourceNode) \
    macro(MediaEncryptedEvent) \
    macro(MediaKeyMessageEvent) \
    macro(MediaKeySession) \
    macro(MediaKeyStatusMap) \
    macro(MediaKeySystemAccess) \
    macro(MediaKeys) \
    macro(MediaQueryListEvent) \
    macro(MediaRecorder) \
    macro(MediaRecorderErrorEvent) \
    macro(MediaSessionCoordinator) \
    macro(MediaSource) \
    macro(MediaStream) \
    macro(MediaStreamAudioDestinationNode) \
    macro(MediaStreamAudioSourceNode) \
    macro(MediaStreamTrack) \
    macro(MerchantValidationEvent) \
    macro(MockRTCRtpTransform) \
    macro(NavigateEvent) \
    macro(Navigation) \
    macro(NavigationCurrentEntryChangeEvent) \
    macro(NavigationDestination) \
    macro(NavigationHistoryEntry) \
    macro(NavigationPreloadManager) \
    macro(NavigationTransition) \
    macro(NavigatorCredentials) \
    macro(NavigatorMediaDevices) \
    macro(NavigatorPermissions) \
    macro(NavigatorUserMedia) \
    macro(Notification) \
    macro(NotificationEvent) \
    macro(OfflineAudioCompletionEvent) \
    macro(OfflineAudioContext) \
    macro(OffscreenCanvas) \
    macro(OffscreenCanvasRenderingContext2D) \
    macro(OscillatorNode) \
    macro(PaintRenderingContext2D) \
    macro(PannerNode) \
    macro(PaymentAddress) \
    macro(PaymentMethodChangeEvent) \
    macro(PaymentRequest) \
    macro(PaymentRequestUpdateEvent) \
    macro(PaymentResponse) \
    macro(PerformanceNavigationTiming) \
    macro(PictureInPictureEvent) \
    macro(PictureInPictureWindow) \
    macro(PaintWorkletGlobalScope) \
    macro(PerformanceEntry) \
    macro(PerformanceEntryList) \
    macro(PerformanceMark) \
    macro(PerformanceMeasure) \
    macro(PerformanceObserver) \
    macro(PerformanceObserverEntryList) \
    macro(PerformancePaintTiming) \
    macro(PerformanceResourceTiming) \
    macro(PerformanceServerTiming) \
    macro(PeriodicWave) \
    macro(PermissionDescriptor) \
    macro(PermissionStatus) \
    macro(Permissions) \
    macro(PointerEvent) \
    macro(PublicKeyCredential) \
    macro(PushEvent) \
    macro(PushManager) \
    macro(PushMessageData) \
    macro(PushNotificationEvent) \
    macro(PushSubscription) \
    macro(PushSubscriptionChangeEvent) \
    macro(PushSubscriptionOptions) \
    macro(Report) \
    macro(ReportBody) \
    macro(ReportingObserver) \
    macro(ResizeObserver) \
    macro(ResizeObserverEntry) \
    macro(RTCCertificate) \
    macro(RTCDTMFSender) \
    macro(RTCDTMFToneChangeEvent) \
    macro(RTCDataChannel) \
    macro(RTCDataChannelEvent) \
    macro(RTCDtlsTransport) \
    macro(RTCEncodedAudioFrame) \
    macro(RTCEncodedVideoFrame) \
    macro(RTCError) \
    macro(RTCErrorEvent) \
    macro(RTCIceCandidate) \
    macro(RTCIceTransport) \
    macro(RTCPeerConnection) \
    macro(RTCPeerConnectionIceEvent) \
    macro(RTCPeerConnectionIceErrorEvent) \
    macro(RTCRtpReceiver) \
    macro(RTCRtpScriptTransform) \
    macro(RTCRtpScriptTransformer) \
    macro(RTCRtpScriptTransformerContext) \
    macro(RTCRtpSender) \
    macro(RTCRtpTransceiver) \
    macro(RTCRtpTransform) \
    macro(RTCSctpTransport) \
    macro(RTCSessionDescription) \
    macro(RTCStatsReport) \
    macro(RTCTrackEvent) \
    macro(RTCTransformEvent) \
    macro(ReadableByteStreamController) \
    macro(ReadableStream) \
    macro(ReadableStreamBYOBReader) \
    macro(ReadableStreamBYOBRequest) \
    macro(ReadableStreamDefaultController) \
    macro(ReadableStreamDefaultReader) \
    macro(RemotePlayback) \
    macro(Request) \
    macro(Response) \
    macro(SFrameTransform) \
    macro(SFrameTransformErrorEvent) \
    macro(SQLError) \
    macro(SQLResultSet) \
    macro(SQLResultSetRowList) \
    macro(SQLTransaction) \
    macro(ScreenLuminance) \
    macro(ScreenOrientation) \
    macro(ScrollTimeline) \
    macro(ServiceWorker) \
    macro(ServiceWorkerContainer) \
    macro(ServiceWorkerGlobalScope) \
    macro(ServiceWorkerRegistration) \
    macro(ShadowRoot) \
    macro(SharedWorker) \
    macro(SharedWorkerGlobalScope) \
    macro(SpeechRecognition) \
    macro(SpeechRecognitionAlternative) \
    macro(SpeechRecognitionErrorEvent) \
    macro(SpeechRecognitionEvent) \
    macro(SpeechRecognitionResult) \
    macro(SpeechRecognitionResultList) \
    macro(SourceBuffer) \
    macro(SourceBufferList) \
    macro(StaticRange) \
    macro(StereoPannerNode) \
    macro(StylePropertyMapReadOnly) \
    macro(StylePropertyMap) \
    macro(StorageManager) \
    macro(StorageManagerFileSystemAccess) \
    macro(SubtleCrypto) \
    macro(TestReportBody) \
    macro(TextDecoderStream) \
    macro(TextDecoderStreamDecoder) \
    macro(TextDetector) \
    macro(TextEncoderStream) \
    macro(TextEncoderStreamEncoder) \
    macro(TextTrackCue) \
    macro(TextTrackCueGeneric) \
    macro(ToggleEvent) \
    macro(TransformStream) \
    macro(TransformStreamDefaultController) \
    macro(UndoItem) \
    macro(UndoManager) \
    macro(VideoDecoder) \
    macro(VideoEncoder) \
    macro(VideoFrame) \
    macro(ViewTimeline) \
    macro(ViewTransition) \
    macro(VisualViewport) \
    macro(WakeLock) \
    macro(WakeLockSentinel) \
    macro(WaveShaperNode) \
    macro(WebGL2RenderingContext) \
    macro(WebGLActiveInfo) \
    macro(WebGLBuffer) \
    macro(WebGLContextEvent) \
    macro(WebGLFramebuffer) \
    macro(WebGLProgram) \
    macro(WebGLQuery) \
    macro(WebGLRenderbuffer) \
    macro(WebGLRenderingContext) \
    macro(WebGLSampler) \
    macro(WebGLShader) \
    macro(WebGLShaderPrecisionFormat) \
    macro(WebGLSync) \
    macro(WebGLTexture) \
    macro(WebGLTransformFeedback) \
    macro(WebGLUniformLocation) \
    macro(WebGLVertexArrayObject) \
    macro(WebKitMediaKeyError) \
    macro(WebKitMediaKeyMessageEvent) \
    macro(WebKitMediaKeyNeededEvent) \
    macro(WebKitMediaKeySession) \
    macro(WebKitMediaKeys) \
    macro(WebSocket) \
    macro(WebTransport) \
    macro(WebTransportBidirectionalStream) \
    macro(WebTransportDatagramDuplexStream) \
    macro(WebTransportError) \
    macro(WebTransportReceiveStream) \
    macro(WebTransportSendStream) \
    macro(WindowClient) \
    macro(Worklet) \
    macro(WorkletGlobalScope) \
    macro(WritableStream) \
    macro(WritableStreamDefaultController) \
    macro(WritableStreamDefaultWriter) \
    macro(XMLHttpRequest) \
    macro(XRBoundedReferenceSpace) \
    macro(XRFrame) \
    macro(XRHand) \
    macro(XRInputSource) \
    macro(XRInputSourceArray) \
    macro(XRInputSourceEvent) \
    macro(XRInputSourcesChangeEvent) \
    macro(XRJointPose) \
    macro(XRJointSpace) \
    macro(XRLayer) \
    macro(XRPose) \
    macro(XRReferenceSpace) \
    macro(XRReferenceSpaceEvent) \
    macro(XRRenderState) \
    macro(XRRigidTransform) \
    macro(XRSession) \
    macro(XRSessionEvent) \
    macro(XRSpace) \
    macro(XRSystem) \
    macro(XRTest) \
    macro(XRView) \
    macro(XRViewerPose) \
    macro(XRViewport) \
    macro(XRWebGLLayer) \
    macro(abortAlgorithm) \
    macro(abortSteps) \
    macro(addAbortAlgorithmToSignal) \
    macro(appendFromJS) \
    macro(applicationCache) \
    macro(associatedReadableByteStreamController) \
    macro(autoAllocateChunkSize) \
    macro(backingMap) \
    macro(backingSet) \
    macro(backpressure) \
    macro(backpressureChangePromise) \
    macro(blur) \
    macro(body) \
    macro(byobRequest) \
    macro(cachedAttrAssociatedElements) \
    macro(caches) \
    macro(cancel) \
    macro(cancelAlgorithm) \
    macro(cancelAnimationFrame) \
    macro(cancelIdleCallback) \
    macro(cloneArrayBuffer) \
    macro(close) \
    macro(closeAlgorithm) \
    macro(closeRequest) \
    macro(closeRequested) \
    macro(closed) \
    macro(closedPromise) \
    macro(closedPromiseCapability) \
    macro(collectMatchingElementsInFlatTree) \
    macro(consume) \
    macro(consumeChunk) \
    macro(controlledReadableStream) \
    macro(controller) \
    macro(cookieStore) \
    macro(createAbortSignal) \
    macro(createImageBitmap) \
    macro(createReadableStream) \
    macro(createWritableStreamFromInternal) \
    macro(crossOriginIsolated) \
    macro(customElements) \
    macro(decode) \
    macro(disturbed) \
    macro(document) \
    macro(encode) \
    macro(encoding) \
    macro(errorSteps) \
    macro(failureKind) \
    macro(fatal) \
    macro(fetch) \
    macro(fetchRequest) \
    macro(FileReader) \
    macro(FileReaderSync) \
    macro(fillFromJS) \
    macro(finishConsumingStream) \
    macro(flush) \
    macro(flushAlgorithm) \
    macro(focus) \
    macro(frames) \
    macro(getGlobalObject) \
    macro(getInternalReadableStream) \
    macro(getInternalWritableStream) \
    macro(getMatchedCSSRules) \
    macro(getTracks) \
    macro(getUserMedia) \
    macro(globalObject) \
    macro(gpu) \
    macro(handleEvent) \
    macro(header) \
    macro(highWaterMark) \
    macro(href) \
    macro(ignoreBOM) \
    macro(inFlightCloseRequest) \
    macro(inFlightWriteRequest) \
    macro(indexedDB) \
    macro(initializeWith) \
    macro(internalReadable) \
    macro(internalStream) \
    macro(internalWritable) \
    macro(isAbortSignal) \
    macro(isDisturbed) \
    macro(isLoading) \
    macro(isSecureContext) \
    macro(kind) \
    macro(language) \
    macro(localStreams) \
    macro(location) \
    macro(makeDOMException) \
    macro(makeGetterTypeError) \
    macro(makeThisTypeError) \
    macro(matchingElementInFlatTree) \
    macro(mediaStreamTrackConstraints) \
    macro(navigation) \
    macro(onbackgroundfetchsuccess) \
    macro(onbackgroundfetchfail) \
    macro(onbackgroundfetchabort) \
    macro(onbackgroundfetchclick) \
    macro(oncookiechange) \
    macro(onnotificationclick) \
    macro(onnotificationclose) \
    macro(onpush) \
    macro(onpushsubscriptionchange) \
    macro(onrtctransform) \
    macro(onsearch) \
    macro(ontouchcancel) \
    macro(ontouchend) \
    macro(ontouchmove) \
    macro(ontouchstart) \
    macro(ontouchforcechange) \
    macro(onuncapturederror) \
    macro(onvrdisplayactivate) \
    macro(onvrdisplayblur) \
    macro(onvrdisplayconnect) \
    macro(onvrdisplaydeactivate) \
    macro(onvrdisplaydisconnect) \
    macro(onvrdisplayfocus) \
    macro(onvrdisplaypresentchange) \
    macro(openDatabase) \
    macro(opener) \
    macro(operations) \
    macro(ownerReadableStream) \
    macro(parent) \
    macro(pendingPullIntos) \
    macro(postMessage) \
    macro(privateGetStats) \
    macro(pull) \
    macro(pullAgain) \
    macro(pullAlgorithm) \
    macro(pulling) \
    macro(queue) \
    macro(queuedAddIceCandidate) \
    macro(queuedCreateAnswer) \
    macro(queuedCreateOffer) \
    macro(queuedSetLocalDescription) \
    macro(queuedSetRemoteDescription) \
    macro(readable) \
    macro(readIntoRequests) \
    macro(readRequests) \
    macro(readableByteStreamAPIEnabled) \
    macro(readableStreamController) \
    macro(reader) \
    macro(readyPromise) \
    macro(readyPromiseCapability) \
    macro(removeAbortAlgorithmFromSignal) \
    macro(requestAnimationFrame) \
    macro(requestIdleCallback) \
    macro(response) \
    macro(responseCacheIsValid) \
    macro(retrieveResponse) \
    macro(self) \
    macro(setBody) \
    macro(setBodyFromInputRequest) \
    macro(setStatus) \
    macro(showModalDialog) \
    macro(signal) \
    macro(signalAbort) \
    macro(SpeechSynthesis) \
    macro(SpeechSynthesisErrorEvent) \
    macro(SpeechSynthesisEvent) \
    macro(SpeechSynthesisUtterance) \
    macro(SpeechSynthesisVoice) \
    macro(start) \
    macro(startConsumingStream) \
    macro(started) \
    macro(startedPromise) \
    macro(state) \
    macro(storedError) \
    macro(strategy) \
    macro(strategyHWM) \
    macro(strategySizeAlgorithm) \
    macro(stream) \
    macro(streamClosed) \
    macro(streamClosing) \
    macro(streamErrored) \
    macro(streamReadable) \
    macro(streamWaiting) \
    macro(streamWritable) \
    macro(structuredCloneForStream) \
    macro(timeline) \
    macro(top) \
    macro(textDecoderStreamDecoder) \
    macro(textDecoderStreamTransform) \
    macro(textEncoderStreamEncoder) \
    macro(textEncoderStreamTransform) \
    macro(transformAlgorithm) \
    macro(underlyingByteSource) \
    macro(underlyingSink) \
    macro(underlyingSource) \
    macro(view) \
    macro(visualViewport) \
    macro(webkit) \
    macro(webkitAudioContext) \
    macro(webkitAudioPannerNode) \
    macro(webkitIDBCursor) \
    macro(webkitIDBDatabase) \
    macro(webkitIDBFactory) \
    macro(webkitIDBIndex) \
    macro(webkitIDBKeyRange) \
    macro(webkitIDBObjectStore) \
    macro(webkitIDBRequest) \
    macro(webkitIDBTransaction) \
    macro(webkitIndexedDB) \
    macro(webkitOfflineAudioContext) \
    macro(webkitOscillatorNode) \
    macro(webkitSpeechRecognition) \
    macro(window) \
    macro(writable) \
    macro(writeAlgorithm) \
    macro(writing) \
    macro(writer) \
    macro(pendingAbortRequest) \
    macro(writeRequests) \
    WEBCORE_ADDITIONAL_PRIVATE_IDENTIFIERS(macro) \
    WEBCORE_COMMON_PRIVATE_IDENTIFIERS_FOR_SHADOWREALM_EACH_PROPERTY_NAME(macro) \

class WebCoreBuiltinNames {
public:
    // FIXME: Remove the __attribute__((nodebug)) when <rdar://68246686> is fixed.
#if COMPILER(CLANG)
    __attribute__((nodebug))
#endif
    explicit WebCoreBuiltinNames(JSC::VM& vm)
        : m_vm(vm)
        WEBCORE_COMMON_PRIVATE_IDENTIFIERS_EACH_PROPERTY_NAME(INITIALIZE_BUILTIN_NAMES)
    {
#define EXPORT_NAME(name) m_vm.propertyNames->appendExternalName(name##PublicName(), name##PrivateName());
        WEBCORE_COMMON_PRIVATE_IDENTIFIERS_EACH_PROPERTY_NAME(EXPORT_NAME)
#undef EXPORT_NAME
    }

    WEBCORE_COMMON_PRIVATE_IDENTIFIERS_EACH_PROPERTY_NAME(DECLARE_BUILTIN_IDENTIFIER_ACCESSOR)

private:
    JSC::VM& m_vm;
    WEBCORE_COMMON_PRIVATE_IDENTIFIERS_EACH_PROPERTY_NAME(DECLARE_BUILTIN_NAMES)
};

} // namespace WebCore
