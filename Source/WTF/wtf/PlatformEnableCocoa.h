/*
 * Copyright (C) 2006-2023 Apple Inc. All rights reserved.
 * Copyright (C) 2007-2009 Torch Mobile, Inc.
 * Copyright (C) 2010, 2011 Research In Motion Limited. All rights reserved.
 * Copyright (C) 2013 Samsung Electronics. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#ifndef WTF_PLATFORM_GUARD_AGAINST_INDIRECT_INCLUSION
#error "Please #include <wtf/Platform.h> instead of this file directly."
#endif

#if !PLATFORM(COCOA)
#error "This file should only be included when building for one of Apple's Cocoa platforms."
#endif

/* Please keep the following in alphabetical order so we can notice duplicates. */
/* Items should only be here if they are different from the defaults in PlatformEnable.h. */

#if !defined(ENABLE_3D_TRANSFORMS)
#define ENABLE_3D_TRANSFORMS 1
#endif

#if !defined(ENABLE_ACCESSIBILITY_ISOLATED_TREE) && PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 130000
#define ENABLE_ACCESSIBILITY_ISOLATED_TREE 1
#endif

#if !defined(ENABLE_AIRPLAY_PICKER) && !PLATFORM(MAC) && !PLATFORM(MACCATALYST) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define ENABLE_AIRPLAY_PICKER 1
#endif

#if !defined(ENABLE_ALTERNATE_WEBM_PLAYER) && !PLATFORM(MACCATALYST) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define ENABLE_ALTERNATE_WEBM_PLAYER 1
#endif

#if !defined(ENABLE_ANIMATED_KEYBOARD_SCROLLING) && PLATFORM(IOS_FAMILY)
#define ENABLE_ANIMATED_KEYBOARD_SCROLLING 1
#endif

#if !defined(ENABLE_APPLE_PAY) && !PLATFORM(MACCATALYST) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define ENABLE_APPLE_PAY 1
#endif

#if !defined(ENABLE_APPLE_PAY_AUTOMATIC_RELOAD_LINE_ITEM) && HAVE(PASSKIT_AUTOMATIC_RELOAD_SUMMARY_ITEM)
#define ENABLE_APPLE_PAY_AUTOMATIC_RELOAD_LINE_ITEM 1
#endif

#if !defined(ENABLE_APPLE_PAY_AUTOMATIC_RELOAD_PAYMENTS) && HAVE(PASSKIT_AUTOMATIC_RELOAD_PAYMENTS)
#define ENABLE_APPLE_PAY_AUTOMATIC_RELOAD_PAYMENTS 1
#endif

#if !defined(ENABLE_APPLE_PAY_COUPON_CODE) && HAVE(PASSKIT_COUPON_CODE)
#define ENABLE_APPLE_PAY_COUPON_CODE 1
#endif

#if !defined(ENABLE_APPLE_PAY_DEFERRED_LINE_ITEM) && HAVE(PASSKIT_DEFERRED_SUMMARY_ITEM)
#define ENABLE_APPLE_PAY_DEFERRED_LINE_ITEM 1
#endif

#if !defined(ENABLE_APPLE_PAY_FEATURES) && (PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 130000)
#define ENABLE_APPLE_PAY_FEATURES 1
#endif

#if !defined(ENABLE_APPLE_PAY_INSTALLMENTS) && HAVE(PASSKIT_INSTALLMENTS)
#define ENABLE_APPLE_PAY_INSTALLMENTS 1
#endif

#if !defined(ENABLE_APPLE_PAY_LATER) && (PLATFORM(IOS)  || PLATFORM(VISION))
#define ENABLE_APPLE_PAY_LATER 1
#endif

#if !defined(ENABLE_APPLE_PAY_LATER_AVAILABILITY) && HAVE(PASSKIT_APPLE_PAY_LATER_AVAILABILITY)
#define ENABLE_APPLE_PAY_LATER_AVAILABILITY 1
#endif

#if !defined(ENABLE_APPLE_PAY_MULTI_MERCHANT_PAYMENTS) && HAVE(PASSKIT_MULTI_MERCHANT_PAYMENTS)
#define ENABLE_APPLE_PAY_MULTI_MERCHANT_PAYMENTS 1
#endif

#if !defined(ENABLE_APPLE_PAY_NEW_BUTTON_TYPES) && HAVE(PASSKIT_NEW_BUTTON_TYPES)
#define ENABLE_APPLE_PAY_NEW_BUTTON_TYPES 1
#endif

#if !defined(ENABLE_APPLE_PAY_PAYMENT_ORDER_DETAILS) && HAVE(PASSKIT_PAYMENT_ORDER_DETAILS)
#define ENABLE_APPLE_PAY_PAYMENT_ORDER_DETAILS 1
#endif

#if !defined(ENABLE_APPLE_PAY_SELECTED_SHIPPING_METHOD) && HAVE(PASSKIT_DEFAULT_SHIPPING_METHOD)
#define ENABLE_APPLE_PAY_SELECTED_SHIPPING_METHOD 1
#endif

#if !defined(ENABLE_APPLE_PAY_SHIPPING_CONTACT_EDITING_MODE) && HAVE(PASSKIT_SHIPPING_CONTACT_EDITING_MODE)
#define ENABLE_APPLE_PAY_SHIPPING_CONTACT_EDITING_MODE 1
#endif

#if !defined(ENABLE_APPLE_PAY_RECURRING_LINE_ITEM) && HAVE(PASSKIT_RECURRING_SUMMARY_ITEM)
#define ENABLE_APPLE_PAY_RECURRING_LINE_ITEM 1
#endif

