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
#include "JSTestDefaultToJSONInheritFinal.h"

#include "ActiveDOMObject.h"
#include "DeprecatedGlobalSettings.h"
#include "Document.h"
#include "ExtendedDOMClientIsoSubspaces.h"
#include "ExtendedDOMIsoSubspaces.h"
#include "JSDOMAttribute.h"
#include "JSDOMBinding.h"
#include "JSDOMConstructorNotConstructable.h"
#include "JSDOMConvertBoolean.h"
#include "JSDOMConvertInterface.h"
#include "JSDOMConvertNullable.h"
#include "JSDOMConvertNumbers.h"
#include "JSDOMConvertRecord.h"
#include "JSDOMConvertSequences.h"
#include "JSDOMConvertStrings.h"
#include "JSDOMExceptionHandling.h"
#include "JSDOMGlobalObject.h"
#include "JSDOMGlobalObjectInlines.h"
#include "JSDOMOperation.h"
#include "JSDOMWrapperCache.h"
#include "JSTestDefaultToJSONIndirectInheritance.h"
#include "JSTestDefaultToJSONInheritFinal.h"
#include "ScriptExecutionContext.h"
#include "WebCoreJSClientData.h"
#include <JavaScriptCore/HeapAnalyzer.h>
#include <JavaScriptCore/JSArray.h>
#include <JavaScriptCore/JSCInlines.h>
#include <JavaScriptCore/JSDestructibleObjectHeapCellType.h>
#include <JavaScriptCore/ObjectConstructor.h>
#include <JavaScriptCore/SlotVisitorMacros.h>
#include <JavaScriptCore/SubspaceInlines.h>
#include <wtf/GetPtr.h>
#include <wtf/PointerPreparations.h>
#include <wtf/URL.h>
#include <wtf/Vector.h>

#if ENABLE(TEST_CONDITIONAL)
#include "JSDOMConvertEnumeration.h"
#include "JSTestDefaultToJSONEnum.h"
#endif


namespace WebCore {
using namespace JSC;

// Functions

static JSC_DECLARE_HOST_FUNCTION(jsTestDefaultToJSONInheritFinalPrototypeFunction_toJSON);

// Attributes

static JSC_DECLARE_CUSTOM_GETTER(jsTestDefaultToJSONInheritFinalConstructor);
static JSC_DECLARE_CUSTOM_GETTER(jsTestDefaultToJSONInheritFinal_finalLongAttributeFoo);
static JSC_DECLARE_CUSTOM_SETTER(setJSTestDefaultToJSONInheritFinal_finalLongAttributeFoo);
static JSC_DECLARE_CUSTOM_GETTER(jsTestDefaultToJSONInheritFinal_finalLongAttributeBar);
static JSC_DECLARE_CUSTOM_SETTER(setJSTestDefaultToJSONInheritFinal_finalLongAttributeBar);

class JSTestDefaultToJSONInheritFinalPrototype final : public JSC::JSNonFinalObject {
public:
    using Base = JSC::JSNonFinalObject;
    static JSTestDefaultToJSONInheritFinalPrototype* create(JSC::VM& vm, JSDOMGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSTestDefaultToJSONInheritFinalPrototype* ptr = new (NotNull, JSC::allocateCell<JSTestDefaultToJSONInheritFinalPrototype>(vm)) JSTestDefaultToJSONInheritFinalPrototype(vm, globalObject, structure);
        ptr->finishCreation(vm);
        return ptr;
    }

    DECLARE_INFO;
    template<typename CellType, JSC::SubspaceAccess>
    static JSC::GCClient::IsoSubspace* subspaceFor(JSC::VM& vm)
    {
        STATIC_ASSERT_ISO_SUBSPACE_SHARABLE(JSTestDefaultToJSONInheritFinalPrototype, Base);
        return &vm.plainObjectSpace();
    }
    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

private:
    JSTestDefaultToJSONInheritFinalPrototype(JSC::VM& vm, JSC::JSGlobalObject*, JSC::Structure* structure)
        : JSC::JSNonFinalObject(vm, structure)
    {
    }

