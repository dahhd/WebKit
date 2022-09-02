/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"
#include "JSTestDefaultToJSONEnum.h"

#include <JavaScriptCore/JSCInlines.h>
#include <JavaScriptCore/JSString.h>
#include <wtf/NeverDestroyed.h>
#include <wtf/SortedArrayMap.h>


namespace WebCore {
using namespace JSC;

String convertEnumerationToString(TestDefaultToJSONEnum enumerationValue)
{
    static const NeverDestroyed<String> values[] = {
        MAKE_STATIC_STRING_IMPL("EnumValue1"),
        MAKE_STATIC_STRING_IMPL("EnumValue2"),
    };
    static_assert(static_cast<size_t>(TestDefaultToJSONEnum::EnumValue1) == 0, "TestDefaultToJSONEnum::EnumValue1 is not 0 as expected");
    static_assert(static_cast<size_t>(TestDefaultToJSONEnum::EnumValue2) == 1, "TestDefaultToJSONEnum::EnumValue2 is not 1 as expected");
    ASSERT(static_cast<size_t>(enumerationValue) < WTF_ARRAY_LENGTH(values));
    return values[static_cast<size_t>(enumerationValue)];
}

template<> JSString* convertEnumerationToJS(JSGlobalObject& lexicalGlobalObject, TestDefaultToJSONEnum enumerationValue)
{
    return jsStringWithCache(lexicalGlobalObject.vm(), convertEnumerationToString(enumerationValue));
}

template<> std::optional<TestDefaultToJSONEnum> parseEnumeration<TestDefaultToJSONEnum>(JSGlobalObject& lexicalGlobalObject, JSValue value)
{
    auto stringValue = value.toWTFString(&lexicalGlobalObject);
    static constexpr std::pair<ComparableASCIILiteral, TestDefaultToJSONEnum> mappings[] = {
        { "EnumValue1", TestDefaultToJSONEnum::EnumValue1 },
        { "EnumValue2", TestDefaultToJSONEnum::EnumValue2 },
    };
    static constexpr SortedArrayMap enumerationMapping { mappings };
    if (auto* enumerationValue = enumerationMapping.tryGet(stringValue); LIKELY(enumerationValue))
        return *enumerationValue;
    return std::nullopt;
}

template<> const char* expectedEnumerationValues<TestDefaultToJSONEnum>()
{
    return "\"EnumValue1\", \"EnumValue2\"";
}

} // namespace WebCore