#if !defined(ENABLE_APPLE_PAY_RECURRING_PAYMENTS) && HAVE(PASSKIT_RECURRING_PAYMENTS)
#define ENABLE_APPLE_PAY_RECURRING_PAYMENTS 1
#endif

#if !defined(ENABLE_APPLE_PAY_DEFERRED_PAYMENTS) && HAVE(PASSKIT_DEFERRED_PAYMENTS)
#define ENABLE_APPLE_PAY_DEFERRED_PAYMENTS 1
#endif

#if !PLATFORM(MAC) && !PLATFORM(MACCATALYST) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)

#if !defined(ENABLE_APPLE_PAY_REMOTE_UI)
#define ENABLE_APPLE_PAY_REMOTE_UI 1
#endif

#if !defined(ENABLE_APPLE_PAY_REMOTE_UI_USES_SCENE) && PLATFORM(VISION)
#define ENABLE_APPLE_PAY_REMOTE_UI_USES_SCENE 1
#endif

#endif // !PLATFORM(MAC) && !PLATFORM(MACCATALYST) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)

#if !defined(ENABLE_APPLE_PAY_SESSION_V11) && (PLATFORM(MAC) || PLATFORM(IOS) || PLATFORM(VISION))
#define ENABLE_APPLE_PAY_SESSION_V11 1
#endif

#if !defined(ENABLE_APPLE_PAY_SHIPPING_METHOD_DATE_COMPONENTS_RANGE) && HAVE(PASSKIT_SHIPPING_METHOD_DATE_COMPONENTS_RANGE)
#define ENABLE_APPLE_PAY_SHIPPING_METHOD_DATE_COMPONENTS_RANGE 1
#endif

#if !defined(ENABLE_APPLE_PAY_UPDATE_SHIPPING_METHODS_WHEN_CHANGING_LINE_ITEMS) && HAVE(PASSKIT_UPDATE_SHIPPING_METHODS_WHEN_CHANGING_SUMMARY_ITEMS)
#define ENABLE_APPLE_PAY_UPDATE_SHIPPING_METHODS_WHEN_CHANGING_LINE_ITEMS 1
#endif

#if !defined(ENABLE_APPLE_PAY_AMS_UI) && (PLATFORM(MAC) || PLATFORM(IOS) || PLATFORM(VISION))
#define ENABLE_APPLE_PAY_AMS_UI 1
#endif

#if !defined(ENABLE_APPLICATION_MANIFEST)
#define ENABLE_APPLICATION_MANIFEST 1
#endif

#if !defined(ENABLE_APP_BOUND_DOMAINS) && PLATFORM(IOS_FAMILY)
#define ENABLE_APP_BOUND_DOMAINS 1
#endif

#if !defined(ENABLE_MANAGED_DOMAINS)
#define ENABLE_MANAGED_DOMAINS 1
#endif

#if !defined(ENABLE_ASYNC_SCROLLING)
#define ENABLE_ASYNC_SCROLLING 1
#endif

#if !defined(ENABLE_ATTACHMENT_ELEMENT)
#define ENABLE_ATTACHMENT_ELEMENT 1
#endif

#if !defined(ENABLE_AUTOCAPITALIZE) && !PLATFORM(MAC)
#define ENABLE_AUTOCAPITALIZE 1
#endif

#if !defined(ENABLE_AUTOCORRECT)
#define ENABLE_AUTOCORRECT 1
#endif

#if !defined(ENABLE_AVF_CAPTIONS)
#define ENABLE_AVF_CAPTIONS 1
#endif

#if !defined(ENABLE_BUILT_IN_NOTIFICATIONS) && (PLATFORM(MAC) || PLATFORM(IOS) || PLATFORM(VISION))
#define ENABLE_BUILT_IN_NOTIFICATIONS 1
#endif

#if !defined(ENABLE_INSTALL_COORDINATION_BUNDLES) && (PLATFORM(IOS) || PLATFORM(VISION))
#define ENABLE_INSTALL_COORDINATION_BUNDLES 1
#endif

#if !defined(ENABLE_CACHE_PARTITIONING)
#define ENABLE_CACHE_PARTITIONING 1
#endif

#if !defined(ENABLE_CFPREFS_DIRECT_MODE)
#define ENABLE_CFPREFS_DIRECT_MODE 1
#endif

#if !defined(ENABLE_CONTENT_CHANGE_OBSERVER) && PLATFORM(IOS_FAMILY)
#define ENABLE_CONTENT_CHANGE_OBSERVER 1
#endif

#if !defined(ENABLE_CONTENT_EXTENSIONS)
#define ENABLE_CONTENT_EXTENSIONS 1
#endif

// FIXME: Why not APPLETV?
#if !defined(ENABLE_CONTENT_FILTERING) && !PLATFORM(APPLETV)
#define ENABLE_CONTENT_FILTERING 1
#endif

#if defined(ENABLE_CONTENT_FILTERING) && ENABLE_CONTENT_FILTERING
#define ENABLE_CONTENT_FILTERING_IN_NETWORKING_PROCESS 1
#endif

#if !defined(ENABLE_CONTEXT_MENUS) && PLATFORM(IOS_FAMILY)
#define ENABLE_CONTEXT_MENUS 0
#endif

#if !defined(ENABLE_CONTEXT_MENU_EVENT) && (PLATFORM(WATCHOS) || PLATFORM(APPLETV))
#define ENABLE_CONTEXT_MENU_EVENT 0
#endif