    void finishCreation(JSC::VM&);
};
STATIC_ASSERT_ISO_SUBSPACE_SHARABLE(JSTestDefaultToJSONInheritFinalPrototype, JSTestDefaultToJSONInheritFinalPrototype::Base);

using JSTestDefaultToJSONInheritFinalDOMConstructor = JSDOMConstructorNotConstructable<JSTestDefaultToJSONInheritFinal>;

template<> const ClassInfo JSTestDefaultToJSONInheritFinalDOMConstructor::s_info = { "TestDefaultToJSONInheritFinal"_s, &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestDefaultToJSONInheritFinalDOMConstructor) };

template<> JSValue JSTestDefaultToJSONInheritFinalDOMConstructor::prototypeForStructure(JSC::VM& vm, const JSDOMGlobalObject& globalObject)
{
    return JSTestDefaultToJSONInherit::getConstructor(vm, &globalObject);
}

template<> void JSTestDefaultToJSONInheritFinalDOMConstructor::initializeProperties(VM& vm, JSDOMGlobalObject& globalObject)
{
    putDirect(vm, vm.propertyNames->length, jsNumber(0), JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum);
    JSString* nameString = jsNontrivialString(vm, "TestDefaultToJSONInheritFinal"_s);
    m_originalName.set(vm, this, nameString);
    putDirect(vm, vm.propertyNames->name, nameString, JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum);
    putDirect(vm, vm.propertyNames->prototype, JSTestDefaultToJSONInheritFinal::prototype(vm, globalObject), JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum | JSC::PropertyAttribute::DontDelete);
}

/* Hash table for prototype */

static const HashTableValue JSTestDefaultToJSONInheritFinalPrototypeTableValues[] =
{
    { "constructor"_s, static_cast<unsigned>(PropertyAttribute::DontEnum), NoIntrinsic, { HashTableValue::GetterSetterType, jsTestDefaultToJSONInheritFinalConstructor, 0 } },
    { "finalLongAttributeFoo"_s, JSC::PropertyAttribute::CustomAccessor | JSC::PropertyAttribute::DOMAttribute, NoIntrinsic, { HashTableValue::GetterSetterType, jsTestDefaultToJSONInheritFinal_finalLongAttributeFoo, setJSTestDefaultToJSONInheritFinal_finalLongAttributeFoo } },
    { "finalLongAttributeBar"_s, JSC::PropertyAttribute::CustomAccessor | JSC::PropertyAttribute::DOMAttribute, NoIntrinsic, { HashTableValue::GetterSetterType, jsTestDefaultToJSONInheritFinal_finalLongAttributeBar, setJSTestDefaultToJSONInheritFinal_finalLongAttributeBar } },
    { "toJSON"_s, static_cast<unsigned>(JSC::PropertyAttribute::Function), NoIntrinsic, { HashTableValue::NativeFunctionType, jsTestDefaultToJSONInheritFinalPrototypeFunction_toJSON, 0 } },
};

const ClassInfo JSTestDefaultToJSONInheritFinalPrototype::s_info = { "TestDefaultToJSONInheritFinal"_s, &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestDefaultToJSONInheritFinalPrototype) };

void JSTestDefaultToJSONInheritFinalPrototype::finishCreation(VM& vm)
{
    Base::finishCreation(vm);
    reifyStaticProperties(vm, JSTestDefaultToJSONInheritFinal::info(), JSTestDefaultToJSONInheritFinalPrototypeTableValues, *this);
    JSC_TO_STRING_TAG_WITHOUT_TRANSITION();
}

const ClassInfo JSTestDefaultToJSONInheritFinal::s_info = { "TestDefaultToJSONInheritFinal"_s, &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestDefaultToJSONInheritFinal) };

JSTestDefaultToJSONInheritFinal::JSTestDefaultToJSONInheritFinal(Structure* structure, JSDOMGlobalObject& globalObject, Ref<TestDefaultToJSONInheritFinal>&& impl)
    : JSTestDefaultToJSONInherit(structure, globalObject, WTFMove(impl))
{
}

