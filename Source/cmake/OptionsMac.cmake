# FIXME: These should line up with versions in Configurations/Version.xcconfig.
# See Source/WebKitLegacy/PlatformWin.cmake for how WebKitVersion.h is generated.
set(WEBKIT_MAC_VERSION 615.1.1)
set(MACOSX_FRAMEWORK_BUNDLE_VERSION 615.1.1+)

WEBKIT_OPTION_BEGIN()
# Private options shared with other WebKit ports. Add options here only if
# we need a value different from the default defined in WebKitFeatures.cmake.

# FIXME: https://bugs.webkit.org/show_bug.cgi?id=231776
# WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_API_TESTS PRIVATE ON)

WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_APPLE_PAY PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_APPLICATION_MANIFEST PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_ASYNC_SCROLLING PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_ATTACHMENT_ELEMENT PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_AVF_CAPTIONS PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_CACHE_PARTITIONING PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_CONTENT_EXTENSIONS PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_CONTENT_FILTERING PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_CSS_PAINTING_API PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_CURSOR_VISIBILITY PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_DARK_MODE_CSS PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_DATACUE_VALUE PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_DATALIST_ELEMENT PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_DRAG_SUPPORT PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_ENCRYPTED_MEDIA PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_EXPERIMENTAL_FEATURES PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_GAMEPAD PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_GPU_PROCESS PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_INPUT_TYPE_COLOR PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_INPUT_TYPE_DATE PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_INPUT_TYPE_DATETIMELOCAL PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_INPUT_TYPE_MONTH PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_INPUT_TYPE_TIME PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_INPUT_TYPE_WEEK PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_INSPECTOR_ALTERNATE_DISPATCHERS PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_INSPECTOR_EXTENSIONS PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_INSPECTOR_TELEMETRY PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_LAYER_BASED_SVG_ENGINE PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_LEGACY_CUSTOM_PROTOCOL_MANAGER PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_LEGACY_ENCRYPTED_MEDIA PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_MEDIA_SOURCE PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_MEDIA_STREAM PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_MEMORY_SAMPLER PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_MOUSE_CURSOR_SCALE PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_NETWORK_CACHE_SPECULATIVE_REVALIDATION PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_NETWORK_CACHE_STALE_WHILE_REVALIDATE PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_PAYMENT_REQUEST PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_PDFKIT_PLUGIN PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_PERIODIC_MEMORY_MONITOR PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_PICTURE_IN_PICTURE_API PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_POINTER_LOCK PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_RESOURCE_USAGE PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_SANDBOX_EXTENSIONS PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_SERVER_PRECONNECT PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_SERVICE_CONTROLS PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_SHAREABLE_RESOURCE PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_SPEECH_SYNTHESIS PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_TELEPHONE_NUMBER_DETECTION PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_TEXT_AUTOSIZING PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_VARIATION_FONTS PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_VIDEO_PRESENTATION_MODE PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_WEBDRIVER_KEYBOARD_INTERACTIONS PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_WEBDRIVER_MOUSE_INTERACTIONS PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_WEBDRIVER_WHEEL_INTERACTIONS PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_WEBXR PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_WEB_API_STATISTICS PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_WEB_AUTHN PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_WEB_RTC PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(ENABLE_WIRELESS_PLAYBACK_TARGET PRIVATE ON)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(USE_AVIF PRIVATE OFF)
WEBKIT_OPTION_DEFAULT_PORT_VALUE(USE_JPEGXL PRIVATE OFF)

WEBKIT_OPTION_END()

SET_AND_EXPOSE_TO_BUILD(USE_LIBWEBRTC TRUE)

set(ENABLE_WEBKIT_LEGACY ON)
set(ENABLE_WEBKIT ON)

set(bmalloc_LIBRARY_TYPE OBJECT)
set(WTF_LIBRARY_TYPE OBJECT)
set(JavaScriptCore_LIBRARY_TYPE SHARED)
set(PAL_LIBRARY_TYPE OBJECT)
set(WebCore_LIBRARY_TYPE SHARED)

find_package(ICU 61.2 REQUIRED COMPONENTS data i18n uc)
find_package(LibXml2 2.8.0 REQUIRED)
find_package(LibXslt 1.1.7 REQUIRED)