#if !defined(ENABLE_CONTEXT_MENU_QR_CODE_DETECTION) && PLATFORM(MAC)
#define ENABLE_CONTEXT_MENU_QR_CODE_DETECTION 1
#endif

#if !defined(ENABLE_CSS_COMPOSITING)
#define ENABLE_CSS_COMPOSITING 1
#endif

#if !defined(ENABLE_CSS_CONIC_GRADIENTS)
#define ENABLE_CSS_CONIC_GRADIENTS 1
#endif

#if !defined(ENABLE_CSS_PAINTING_API)
#define ENABLE_CSS_PAINTING_API 1
#endif

#if !defined(ENABLE_CURSOR_VISIBILITY)
#define ENABLE_CURSOR_VISIBILITY 1
#endif

#if !defined(ENABLE_CUSTOM_CURSOR_SUPPORT) && PLATFORM(IOS_FAMILY)
#define ENABLE_CUSTOM_CURSOR_SUPPORT 0
#endif

#if !defined(ENABLE_DARK_MODE_CSS) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define ENABLE_DARK_MODE_CSS 1
#endif

#if !defined(ENABLE_DATACUE_VALUE)
#define ENABLE_DATACUE_VALUE 1
#endif

#if !defined(ENABLE_DATALIST_ELEMENT)
#define ENABLE_DATALIST_ELEMENT 1
#endif

#if !defined(ENABLE_DATA_DETECTION) && (PLATFORM(MAC) || PLATFORM(IOS) || PLATFORM(MACCATALYST) || PLATFORM(VISION))
#define ENABLE_DATA_DETECTION 1
#endif

#if !defined(ENABLE_DEVICE_ORIENTATION)
#define ENABLE_DEVICE_ORIENTATION 1
#endif

#if !defined(ENABLE_DRAG_SUPPORT) && (PLATFORM(WATCHOS) || PLATFORM(APPLETV))
#define ENABLE_DRAG_SUPPORT 0
#endif

#if !defined(ENABLE_EDITABLE_REGION) && (PLATFORM(IOS) || PLATFORM(VISION))
#define ENABLE_EDITABLE_REGION 1
#endif

#if !defined(ENABLE_ENCRYPTED_MEDIA) && !PLATFORM(MACCATALYST)
#define ENABLE_ENCRYPTED_MEDIA 1
#endif

#if !defined(ENABLE_FILE_REPLACEMENT)
#define ENABLE_FILE_REPLACEMENT 1
#endif

#if !defined(ENABLE_FILESYSTEM_POSIX_FAST_PATH)
#define ENABLE_FILESYSTEM_POSIX_FAST_PATH 1
#endif

#if !defined(ENABLE_FILTERS_LEVEL_2)
#define ENABLE_FILTERS_LEVEL_2 1
#endif

#if !defined(ENABLE_FTL_JIT) && !USE(JSVALUE32_64)
#define ENABLE_FTL_JIT 1
#endif

#if !defined(ENABLE_FULLSCREEN_API) && !PLATFORM(MACCATALYST) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define ENABLE_FULLSCREEN_API 1
#endif

#if !defined(ENABLE_FULL_KEYBOARD_ACCESS) && !PLATFORM(MACCATALYST) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define ENABLE_FULL_KEYBOARD_ACCESS 1
#endif

#if !defined(ENABLE_GAMEPAD) && !PLATFORM(WATCHOS)
#define ENABLE_GAMEPAD 1
#endif

#if !defined(ENABLE_GEOLOCATION)
#define ENABLE_GEOLOCATION 1
#endif

#if !defined(ENABLE_GPU_PROCESS)
#define ENABLE_GPU_PROCESS 1
#endif

#if !defined(ENABLE_GPU_PROCESS_BY_DEFAULT)
#define ENABLE_GPU_PROCESS_BY_DEFAULT 1
#endif

#if !defined(ENABLE_GPU_DRIVER_PREWARMING) && PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED < 140000
#define ENABLE_GPU_DRIVER_PREWARMING 1
#endif

#if !defined(ENABLE_HARDWARE_JPEG) && !(PLATFORM(MAC) && CPU(X86_64))
#define ENABLE_HARDWARE_JPEG 1
#endif

#if !defined(ENABLE_INPUT_TYPE_DATE)
#define ENABLE_INPUT_TYPE_DATE 1
#endif

#if !defined(ENABLE_INPUT_TYPE_DATETIMELOCAL)
#define ENABLE_INPUT_TYPE_DATETIMELOCAL 1
#endif

#if !defined(ENABLE_INPUT_TYPE_MONTH)
#define ENABLE_INPUT_TYPE_MONTH 1
#endif

#if !defined(ENABLE_INPUT_TYPE_TIME)
#define ENABLE_INPUT_TYPE_TIME 1
#endif

#if !defined(ENABLE_INPUT_TYPE_WEEK)
#define ENABLE_INPUT_TYPE_WEEK 1
#endif

#if !defined(ENABLE_INSPECTOR_ALTERNATE_DISPATCHERS)
#define ENABLE_INSPECTOR_ALTERNATE_DISPATCHERS 1
#endif

#if !defined(ENABLE_INSPECTOR_EXTENSIONS) && PLATFORM(MAC)
#define ENABLE_INSPECTOR_EXTENSIONS 1
#endif

#if !defined(ENABLE_INSPECTOR_NETWORK_THROTTLING)
#define ENABLE_INSPECTOR_NETWORK_THROTTLING 1
#endif