static_assert(!std::is_base_of<ActiveDOMObject, TestDefaultToJSONInheritFinal>::value, "Interface is not marked as [ActiveDOMObject] even though implementation class subclasses ActiveDOMObject.");

JSObject* JSTestDefaultToJSONInheritFinal::createPrototype(VM& vm, JSDOMGlobalObject& globalObject)
{
    auto* structure = JSTestDefaultToJSONInheritFinalPrototype::createStructure(vm, &globalObject, JSTestDefaultToJSONInherit::prototype(vm, globalObject));
    structure->setMayBePrototype(true);
    return JSTestDefaultToJSONInheritFinalPrototype::create(vm, &globalObject, structure);
}

JSObject* JSTestDefaultToJSONInheritFinal::prototype(VM& vm, JSDOMGlobalObject& globalObject)
{
    return getDOMPrototype<JSTestDefaultToJSONInheritFinal>(vm, globalObject);
}

JSValue JSTestDefaultToJSONInheritFinal::getConstructor(VM& vm, const JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSTestDefaultToJSONInheritFinalDOMConstructor, DOMConstructorID::TestDefaultToJSONInheritFinal>(vm, *jsCast<const JSDOMGlobalObject*>(globalObject));
}

JSC_DEFINE_CUSTOM_GETTER(jsTestDefaultToJSONInheritFinalConstructor, (JSGlobalObject* lexicalGlobalObject, EncodedJSValue thisValue, PropertyName))
{
    VM& vm = JSC::getVM(lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    auto* prototype = jsDynamicCast<JSTestDefaultToJSONInheritFinalPrototype*>(JSValue::decode(thisValue));
    if (UNLIKELY(!prototype))
        return throwVMTypeError(lexicalGlobalObject, throwScope);
    return JSValue::encode(JSTestDefaultToJSONInheritFinal::getConstructor(JSC::getVM(lexicalGlobalObject), prototype->globalObject()));
}

static inline JSValue jsTestDefaultToJSONInheritFinal_finalLongAttributeFooGetter(JSGlobalObject& lexicalGlobalObject, JSTestDefaultToJSONInheritFinal& thisObject)
{
    auto& vm = JSC::getVM(&lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    auto& impl = thisObject.wrapped();
    RELEASE_AND_RETURN(throwScope, (toJS<IDLLong>(lexicalGlobalObject, throwScope, impl.finalLongAttributeFoo())));
}

JSC_DEFINE_CUSTOM_GETTER(jsTestDefaultToJSONInheritFinal_finalLongAttributeFoo, (JSGlobalObject* lexicalGlobalObject, EncodedJSValue thisValue, PropertyName attributeName))
{
    return IDLAttribute<JSTestDefaultToJSONInheritFinal>::get<jsTestDefaultToJSONInheritFinal_finalLongAttributeFooGetter, CastedThisErrorBehavior::Assert>(*lexicalGlobalObject, thisValue, attributeName);
}

static inline bool setJSTestDefaultToJSONInheritFinal_finalLongAttributeFooSetter(JSGlobalObject& lexicalGlobalObject, JSTestDefaultToJSONInheritFinal& thisObject, JSValue value)
{
    auto& vm = JSC::getVM(&lexicalGlobalObject);
    UNUSED_PARAM(vm);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    auto& impl = thisObject.wrapped();
    auto nativeValue = convert<IDLLong>(lexicalGlobalObject, value);
    RETURN_IF_EXCEPTION(throwScope, false);
    invokeFunctorPropagatingExceptionIfNecessary(lexicalGlobalObject, throwScope, [&] {
        return impl.setFinalLongAttributeFoo(WTFMove(nativeValue));
    });
    return true;
}

JSC_DEFINE_CUSTOM_SETTER(setJSTestDefaultToJSONInheritFinal_finalLongAttributeFoo, (JSGlobalObject* lexicalGlobalObject, EncodedJSValue thisValue, EncodedJSValue encodedValue, PropertyName attributeName))
{
    return IDLAttribute<JSTestDefaultToJSONInheritFinal>::set<setJSTestDefaultToJSONInheritFinal_finalLongAttributeFooSetter>(*lexicalGlobalObject, thisValue, encodedValue, attributeName);
}

static inline JSValue jsTestDefaultToJSONInheritFinal_finalLongAttributeBarGetter(JSGlobalObject& lexicalGlobalObject, JSTestDefaultToJSONInheritFinal& thisObject)
{
    auto& vm = JSC::getVM(&lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    auto& impl = thisObject.wrapped();
    RELEASE_AND_RETURN(throwScope, (toJS<IDLLong>(lexicalGlobalObject, throwScope, impl.finalLongAttributeBar())));
}

JSC_DEFINE_CUSTOM_GETTER(jsTestDefaultToJSONInheritFinal_finalLongAttributeBar, (JSGlobalObject* lexicalGlobalObject, EncodedJSValue thisValue, PropertyName attributeName))
{
    return IDLAttribute<JSTestDefaultToJSONInheritFinal>::get<jsTestDefaultToJSONInheritFinal_finalLongAttributeBarGetter, CastedThisErrorBehavior::Assert>(*lexicalGlobalObject, thisValue, attributeName);
}

static inline bool setJSTestDefaultToJSONInheritFinal_finalLongAttributeBarSetter(JSGlobalObject& lexicalGlobalObject, JSTestDefaultToJSONInheritFinal& thisObject, JSValue value)
{
    auto& vm = JSC::getVM(&lexicalGlobalObject);
    UNUSED_PARAM(vm);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    auto& impl = thisObject.wrapped();
    auto nativeValue = convert<IDLLong>(lexicalGlobalObject, value);
    RETURN_IF_EXCEPTION(throwScope, false);
    invokeFunctorPropagatingExceptionIfNecessary(lexicalGlobalObject, throwScope, [&] {
        return impl.setFinalLongAttributeBar(WTFMove(nativeValue));
    });
    return true;
}

JSC_DEFINE_CUSTOM_SETTER(setJSTestDefaultToJSONInheritFinal_finalLongAttributeBar, (JSGlobalObject* lexicalGlobalObject, EncodedJSValue thisValue, EncodedJSValue encodedValue, PropertyName attributeName))
{
    return IDLAttribute<JSTestDefaultToJSONInheritFinal>::set<setJSTestDefaultToJSONInheritFinal_finalLongAttributeBarSetter>(*lexicalGlobalObject, thisValue, encodedValue, attributeName);
}

static inline EncodedJSValue jsTestDefaultToJSONInheritFinalPrototypeFunction_toJSONBody(JSGlobalObject* lexicalGlobalObject, CallFrame*, JSTestDefaultToJSONInheritFinal* castedThis)
{
    auto& vm = JSC::getVM(lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    UNUSED_PARAM(throwScope);
    auto& impl = castedThis->wrapped();
    auto* result = constructEmptyObject(lexicalGlobalObject);
    if (DeprecatedGlobalSettings::testDeprecatedGlobalSettingEnabled()) {
        auto longAttributeValue = toJS<IDLLong>(*lexicalGlobalObject, throwScope, impl.longAttribute());
        RETURN_IF_EXCEPTION(throwScope, { });
        result->putDirect(vm, Identifier::fromString(vm, "longAttribute"_s), longAttributeValue);
    }
    if (downcast<Document>(jsCast<JSDOMGlobalObject*>(castedThis->globalObject())->scriptExecutionContext())->settingsValues().testSettingEnabled) {
        auto enabledBySettingsAttributeValue = toJS<IDLUnsignedShort>(*lexicalGlobalObject, throwScope, impl.enabledBySettingsAttribute());
        RETURN_IF_EXCEPTION(throwScope, { });
        result->putDirect(vm, Identifier::fromString(vm, "enabledBySettingsAttribute"_s), enabledBySettingsAttributeValue);
    }
#if ENABLE(TEST_CONDITIONAL)
    auto enabledByConditionalAttributeValue = toJS<IDLEnumeration<TestDefaultToJSONEnum>>(*lexicalGlobalObject, throwScope, impl.enabledByConditionalAttribute());
    RETURN_IF_EXCEPTION(throwScope, { });
    result->putDirect(vm, Identifier::fromString(vm, "enabledByConditionalAttribute"_s), enabledByConditionalAttributeValue);
#endif

    auto firstStringAttributeValue = toJS<IDLDOMString>(*lexicalGlobalObject, throwScope, impl.firstStringAttribute());
    RETURN_IF_EXCEPTION(throwScope, { });
    result->putDirect(vm, Identifier::fromString(vm, "firstStringAttribute"_s), firstStringAttributeValue);
    auto secondLongAttributeValue = toJS<IDLLong>(*lexicalGlobalObject, throwScope, impl.secondLongAttribute());
    RETURN_IF_EXCEPTION(throwScope, { });
    result->putDirect(vm, Identifier::fromString(vm, "secondLongAttribute"_s), secondLongAttributeValue);
    auto fourthUnrestrictedDoubleAttributeValue = toJS<IDLUnrestrictedDouble>(*lexicalGlobalObject, throwScope, impl.fourthUnrestrictedDoubleAttribute());
    RETURN_IF_EXCEPTION(throwScope, { });
    result->putDirect(vm, Identifier::fromString(vm, "fourthUnrestrictedDoubleAttribute"_s), fourthUnrestrictedDoubleAttributeValue);
    auto fifthLongClampedAttributeValue = toJS<IDLClampAdaptor<IDLLong>>(*lexicalGlobalObject, throwScope, impl.fifthLongClampedAttribute());
    RETURN_IF_EXCEPTION(throwScope, { });
    result->putDirect(vm, Identifier::fromString(vm, "fifthLongClampedAttribute"_s), fifthLongClampedAttributeValue);
    auto sixthTypedefAttributeValue = toJS<IDLDouble>(*lexicalGlobalObject, throwScope, impl.sixthTypedefAttribute());
    RETURN_IF_EXCEPTION(throwScope, { });
    result->putDirect(vm, Identifier::fromString(vm, "sixthTypedefAttribute"_s), sixthTypedefAttributeValue);
    auto seventhDirectlyToJSONableAttributeValue = toJS<IDLInterface<TestDefaultToJSONInheritFinal>>(*lexicalGlobalObject, *castedThis->globalObject(), throwScope, impl.seventhDirectlyToJSONableAttribute());
    RETURN_IF_EXCEPTION(throwScope, { });
    result->putDirect(vm, Identifier::fromString(vm, "seventhDirectlyToJSONableAttribute"_s), seventhDirectlyToJSONableAttributeValue);
    auto eighthIndirectlyAttributeValue = toJS<IDLInterface<TestDefaultToJSONIndirectInheritance>>(*lexicalGlobalObject, *castedThis->globalObject(), throwScope, impl.eighthIndirectlyAttribute());
    RETURN_IF_EXCEPTION(throwScope, { });
    result->putDirect(vm, Identifier::fromString(vm, "eighthIndirectlyAttribute"_s), eighthIndirectlyAttributeValue);
    auto ninthOptionalDirectlyToJSONableAttributeValue = toJS<IDLNullable<IDLInterface<TestDefaultToJSONInheritFinal>>>(*lexicalGlobalObject, *castedThis->globalObject(), throwScope, impl.ninthOptionalDirectlyToJSONableAttribute());
    RETURN_IF_EXCEPTION(throwScope, { });
    result->putDirect(vm, Identifier::fromString(vm, "ninthOptionalDirectlyToJSONableAttribute"_s), ninthOptionalDirectlyToJSONableAttributeValue);
    auto tenthFrozenArrayAttributeValue = toJS<IDLFrozenArray<IDLBoolean>>(*lexicalGlobalObject, *castedThis->globalObject(), throwScope, impl.tenthFrozenArrayAttribute());
    RETURN_IF_EXCEPTION(throwScope, { });
    result->putDirect(vm, Identifier::fromString(vm, "tenthFrozenArrayAttribute"_s), tenthFrozenArrayAttributeValue);
    auto eleventhSequenceAttributeValue = toJS<IDLSequence<IDLDOMString>>(*lexicalGlobalObject, *castedThis->globalObject(), throwScope, impl.eleventhSequenceAttribute());
    RETURN_IF_EXCEPTION(throwScope, { });
    result->putDirect(vm, Identifier::fromString(vm, "eleventhSequenceAttribute"_s), eleventhSequenceAttributeValue);
    auto twelfthInterfaceSequenceAttributeValue = toJS<IDLSequence<IDLInterface<TestDefaultToJSONInheritFinal>>>(*lexicalGlobalObject, *castedThis->globalObject(), throwScope, impl.twelfthInterfaceSequenceAttribute());
    RETURN_IF_EXCEPTION(throwScope, { });
    result->putDirect(vm, Identifier::fromString(vm, "twelfthInterfaceSequenceAttribute"_s), twelfthInterfaceSequenceAttributeValue);
    auto thirteenthRecordAttributeValue = toJS<IDLRecord<IDLDOMString, IDLUnsignedShort>>(*lexicalGlobalObject, *castedThis->globalObject(), throwScope, impl.thirteenthRecordAttribute());
    RETURN_IF_EXCEPTION(throwScope, { });
    result->putDirect(vm, Identifier::fromString(vm, "thirteenthRecordAttribute"_s), thirteenthRecordAttributeValue);
    auto inheritLongAttributeValue = toJS<IDLLong>(*lexicalGlobalObject, throwScope, impl.inheritLongAttribute());
    RETURN_IF_EXCEPTION(throwScope, { });
    result->putDirect(vm, Identifier::fromString(vm, "inheritLongAttribute"_s), inheritLongAttributeValue);
    auto finalLongAttributeFooValue = toJS<IDLLong>(*lexicalGlobalObject, throwScope, impl.finalLongAttributeFoo());
    RETURN_IF_EXCEPTION(throwScope, { });
    result->putDirect(vm, Identifier::fromString(vm, "finalLongAttributeFoo"_s), finalLongAttributeFooValue);
    auto finalLongAttributeBarValue = toJS<IDLLong>(*lexicalGlobalObject, throwScope, impl.finalLongAttributeBar());
    RETURN_IF_EXCEPTION(throwScope, { });
    result->putDirect(vm, Identifier::fromString(vm, "finalLongAttributeBar"_s), finalLongAttributeBarValue);
    return JSValue::encode(result);
}

JSC_DEFINE_HOST_FUNCTION(jsTestDefaultToJSONInheritFinalPrototypeFunction_toJSON, (JSGlobalObject* lexicalGlobalObject, CallFrame* callFrame))
{
    return IDLOperation<JSTestDefaultToJSONInheritFinal>::call<jsTestDefaultToJSONInheritFinalPrototypeFunction_toJSONBody>(*lexicalGlobalObject, *callFrame, "toJSON");
}

JSC::GCClient::IsoSubspace* JSTestDefaultToJSONInheritFinal::subspaceForImpl(JSC::VM& vm)
{
    return WebCore::subspaceForImpl<JSTestDefaultToJSONInheritFinal, UseCustomHeapCellType::No>(vm,
        [] (auto& spaces) { return spaces.m_clientSubspaceForTestDefaultToJSONInheritFinal.get(); },
        [] (auto& spaces, auto&& space) { spaces.m_clientSubspaceForTestDefaultToJSONInheritFinal = std::forward<decltype(space)>(space); },
        [] (auto& spaces) { return spaces.m_subspaceForTestDefaultToJSONInheritFinal.get(); },
        [] (auto& spaces, auto&& space) { spaces.m_subspaceForTestDefaultToJSONInheritFinal = std::forward<decltype(space)>(space); }
    );
}

void JSTestDefaultToJSONInheritFinal::analyzeHeap(JSCell* cell, HeapAnalyzer& analyzer)
{
    auto* thisObject = jsCast<JSTestDefaultToJSONInheritFinal*>(cell);
    analyzer.setWrappedObjectForCell(cell, &thisObject->wrapped());
    if (thisObject->scriptExecutionContext())
        analyzer.setLabelForCell(cell, "url "_s + thisObject->scriptExecutionContext()->url().string());
    Base::analyzeHeap(cell, analyzer);
}


}
