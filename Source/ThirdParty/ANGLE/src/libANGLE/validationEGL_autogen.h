// GENERATED FILE - DO NOT EDIT.
// Generated by generate_entry_points.py using data from egl.xml and egl_angle_ext.xml.
//
// Copyright 2020 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// validationEGL_autogen.h:
//   Validation functions for the EGL entry points.

#ifndef LIBANGLE_VALIDATION_EGL_AUTOGEN_H_
#define LIBANGLE_VALIDATION_EGL_AUTOGEN_H_

#include "libANGLE/validationEGL.h"

namespace egl
{

// EGL 1.0
bool ValidateChooseConfig(const ValidationContext *val,
                          const egl::Display *dpyPacked,
                          const AttributeMap &attrib_listPacked,
                          const EGLConfig *configs,
                          EGLint config_size,
                          const EGLint *num_config);
bool ValidateCopyBuffers(const ValidationContext *val,
                         const egl::Display *dpyPacked,
                         SurfaceID surfacePacked,
                         EGLNativePixmapType target);
bool ValidateCreateContext(const ValidationContext *val,
                           const egl::Display *dpyPacked,
                           const egl::Config *configPacked,
                           gl::ContextID share_contextPacked,
                           const AttributeMap &attrib_listPacked);
bool ValidateCreatePbufferSurface(const ValidationContext *val,
                                  const egl::Display *dpyPacked,
                                  const egl::Config *configPacked,
                                  const AttributeMap &attrib_listPacked);
bool ValidateCreatePixmapSurface(const ValidationContext *val,
                                 const egl::Display *dpyPacked,
                                 const egl::Config *configPacked,
                                 EGLNativePixmapType pixmap,
                                 const AttributeMap &attrib_listPacked);
bool ValidateCreateWindowSurface(const ValidationContext *val,
                                 const egl::Display *dpyPacked,
                                 const egl::Config *configPacked,
                                 EGLNativeWindowType win,
                                 const AttributeMap &attrib_listPacked);
bool ValidateDestroyContext(const ValidationContext *val,
                            const egl::Display *dpyPacked,
                            gl::ContextID ctxPacked);
bool ValidateDestroySurface(const ValidationContext *val,
                            const egl::Display *dpyPacked,
                            SurfaceID surfacePacked);
bool ValidateGetConfigAttrib(const ValidationContext *val,
                             const egl::Display *dpyPacked,
                             const egl::Config *configPacked,
                             EGLint attribute,
                             const EGLint *value);
bool ValidateGetConfigs(const ValidationContext *val,
                        const egl::Display *dpyPacked,
                        const EGLConfig *configs,
                        EGLint config_size,
                        const EGLint *num_config);
bool ValidateGetCurrentDisplay(const ValidationContext *val);
bool ValidateGetCurrentSurface(const ValidationContext *val, EGLint readdraw);
bool ValidateGetDisplay(const ValidationContext *val, EGLNativeDisplayType display_id);
bool ValidateGetError(const ValidationContext *val);
bool ValidateGetProcAddress(const ValidationContext *val, const char *procname);
bool ValidateInitialize(const ValidationContext *val,
                        const egl::Display *dpyPacked,
                        const EGLint *major,
                        const EGLint *minor);
bool ValidateMakeCurrent(const ValidationContext *val,
                         const egl::Display *dpyPacked,
                         SurfaceID drawPacked,
                         SurfaceID readPacked,
                         gl::ContextID ctxPacked);
bool ValidateQueryContext(const ValidationContext *val,
                          const egl::Display *dpyPacked,
                          gl::ContextID ctxPacked,
                          EGLint attribute,
                          const EGLint *value);
bool ValidateQueryString(const ValidationContext *val, const egl::Display *dpyPacked, EGLint name);
bool ValidateQuerySurface(const ValidationContext *val,
                          const egl::Display *dpyPacked,
                          SurfaceID surfacePacked,
                          EGLint attribute,
                          const EGLint *value);
bool ValidateSwapBuffers(const ValidationContext *val,
                         const egl::Display *dpyPacked,
                         SurfaceID surfacePacked);
bool ValidateTerminate(const ValidationContext *val, const egl::Display *dpyPacked);
bool ValidateWaitGL(const ValidationContext *val);
bool ValidateWaitNative(const ValidationContext *val, EGLint engine);

// EGL 1.1
bool ValidateBindTexImage(const ValidationContext *val,
                          const egl::Display *dpyPacked,
                          SurfaceID surfacePacked,
                          EGLint buffer);
bool ValidateReleaseTexImage(const ValidationContext *val,
                             const egl::Display *dpyPacked,
                             SurfaceID surfacePacked,
                             EGLint buffer);
bool ValidateSurfaceAttrib(const ValidationContext *val,
                           const egl::Display *dpyPacked,
                           SurfaceID surfacePacked,
                           EGLint attribute,
                           EGLint value);
bool ValidateSwapInterval(const ValidationContext *val,
                          const egl::Display *dpyPacked,
                          EGLint interval);

// EGL 1.2
bool ValidateBindAPI(const ValidationContext *val, EGLenum api);
bool ValidateCreatePbufferFromClientBuffer(const ValidationContext *val,
                                           const egl::Display *dpyPacked,
                                           EGLenum buftype,
                                           EGLClientBuffer buffer,
                                           const egl::Config *configPacked,
                                           const AttributeMap &attrib_listPacked);
bool ValidateQueryAPI(const ValidationContext *val);
bool ValidateReleaseThread(const ValidationContext *val);
bool ValidateWaitClient(const ValidationContext *val);

// EGL 1.4
bool ValidateGetCurrentContext(const ValidationContext *val);

// EGL 1.5
bool ValidateClientWaitSync(const ValidationContext *val,
                            const egl::Display *dpyPacked,
                            egl::SyncID syncPacked,
                            EGLint flags,
                            EGLTime timeout);
bool ValidateCreateImage(const ValidationContext *val,
                         const egl::Display *dpyPacked,
                         gl::ContextID ctxPacked,
                         EGLenum target,
                         EGLClientBuffer buffer,
                         const AttributeMap &attrib_listPacked);
bool ValidateCreatePlatformPixmapSurface(const ValidationContext *val,
                                         const egl::Display *dpyPacked,
                                         const egl::Config *configPacked,
                                         const void *native_pixmap,
                                         const AttributeMap &attrib_listPacked);
bool ValidateCreatePlatformWindowSurface(const ValidationContext *val,
                                         const egl::Display *dpyPacked,
                                         const egl::Config *configPacked,
                                         const void *native_window,
                                         const AttributeMap &attrib_listPacked);
bool ValidateCreateSync(const ValidationContext *val,
                        const egl::Display *dpyPacked,
                        EGLenum type,
                        const AttributeMap &attrib_listPacked);
bool ValidateDestroyImage(const ValidationContext *val,
                          const egl::Display *dpyPacked,
                          ImageID imagePacked);
bool ValidateDestroySync(const ValidationContext *val,
                         const egl::Display *dpyPacked,
                         egl::SyncID syncPacked);
bool ValidateGetPlatformDisplay(const ValidationContext *val,
                                EGLenum platform,
                                const void *native_display,
                                const AttributeMap &attrib_listPacked);
bool ValidateGetSyncAttrib(const ValidationContext *val,
                           const egl::Display *dpyPacked,
                           egl::SyncID syncPacked,
                           EGLint attribute,
                           const EGLAttrib *value);
bool ValidateWaitSync(const ValidationContext *val,
                      const egl::Display *dpyPacked,
                      egl::SyncID syncPacked,
                      EGLint flags);

// EGL_ANDROID_blob_cache
bool ValidateSetBlobCacheFuncsANDROID(const ValidationContext *val,
                                      const egl::Display *dpyPacked,
                                      EGLSetBlobFuncANDROID set,
                                      EGLGetBlobFuncANDROID get);

// EGL_ANDROID_create_native_client_buffer
bool ValidateCreateNativeClientBufferANDROID(const ValidationContext *val,
                                             const AttributeMap &attrib_listPacked);

// EGL_ANDROID_get_frame_timestamps
bool ValidateGetCompositorTimingSupportedANDROID(const ValidationContext *val,
                                                 const egl::Display *dpyPacked,
                                                 SurfaceID surfacePacked,
                                                 CompositorTiming namePacked);
bool ValidateGetCompositorTimingANDROID(const ValidationContext *val,
                                        const egl::Display *dpyPacked,
                                        SurfaceID surfacePacked,
                                        EGLint numTimestamps,
                                        const EGLint *names,
                                        const EGLnsecsANDROID *values);
bool ValidateGetNextFrameIdANDROID(const ValidationContext *val,
                                   const egl::Display *dpyPacked,
                                   SurfaceID surfacePacked,
                                   const EGLuint64KHR *frameId);
bool ValidateGetFrameTimestampSupportedANDROID(const ValidationContext *val,
                                               const egl::Display *dpyPacked,
                                               SurfaceID surfacePacked,
                                               Timestamp timestampPacked);
bool ValidateGetFrameTimestampsANDROID(const ValidationContext *val,
                                       const egl::Display *dpyPacked,
                                       SurfaceID surfacePacked,
                                       EGLuint64KHR frameId,
                                       EGLint numTimestamps,
                                       const EGLint *timestamps,
                                       const EGLnsecsANDROID *values);

// EGL_ANDROID_get_native_client_buffer
bool ValidateGetNativeClientBufferANDROID(const ValidationContext *val,
                                          const struct AHardwareBuffer *buffer);

// EGL_ANDROID_native_fence_sync
bool ValidateDupNativeFenceFDANDROID(const ValidationContext *val,
                                     const egl::Display *dpyPacked,
                                     egl::SyncID syncPacked);

// EGL_ANDROID_presentation_time
bool ValidatePresentationTimeANDROID(const ValidationContext *val,
                                     const egl::Display *dpyPacked,
                                     SurfaceID surfacePacked,
                                     EGLnsecsANDROID time);

// EGL_ANGLE_device_creation
bool ValidateCreateDeviceANGLE(const ValidationContext *val,
                               EGLint device_type,
                               const void *native_device,
                               const EGLAttrib *attrib_list);
bool ValidateReleaseDeviceANGLE(const ValidationContext *val, const egl::Device *devicePacked);

// EGL_ANGLE_feature_control
bool ValidateQueryStringiANGLE(const ValidationContext *val,
                               const egl::Display *dpyPacked,
                               EGLint name,
                               EGLint index);
bool ValidateQueryDisplayAttribANGLE(const ValidationContext *val,
                                     const egl::Display *dpyPacked,
                                     EGLint attribute,
                                     const EGLAttrib *value);

// EGL_ANGLE_metal_shared_event_sync
bool ValidateCopyMetalSharedEventANGLE(const ValidationContext *val,
                                       const egl::Display *dpyPacked,
                                       egl::SyncID syncPacked);

// EGL_ANGLE_power_preference
bool ValidateReleaseHighPowerGPUANGLE(const ValidationContext *val,
                                      const egl::Display *dpyPacked,
                                      gl::ContextID ctxPacked);
bool ValidateReacquireHighPowerGPUANGLE(const ValidationContext *val,
                                        const egl::Display *dpyPacked,
                                        gl::ContextID ctxPacked);
bool ValidateHandleGPUSwitchANGLE(const ValidationContext *val, const egl::Display *dpyPacked);
bool ValidateForceGPUSwitchANGLE(const ValidationContext *val,
                                 const egl::Display *dpyPacked,
                                 EGLint gpuIDHigh,
                                 EGLint gpuIDLow);

// EGL_ANGLE_prepare_swap_buffers
bool ValidatePrepareSwapBuffersANGLE(const ValidationContext *val,
                                     const egl::Display *dpyPacked,
                                     SurfaceID surfacePacked);

// EGL_ANGLE_program_cache_control
bool ValidateProgramCacheGetAttribANGLE(const ValidationContext *val,
                                        const egl::Display *dpyPacked,
                                        EGLenum attrib);
bool ValidateProgramCacheQueryANGLE(const ValidationContext *val,
                                    const egl::Display *dpyPacked,
                                    EGLint index,
                                    const void *key,
                                    const EGLint *keysize,
                                    const void *binary,
                                    const EGLint *binarysize);
bool ValidateProgramCachePopulateANGLE(const ValidationContext *val,
                                       const egl::Display *dpyPacked,
                                       const void *key,
                                       EGLint keysize,
                                       const void *binary,
                                       EGLint binarysize);
bool ValidateProgramCacheResizeANGLE(const ValidationContext *val,
                                     const egl::Display *dpyPacked,
                                     EGLint limit,
                                     EGLint mode);

// EGL_ANGLE_query_surface_pointer
bool ValidateQuerySurfacePointerANGLE(const ValidationContext *val,
                                      const egl::Display *dpyPacked,
                                      SurfaceID surfacePacked,
                                      EGLint attribute,
                                      void *const *value);

// EGL_ANGLE_stream_producer_d3d_texture
bool ValidateCreateStreamProducerD3DTextureANGLE(const ValidationContext *val,
                                                 const egl::Display *dpyPacked,
                                                 const egl::Stream *streamPacked,
                                                 const AttributeMap &attrib_listPacked);
bool ValidateStreamPostD3DTextureANGLE(const ValidationContext *val,
                                       const egl::Display *dpyPacked,
                                       const egl::Stream *streamPacked,
                                       const void *texture,
                                       const AttributeMap &attrib_listPacked);

// EGL_ANGLE_swap_with_frame_token
bool ValidateSwapBuffersWithFrameTokenANGLE(const ValidationContext *val,
                                            const egl::Display *dpyPacked,
                                            SurfaceID surfacePacked,
                                            EGLFrameTokenANGLE frametoken);

// EGL_ANGLE_sync_control_rate
bool ValidateGetMscRateANGLE(const ValidationContext *val,
                             const egl::Display *dpyPacked,
                             SurfaceID surfacePacked,
                             const EGLint *numerator,
                             const EGLint *denominator);

// EGL_ANGLE_vulkan_image
bool ValidateExportVkImageANGLE(const ValidationContext *val,
                                const egl::Display *dpyPacked,
                                ImageID imagePacked,
                                const void *vk_image,
                                const void *vk_image_create_info);

// EGL_ANGLE_wait_until_work_scheduled
bool ValidateWaitUntilWorkScheduledANGLE(const ValidationContext *val,
                                         const egl::Display *dpyPacked);

// EGL_CHROMIUM_sync_control
bool ValidateGetSyncValuesCHROMIUM(const ValidationContext *val,
                                   const egl::Display *dpyPacked,
                                   SurfaceID surfacePacked,
                                   const EGLuint64KHR *ust,
                                   const EGLuint64KHR *msc,
                                   const EGLuint64KHR *sbc);

// EGL_EXT_device_query
bool ValidateQueryDeviceAttribEXT(const ValidationContext *val,
                                  const egl::Device *devicePacked,
                                  EGLint attribute,
                                  const EGLAttrib *value);
bool ValidateQueryDeviceStringEXT(const ValidationContext *val,
                                  const egl::Device *devicePacked,
                                  EGLint name);
bool ValidateQueryDisplayAttribEXT(const ValidationContext *val,
                                   const egl::Display *dpyPacked,
                                   EGLint attribute,
                                   const EGLAttrib *value);

// EGL_EXT_image_dma_buf_import_modifiers
bool ValidateQueryDmaBufFormatsEXT(const ValidationContext *val,
                                   const egl::Display *dpyPacked,
                                   EGLint max_formats,
                                   const EGLint *formats,
                                   const EGLint *num_formats);
bool ValidateQueryDmaBufModifiersEXT(const ValidationContext *val,
                                     const egl::Display *dpyPacked,
                                     EGLint format,
                                     EGLint max_modifiers,
                                     const EGLuint64KHR *modifiers,
                                     const EGLBoolean *external_only,
                                     const EGLint *num_modifiers);

// EGL_EXT_platform_base
bool ValidateCreatePlatformPixmapSurfaceEXT(const ValidationContext *val,
                                            const egl::Display *dpyPacked,
                                            const egl::Config *configPacked,
                                            const void *native_pixmap,
                                            const AttributeMap &attrib_listPacked);
bool ValidateCreatePlatformWindowSurfaceEXT(const ValidationContext *val,
                                            const egl::Display *dpyPacked,
                                            const egl::Config *configPacked,
                                            const void *native_window,
                                            const AttributeMap &attrib_listPacked);
bool ValidateGetPlatformDisplayEXT(const ValidationContext *val,
                                   EGLenum platform,
                                   const void *native_display,
                                   const AttributeMap &attrib_listPacked);

// EGL_KHR_debug
bool ValidateDebugMessageControlKHR(const ValidationContext *val,
                                    EGLDEBUGPROCKHR callback,
                                    const AttributeMap &attrib_listPacked);
bool ValidateLabelObjectKHR(const ValidationContext *val,
                            const egl::Display *displayPacked,
                            ObjectType objectTypePacked,
                            EGLObjectKHR object,
                            EGLLabelKHR label);
bool ValidateQueryDebugKHR(const ValidationContext *val, EGLint attribute, const EGLAttrib *value);

// EGL_KHR_fence_sync
bool ValidateClientWaitSyncKHR(const ValidationContext *val,
                               const egl::Display *dpyPacked,
                               egl::SyncID syncPacked,
                               EGLint flags,
                               EGLTimeKHR timeout);
bool ValidateCreateSyncKHR(const ValidationContext *val,
                           const egl::Display *dpyPacked,
                           EGLenum type,
                           const AttributeMap &attrib_listPacked);
bool ValidateDestroySyncKHR(const ValidationContext *val,
                            const egl::Display *dpyPacked,
                            egl::SyncID syncPacked);
bool ValidateGetSyncAttribKHR(const ValidationContext *val,
                              const egl::Display *dpyPacked,
                              egl::SyncID syncPacked,
                              EGLint attribute,
                              const EGLint *value);

// EGL_KHR_image
bool ValidateCreateImageKHR(const ValidationContext *val,
                            const egl::Display *dpyPacked,
                            gl::ContextID ctxPacked,
                            EGLenum target,
                            EGLClientBuffer buffer,
                            const AttributeMap &attrib_listPacked);
bool ValidateDestroyImageKHR(const ValidationContext *val,
                             const egl::Display *dpyPacked,
                             ImageID imagePacked);

// EGL_KHR_lock_surface3
bool ValidateLockSurfaceKHR(const ValidationContext *val,
                            const egl::Display *dpyPacked,
                            SurfaceID surfacePacked,
                            const AttributeMap &attrib_listPacked);
bool ValidateQuerySurface64KHR(const ValidationContext *val,
                               const egl::Display *dpyPacked,
                               SurfaceID surfacePacked,
                               EGLint attribute,
                               const EGLAttribKHR *value);
bool ValidateUnlockSurfaceKHR(const ValidationContext *val,
                              const egl::Display *dpyPacked,
                              SurfaceID surfacePacked);

// EGL_KHR_partial_update
bool ValidateSetDamageRegionKHR(const ValidationContext *val,
                                const egl::Display *dpyPacked,
                                SurfaceID surfacePacked,
                                const EGLint *rects,
                                EGLint n_rects);

// EGL_KHR_reusable_sync
bool ValidateSignalSyncKHR(const ValidationContext *val,
                           const egl::Display *dpyPacked,
                           egl::SyncID syncPacked,
                           EGLenum mode);

// EGL_KHR_stream
bool ValidateCreateStreamKHR(const ValidationContext *val,
                             const egl::Display *dpyPacked,
                             const AttributeMap &attrib_listPacked);
bool ValidateDestroyStreamKHR(const ValidationContext *val,
                              const egl::Display *dpyPacked,
                              const egl::Stream *streamPacked);
bool ValidateQueryStreamKHR(const ValidationContext *val,
                            const egl::Display *dpyPacked,
                            const egl::Stream *streamPacked,
                            EGLenum attribute,
                            const EGLint *value);
bool ValidateQueryStreamu64KHR(const ValidationContext *val,
                               const egl::Display *dpyPacked,
                               const egl::Stream *streamPacked,
                               EGLenum attribute,
                               const EGLuint64KHR *value);
bool ValidateStreamAttribKHR(const ValidationContext *val,
                             const egl::Display *dpyPacked,
                             const egl::Stream *streamPacked,
                             EGLenum attribute,
                             EGLint value);

// EGL_KHR_stream_consumer_gltexture
bool ValidateStreamConsumerAcquireKHR(const ValidationContext *val,
                                      const egl::Display *dpyPacked,
                                      const egl::Stream *streamPacked);
bool ValidateStreamConsumerGLTextureExternalKHR(const ValidationContext *val,
                                                const egl::Display *dpyPacked,
                                                const egl::Stream *streamPacked);
bool ValidateStreamConsumerReleaseKHR(const ValidationContext *val,
                                      const egl::Display *dpyPacked,
                                      const egl::Stream *streamPacked);

// EGL_KHR_swap_buffers_with_damage
bool ValidateSwapBuffersWithDamageKHR(const ValidationContext *val,
                                      const egl::Display *dpyPacked,
                                      SurfaceID surfacePacked,
                                      const EGLint *rects,
                                      EGLint n_rects);

// EGL_KHR_wait_sync
bool ValidateWaitSyncKHR(const ValidationContext *val,
                         const egl::Display *dpyPacked,
                         egl::SyncID syncPacked,
                         EGLint flags);

// EGL_NV_post_sub_buffer
bool ValidatePostSubBufferNV(const ValidationContext *val,
                             const egl::Display *dpyPacked,
                             SurfaceID surfacePacked,
                             EGLint x,
                             EGLint y,
                             EGLint width,
                             EGLint height);

// EGL_NV_stream_consumer_gltexture_yuv
bool ValidateStreamConsumerGLTextureExternalAttribsNV(const ValidationContext *val,
                                                      const egl::Display *dpyPacked,
                                                      const egl::Stream *streamPacked,
                                                      const AttributeMap &attrib_listPacked);
}  // namespace egl

#endif  // LIBANGLE_VALIDATION_EGL_AUTOGEN_H_