#if !defined(ENABLE_INSPECTOR_TELEMETRY)
#define ENABLE_INSPECTOR_TELEMETRY 1
#endif

#if !defined(ENABLE_IOS_FORM_CONTROL_REFRESH) && PLATFORM(IOS_FAMILY)
#define ENABLE_IOS_FORM_CONTROL_REFRESH 1
#endif

#if !defined(ENABLE_IOS_GESTURE_EVENTS) && PLATFORM(IOS_FAMILY) && USE(APPLE_INTERNAL_SDK)
#define ENABLE_IOS_GESTURE_EVENTS 1
#endif

#if !defined(ENABLE_IOS_TOUCH_EVENTS) && PLATFORM(IOS_FAMILY) && USE(APPLE_INTERNAL_SDK)
#define ENABLE_IOS_TOUCH_EVENTS 1
#endif

#if !defined(ENABLE_LAYER_BASED_SVG_ENGINE)
#define ENABLE_LAYER_BASED_SVG_ENGINE 1
#endif

#if !defined(ENABLE_LEGACY_CUSTOM_PROTOCOL_MANAGER)
#define ENABLE_LEGACY_CUSTOM_PROTOCOL_MANAGER 1
#endif

#if !defined(ENABLE_LEGACY_ENCRYPTED_MEDIA) && !PLATFORM(MACCATALYST)
#define ENABLE_LEGACY_ENCRYPTED_MEDIA 1
#endif

#if !defined(ENABLE_MAC_GESTURE_EVENTS) && PLATFORM(MAC) && USE(APPLE_INTERNAL_SDK)
#define ENABLE_MAC_GESTURE_EVENTS 1
#endif

#if !defined(ENABLE_MEDIA_CAPTURE) && !PLATFORM(MAC) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define ENABLE_MEDIA_CAPTURE 1
#endif

#if !defined(ENABLE_MEDIA_CONTROLS_SCRIPT)
#define ENABLE_MEDIA_CONTROLS_SCRIPT 1
#endif

#if !defined(ENABLE_MEDIA_CONTROLS_CONTEXT_MENUS) && (PLATFORM(MAC) || PLATFORM(IOS) || PLATFORM(MACCATALYST) || PLATFORM(VISION))
#define ENABLE_MEDIA_CONTROLS_CONTEXT_MENUS 1
#endif

#if !defined(ENABLE_MEDIA_SESSION_COORDINATOR) && PLATFORM(MAC)
#define ENABLE_MEDIA_SESSION_COORDINATOR 1
#endif

#if !defined(ENABLE_MEDIA_SESSION_PLAYLIST) && PLATFORM(MAC)
#define ENABLE_MEDIA_SESSION_PLAYLIST 1
#endif

#if !defined(ENABLE_MEDIA_SESSION) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define ENABLE_MEDIA_SESSION 1
#endif

#if !defined(ENABLE_DOM_AUDIO_SESSION) && PLATFORM(COCOA) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define ENABLE_DOM_AUDIO_SESSION 1
#endif

#if !defined(ENABLE_MEDIA_SOURCE) && !PLATFORM(MACCATALYST) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define ENABLE_MEDIA_SOURCE 1
#endif

#if !defined(MANAGED_MEDIA_SOURCE) && ENABLE(MEDIA_SOURCE)
#define ENABLE_MANAGED_MEDIA_SOURCE 1
#endif

#if !defined(ENABLE_MEDIA_STREAM) && !PLATFORM(MACCATALYST) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define ENABLE_MEDIA_STREAM 1
#endif

#if !defined(ENABLE_MEDIA_RECORDER) && ENABLE(MEDIA_STREAM) && !PLATFORM(MACCATALYST) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define ENABLE_MEDIA_RECORDER 1
#endif

#if !defined(ENABLE_MEDIA_USAGE) && (PLATFORM(MAC) || PLATFORM(MACCATALYST))
#define ENABLE_MEDIA_USAGE 1
#endif

#if !defined(ENABLE_MEMORY_SAMPLER)
#define ENABLE_MEMORY_SAMPLER 1
#endif

#if !defined(ENABLE_META_VIEWPORT) && PLATFORM(IOS_FAMILY)
#define ENABLE_META_VIEWPORT 1
#endif

#if !defined(ENABLE_MODEL_ELEMENT)
#define ENABLE_MODEL_ELEMENT 1
#endif

#if !defined(ENABLE_MODERN_MEDIA_CONTROLS)
#define ENABLE_MODERN_MEDIA_CONTROLS 1
#endif

// FIXME: We almost certainly do not need this monospace font exception for watchOS and tvOS.
#if !defined(ENABLE_MONOSPACE_FONT_EXCEPTION) && (PLATFORM(WATCHOS) || PLATFORM(APPLETV))
#define ENABLE_MONOSPACE_FONT_EXCEPTION 1
#endif

#if !defined(ENABLE_MOUSE_CURSOR_SCALE) && PLATFORM(MAC)
#define ENABLE_MOUSE_CURSOR_SCALE 1
#endif

#if !defined(ENABLE_NAVIGATOR_STANDALONE)
#define ENABLE_NAVIGATOR_STANDALONE 1
#endif

#if !defined(ENABLE_NETWORK_CACHE_SPECULATIVE_REVALIDATION)
#define ENABLE_NETWORK_CACHE_SPECULATIVE_REVALIDATION 1
#endif

#if !defined(ENABLE_NETWORK_CACHE_STALE_WHILE_REVALIDATE)
#define ENABLE_NETWORK_CACHE_STALE_WHILE_REVALIDATE 1
#endif

#if !defined(ENABLE_NON_VISIBLE_WEBPROCESS_MEMORY_CLEANUP_TIMER) && (PLATFORM(MAC) || PLATFORM(MACCATALYST))
#define ENABLE_NON_VISIBLE_WEBPROCESS_MEMORY_CLEANUP_TIMER 1
#endif

#if !defined(ENABLE_NOTIFICATIONS)
#define ENABLE_NOTIFICATIONS 1
#endif

#if !defined(ENABLE_OFFSCREEN_CANVAS) && (PLATFORM(MAC) || PLATFORM(IOS_FAMILY))
#define ENABLE_OFFSCREEN_CANVAS 1
#endif

// ORIENTATION_EVENTS should never get enabled on Desktop, only Mobile.
#if !defined(ENABLE_ORIENTATION_EVENTS) && PLATFORM(IOS_FAMILY)
#define ENABLE_ORIENTATION_EVENTS 1
#endif

#if !defined(ENABLE_OVERFLOW_SCROLLING_TOUCH) && PLATFORM(IOS_FAMILY)
#define ENABLE_OVERFLOW_SCROLLING_TOUCH 1
#endif

#if !defined(ENABLE_PAYMENT_REQUEST)
#define ENABLE_PAYMENT_REQUEST 1
#endif

#if !defined(ENABLE_PAYMENT_REQUEST_SELECTED_SHIPPING_OPTION) && HAVE(PASSKIT_DEFAULT_SHIPPING_METHOD)
#define ENABLE_PAYMENT_REQUEST_SELECTED_SHIPPING_OPTION 1
#endif

#if !defined(ENABLE_PICTURE_IN_PICTURE_API) && !PLATFORM(MACCATALYST) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define ENABLE_PICTURE_IN_PICTURE_API 1
#endif

#if !defined(ENABLE_PDFKIT_PLUGIN) && PLATFORM(MAC)
#define ENABLE_PDFKIT_PLUGIN 1
#endif

#if !defined(ENABLE_PERIODIC_MEMORY_MONITOR) && PLATFORM(MAC)
#define ENABLE_PERIODIC_MEMORY_MONITOR 1
#endif

#if !defined(ENABLE_PLATFORM_DRIVEN_TEXT_CHECKING) && PLATFORM(MACCATALYST)
#define ENABLE_PLATFORM_DRIVEN_TEXT_CHECKING 1
#endif

#if !defined(ENABLE_POINTER_LOCK) && PLATFORM(IOS_FAMILY)
#define ENABLE_POINTER_LOCK 0
#endif

#if !defined(ENABLE_PENCIL_HOVER) && PLATFORM(IOS_FAMILY)
#define ENABLE_PENCIL_HOVER 1
#endif


#if !defined(ENABLE_POST_EDITING_GRAMMAR_CHECKING) && ((PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 130000) || PLATFORM(IOS) || PLATFORM(VISION))
#define ENABLE_POST_EDITING_GRAMMAR_CHECKING 1
#endif

#if !defined(ENABLE_PREVIEW_CONVERTER) && (PLATFORM(IOS) || PLATFORM(VISION))
#define ENABLE_PREVIEW_CONVERTER 1
#endif

#if !defined(ENABLE_PRIMARY_SNAPSHOTTED_PLUGIN_HEURISTIC)
#define ENABLE_PRIMARY_SNAPSHOTTED_PLUGIN_HEURISTIC 1
#endif

#if !defined(ENABLE_PUBLIC_SUFFIX_LIST)
#define ENABLE_PUBLIC_SUFFIX_LIST 1
#endif

#if !defined(ENABLE_REMOTE_INSPECTOR)
#define ENABLE_REMOTE_INSPECTOR 1
#endif

#if !defined(ENABLE_TRACKING_PREVENTION)
#define ENABLE_TRACKING_PREVENTION 1
#endif

#if ((PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 130000) \
    || PLATFORM(IOS) \
    || PLATFORM(MACCATALYST) \
    || PLATFORM(VISION))
#define ENABLE_JS_COOKIE_CHECKING 1
#endif

#if !defined(ENABLE_SPEECH_SYNTHESIS) && !PLATFORM(MACCATALYST)
#define ENABLE_SPEECH_SYNTHESIS 1
#endif

#if !defined(ENABLE_RESOURCE_USAGE)
#define ENABLE_RESOURCE_USAGE 1
#endif

#if !defined(ENABLE_REVEAL) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define ENABLE_REVEAL 1
#endif

#if !defined(ENABLE_ROUTING_ARBITRATION) && PLATFORM(MAC)
#define ENABLE_ROUTING_ARBITRATION 1
#endif

#if !defined(ENABLE_RUBBER_BANDING) && PLATFORM(MAC)
#define ENABLE_RUBBER_BANDING 1
#endif

#if !defined(ENABLE_SANDBOX_EXTENSIONS)
#define ENABLE_SANDBOX_EXTENSIONS 1
#endif

#if !defined(ENABLE_SCROLLING_THREAD) && PLATFORM(MAC)
#define ENABLE_SCROLLING_THREAD 1
#endif

#if !defined(ENABLE_SEC_ITEM_SHIM)
#define ENABLE_SEC_ITEM_SHIM 1
#endif

#if !defined(ENABLE_SEPARATED_WX_HEAP) && CPU(ARM64)
#define ENABLE_SEPARATED_WX_HEAP 1
#endif

#if !defined(ENABLE_SERVER_PRECONNECT)
#define ENABLE_SERVER_PRECONNECT 1
#endif

#if !defined(ENABLE_SERVICE_CONTROLS) && PLATFORM(MAC)
#define ENABLE_SERVICE_CONTROLS 1
#endif

#if !defined(ENABLE_SHAREABLE_RESOURCE)
#define ENABLE_SHAREABLE_RESOURCE 1
#endif

// FIXME: Should this be enabled for watchOS and tvOS?
#if !defined(ENABLE_TAKE_UNBOUNDED_NETWORKING_ASSERTION) && !PLATFORM(MAC) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define ENABLE_TAKE_UNBOUNDED_NETWORKING_ASSERTION 1
#endif

#if !defined(ENABLE_TELEPHONE_NUMBER_DETECTION)
#define ENABLE_TELEPHONE_NUMBER_DETECTION 1
#endif

#if !defined(ENABLE_TEXT_AUTOSIZING)
#define ENABLE_TEXT_AUTOSIZING 1
#endif

#if !defined(ENABLE_TEXT_CARET) && PLATFORM(IOS_FAMILY)
#define ENABLE_TEXT_CARET 0
#endif

#if !defined(ENABLE_TEXT_SELECTION) && PLATFORM(IOS_FAMILY)
#define ENABLE_TEXT_SELECTION 0
#endif

#if !defined(ENABLE_TOUCH_EVENTS) && PLATFORM(IOS_FAMILY) && USE(APPLE_INTERNAL_SDK)
#define ENABLE_TOUCH_EVENTS 1
#endif

#if !defined(ENABLE_TOUCH_ACTION_REGIONS) && PLATFORM(IOS_FAMILY)
#define ENABLE_TOUCH_ACTION_REGIONS 1
#endif

// FIXME (rdar://49961589): Enable on macCatalyst.
#if !defined(ENABLE_TRANSCODE_UIIMAGEPICKERCONTROLLER_VIDEO) && (PLATFORM(IOS) || PLATFORM(VISION))
#define ENABLE_TRANSCODE_UIIMAGEPICKERCONTROLLER_VIDEO 1
#endif

#if !defined(ENABLE_WHEEL_EVENT_REGIONS) && (PLATFORM(MAC) || PLATFORM(IOS) || PLATFORM(MACCATALYST) || PLATFORM(VISION))
#define ENABLE_WHEEL_EVENT_REGIONS 1
#endif

#if !defined(ENABLE_UI_SIDE_COMPOSITING)
#define ENABLE_UI_SIDE_COMPOSITING 1
#endif

#if !defined(ENABLE_USER_MESSAGE_HANDLERS)
#define ENABLE_USER_MESSAGE_HANDLERS 1
#endif

#if !defined(ENABLE_VARIATION_FONTS)
#define ENABLE_VARIATION_FONTS 1
#endif

#if !defined(ENABLE_VIDEO)
#define ENABLE_VIDEO 1
#endif

#if !defined(ENABLE_VIDEO_PRESENTATION_MODE) && !PLATFORM(IOS_FAMILY_SIMULATOR) && !PLATFORM(APPLETV)
#define ENABLE_VIDEO_PRESENTATION_MODE 1
#endif

#if !defined(ENABLE_VIDEO_USES_ELEMENT_FULLSCREEN) && (PLATFORM(MAC) || PLATFORM(VISION))
#define ENABLE_VIDEO_USES_ELEMENT_FULLSCREEN 1
#endif

#if PLATFORM(VISION)
#define HAVE_PIP_CONTROLLER 1
#endif

#if !defined(ENABLE_VP9) && (PLATFORM(MAC) || PLATFORM(IOS) || PLATFORM(IOS_SIMULATOR) || PLATFORM(VISION))
#define ENABLE_VP9 1
#endif

#if !defined(ENABLE_WEBDRIVER_KEYBOARD_GRAPHEME_CLUSTERS)
#define ENABLE_WEBDRIVER_KEYBOARD_GRAPHEME_CLUSTERS 1
#endif

#if !defined(ENABLE_WEBDRIVER_KEYBOARD_INTERACTIONS)
#define ENABLE_WEBDRIVER_KEYBOARD_INTERACTIONS 1
#endif

#if !defined(ENABLE_WEBDRIVER_MOUSE_INTERACTIONS) && PLATFORM(MAC)
#define ENABLE_WEBDRIVER_MOUSE_INTERACTIONS 1
#endif

#if !defined(ENABLE_WEBDRIVER_TOUCH_INTERACTIONS) && PLATFORM(IOS_FAMILY)
#define ENABLE_WEBDRIVER_TOUCH_INTERACTIONS 1
#endif

#if !defined(ENABLE_WEBDRIVER_WHEEL_INTERACTIONS) && PLATFORM(MAC)
#define ENABLE_WEBDRIVER_WHEEL_INTERACTIONS 1
#endif

#if !defined(ENABLE_WEBGL)
#define ENABLE_WEBGL 1
#endif

#if !defined(ENABLE_WEBPROCESS_NSRUNLOOP) && PLATFORM(MAC)
#define ENABLE_WEBPROCESS_NSRUNLOOP 1
#endif

#if !defined(ENABLE_WEB_API_STATISTICS)
#define ENABLE_WEB_API_STATISTICS 1
#endif

#if !defined(ENABLE_WEB_ARCHIVE)
#define ENABLE_WEB_ARCHIVE 1
#endif

#if !defined(ENABLE_WEB_AUDIO)
#define ENABLE_WEB_AUDIO 1
#endif

#if !defined(ENABLE_WEB_AUTHN) && !PLATFORM(MACCATALYST) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define ENABLE_WEB_AUTHN 1
#endif

#if !defined(ENABLE_WEB_RTC) && !PLATFORM(MACCATALYST) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define ENABLE_WEB_RTC 1
#endif

#if !defined(ENABLE_WEB_CODECS) && !PLATFORM(MACCATALYST) && !PLATFORM(WATCHOS) && !PLATFORM(APPLETV)
#define ENABLE_WEB_CODECS 1
#endif

#if !defined(ENABLE_WK_WEB_EXTENSIONS) && (PLATFORM(MAC) || PLATFORM(MACCATALYST) || PLATFORM(IOS) || PLATFORM(IOS_SIMULATOR) || PLATFORM(VISION))
#define ENABLE_WK_WEB_EXTENSIONS 1
#endif

#if !defined(ENABLE_WEB_CRYPTO)
#define ENABLE_WEB_CRYPTO 1
#endif

#if !defined(ENABLE_WEB_PLAYBACK_CONTROLS_MANAGER) && PLATFORM(MAC)
#define ENABLE_WEB_PLAYBACK_CONTROLS_MANAGER 1
#endif

#if !defined(ENABLE_WIRELESS_PLAYBACK_TARGET)
#define ENABLE_WIRELESS_PLAYBACK_TARGET 1
#endif

#if !defined(ENABLE_WIRELESS_PLAYBACK_TARGET_AVAILABILITY_API) && ENABLE(WIRELESS_PLAYBACK_TARGET) && !PLATFORM(VISION)
#define ENABLE_WIRELESS_PLAYBACK_TARGET_AVAILABILITY_API 1
#endif

#if !defined(ENABLE_WKPDFVIEW) && (PLATFORM(IOS) || PLATFORM(MACCATALYST) || PLATFORM(VISION))
#define ENABLE_WKPDFVIEW 1
#endif

#if !defined(ENABLE_VORBIS) && (PLATFORM(MAC) || PLATFORM(IOS) || PLATFORM(VISION))
#define ENABLE_VORBIS 1
#endif

#if !defined(ENABLE_SET_WEBCONTENT_PROCESS_INFORMATION_IN_NETWORK_PROCESS) && PLATFORM(MAC)
#define ENABLE_SET_WEBCONTENT_PROCESS_INFORMATION_IN_NETWORK_PROCESS 1
#endif

#if !defined(ENABLE_WEBM_FORMAT_READER) && PLATFORM(MAC)
#define ENABLE_WEBM_FORMAT_READER 1
#endif

// FIXME: Is it intentional that tvOS and macCatalyst are omitted?
#if !defined(ENABLE_OPUS) && (PLATFORM(MAC) || PLATFORM(IOS) || PLATFORM(VISION))
#define ENABLE_OPUS 1
#endif

#if !defined(ENABLE_SEPARATED_MODEL) && PLATFORM(VISION) && HAVE(CORE_ANIMATION_SEPARATED_LAYERS)
#define ENABLE_SEPARATED_MODEL 1
#endif

#if !defined(ENABLE_NOTIFY_FILTERING) && PLATFORM(MAC)
#define ENABLE_NOTIFY_FILTERING 1
#endif

// FIXME: Should this be disabled on tvOS and watchOS?
#if !defined(ENABLE_APP_PRIVACY_REPORT) && PLATFORM(IOS_FAMILY) && !PLATFORM(APPLETV) && !PLATFORM(WATCHOS)
#define ENABLE_APP_PRIVACY_REPORT 1
#endif

#if !defined(ENABLE_APP_HIGHLIGHTS) \
    && (PLATFORM(MAC) || PLATFORM(IOS) || PLATFORM(VISION))
#define ENABLE_APP_HIGHLIGHTS 1
#endif

#if !defined(ENABLE_DESTINATION_COLOR_SPACE_DISPLAY_P3)
#define ENABLE_DESTINATION_COLOR_SPACE_DISPLAY_P3 1
#endif

#if !defined(ENABLE_DESTINATION_COLOR_SPACE_LINEAR_SRGB)
#define ENABLE_DESTINATION_COLOR_SPACE_LINEAR_SRGB 1
#endif

#if !defined(ENABLE_IMAGE_ANALYSIS) && HAVE(VK_IMAGE_ANALYSIS)
#define ENABLE_IMAGE_ANALYSIS 1
#endif

#if !defined(ENABLE_IMAGE_ANALYSIS_FOR_MACHINE_READABLE_CODES) && HAVE(VK_IMAGE_ANALYSIS_FOR_MACHINE_READABLE_CODES)
#define ENABLE_IMAGE_ANALYSIS_FOR_MACHINE_READABLE_CODES 1
#endif

#if !defined(ENABLE_PREDEFINED_COLOR_SPACE_DISPLAY_P3) && HAVE(CORE_ANIMATION_FIX_FOR_RADAR_78402035)
#define ENABLE_PREDEFINED_COLOR_SPACE_DISPLAY_P3 1
#endif

#if PLATFORM(IOS_FAMILY) && HAVE(ASV_INLINE_PREVIEW) && !ENABLE(SEPARATED_MODEL)
#define ENABLE_ARKIT_INLINE_PREVIEW_IOS 1
#endif

#if PLATFORM(MAC) && HAVE(ASV_INLINE_PREVIEW)
#define ENABLE_ARKIT_INLINE_PREVIEW_MAC 1
#endif

#if ENABLE(ARKIT_INLINE_PREVIEW_IOS) || ENABLE(ARKIT_INLINE_PREVIEW_MAC)
#define ENABLE_ARKIT_INLINE_PREVIEW 1
#endif

#if ENABLE(ARKIT_INLINE_PREVIEW_MAC) || ENABLE(ARKIT_INLINE_PREVIEW_IOS)
#define ENABLE_ARKIT_INLINE_PREVIEW_CAMERA_TRANSFORM 1
#define ENABLE_ARKIT_INLINE_PREVIEW_ANIMATIONS_CONTROL 1
#define ENABLE_ARKIT_INLINE_PREVIEW_AUDIO_CONTROL 1
#endif

#if !defined(ENABLE_ADDITIONAL_PRECONNECT_ON_HTTP_1X) && PLATFORM(MAC)
#define ENABLE_ADDITIONAL_PRECONNECT_ON_HTTP_1X 1
#endif

#if !defined(ENABLE_FULLSCREEN_DISMISSAL_GESTURES) && PLATFORM(IOS_FAMILY)
#define ENABLE_FULLSCREEN_DISMISSAL_GESTURES 1
#endif

#if (!defined(ENABLE_LOCKDOWN_MODE_API) && (PLATFORM(MAC) || PLATFORM(IOS) || PLATFORM(VISION)))
#define ENABLE_LOCKDOWN_MODE_API 1
#endif

#if !defined(ENABLE_WEBCONTENT_CRASH_TESTING) && USE(APPLE_INTERNAL_SDK) \
    && ((PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 130000) \
        || PLATFORM(IOS) \
        || PLATFORM(VISION)) \
    && HAVE(CHIRP_SPI)
#define ENABLE_WEBCONTENT_CRASH_TESTING 1
#endif

#if !defined(ENABLE_WEBCONTENT_GPU_SANDBOX_EXTENSIONS_BLOCKING) && ((PLATFORM(IOS) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 170000) || PLATFORM(VISION))
#define ENABLE_WEBCONTENT_GPU_SANDBOX_EXTENSIONS_BLOCKING 1
#endif

#if !defined(ENABLE_MOBILE_GESTALT_DEVICE_NAME) && PLATFORM(IOS)
#define ENABLE_MOBILE_GESTALT_DEVICE_NAME 1
#endif

#if !defined(ENABLE_LOCKDOWN_MODE_TELEMETRY) && PLATFORM(MAC)
#define ENABLE_LOCKDOWN_MODE_TELEMETRY 1
#endif

#if !defined(ENABLE_THREADED_ANIMATION_RESOLUTION)
#define ENABLE_THREADED_ANIMATION_RESOLUTION 1
#endif

#if !defined(ENABLE_WEBGL_IN_WORKERS) \
    && ((PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 140000) \
    || ((PLATFORM(IOS) || PLATFORM(MACCATALYST)) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 170000) \
    || (PLATFORM(WATCHOS) && __WATCH_OS_VERSION_MIN_REQUIRED >= 100000) \
    || (PLATFORM(APPLETV) && __TV_OS_VERSION_MIN_REQUIRED >= 170000) \
    || PLATFORM(VISION))
#define ENABLE_WEBGL_IN_WORKERS 1
#endif

#if !defined(ENABLE_CARENDERSERVER_BLOCKING_IN_WEBCONTENT) \
    && (((PLATFORM(IOS) || PLATFORM(MACCATALYST)) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 170000) && !PLATFORM(VISION) \
    || (PLATFORM(WATCHOS) && __WATCH_OS_VERSION_MIN_REQUIRED >= 100000) \
    || (PLATFORM(APPLETV) && __TV_OS_VERSION_MIN_REQUIRED >= 170000) \
    || PLATFORM(VISION))
#define ENABLE_CARENDERSERVER_BLOCKING_IN_WEBCONTENT 1
#endif

#if !defined(ENABLE_LAUNCHD_BLOCKING_IN_WEBCONTENT) \
    && ((PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 140000) \
    || PLATFORM(IOS_FAMILY))
#define ENABLE_LAUNCHD_BLOCKING_IN_WEBCONTENT 1
#endif

#if !defined(ENABLE_LOGD_BLOCKING_IN_WEBCONTENT) && PLATFORM(IOS) && !PLATFORM(VISION) && USE(APPLE_INTERNAL_SDK)
#define ENABLE_LOGD_BLOCKING_IN_WEBCONTENT 1
#endif

#if !defined(ENABLE_SAMPLE_BUFFER_CONTENT_KEY_SESSION_SUPPORT) \
    && ((PLATFORM(MAC) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 140000) \
    || ((PLATFORM(IOS) || PLATFORM(MACCATALYST)) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 170000) \
    || (PLATFORM(WATCHOS) && __WATCH_OS_VERSION_MIN_REQUIRED >= 100000) \
    || (PLATFORM(APPLETV) && __TV_OS_VERSION_MIN_REQUIRED >= 170000) \
    || PLATFORM(VISION))
#define ENABLE_SAMPLE_BUFFER_CONTENT_KEY_SESSION_SUPPORT 1
#endif

#if !defined(ENABLE_PREWARM_WEBPROCESS_ON_PROVISIONAL_LOAD) && PLATFORM(IOS)
#define ENABLE_PREWARM_WEBPROCESS_ON_PROVISIONAL_LOAD 1
#endif
