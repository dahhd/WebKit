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
#include "JSTestNamedSetterWithLegacyUnforgeableProperties.h"

#include "ActiveDOMObject.h"
#include "ExtendedDOMClientIsoSubspaces.h"
#include "ExtendedDOMIsoSubspaces.h"
#include "IDLTypes.h"
#include "JSDOMAbstractOperations.h"
#include "JSDOMAttribute.h"
#include "JSDOMBinding.h"
#include "JSDOMConstructorNotConstructable.h"
#include "JSDOMConvertBase.h"
#include "JSDOMConvertStrings.h"
#include "JSDOMExceptionHandling.h"
#include "JSDOMGlobalObjectInlines.h"
#include "JSDOMOperation.h"
#include "JSDOMWrapperCache.h"
#include "ScriptExecutionContext.h"
#include "WebCoreJSClientData.h"
#include <JavaScriptCore/FunctionPrototype.h>
#include <JavaScriptCore/HeapAnalyzer.h>
#include <JavaScriptCore/JSCInlines.h>
#include <JavaScriptCore/JSDestructibleObjectHeapCellType.h>
#include <JavaScriptCore/SlotVisitorMacros.h>
#include <JavaScriptCore/SubspaceInlines.h>
#include <wtf/GetPtr.h>
#include <wtf/PointerPreparations.h>
#include <wtf/URL.h>


namespace WebCore {
using namespace JSC;

// Functions

static JSC_DECLARE_HOST_FUNCTION(jsTestNamedSetterWithLegacyUnforgeablePropertiesInstanceFunction_unforgeableOperation);

// Attributes

static JSC_DECLARE_CUSTOM_GETTER(jsTestNamedSetterWithLegacyUnforgeablePropertiesConstructor);
static JSC_DECLARE_CUSTOM_GETTER(jsTestNamedSetterWithLegacyUnforgeableProperties_unforgeableAttribute);

class JSTestNamedSetterWithLegacyUnforgeablePropertiesPrototype final : public JSC::JSNonFinalObject {
public:
    using Base = JSC::JSNonFinalObject;
    static JSTestNamedSetterWithLegacyUnforgeablePropertiesPrototype* create(JSC::VM& vm, JSDOMGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSTestNamedSetterWithLegacyUnforgeablePropertiesPrototype* ptr = new (NotNull, JSC::allocateCell<JSTestNamedSetterWithLegacyUnforgeablePropertiesPrototype>(vm)) JSTestNamedSetterWithLegacyUnforgeablePropertiesPrototype(vm, globalObject, structure);
        ptr->finishCreation(vm);
        return ptr;
    }

    DECLARE_INFO;
    template<typename CellType, JSC::SubspaceAccess>
    static JSC::GCClient::IsoSubspace* subspaceFor(JSC::VM& vm)
    {
        STATIC_ASSERT_ISO_SUBSPACE_SHARABLE(JSTestNamedSetterWithLegacyUnforgeablePropertiesPrototype, Base);
        return &vm.plainObjectSpace();
    }
    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

private:
    JSTestNamedSetterWithLegacyUnforgeablePropertiesPrototype(JSC::VM& vm, JSC::JSGlobalObject*, JSC::Structure* structure)
        : JSC::JSNonFinalObject(vm, structure)
    {
    }

    void finishCreation(JSC::VM&);
};
STATIC_ASSERT_ISO_SUBSPACE_SHARABLE(JSTestNamedSetterWithLegacyUnforgeablePropertiesPrototype, JSTestNamedSetterWithLegacyUnforgeablePropertiesPrototype::Base);

using JSTestNamedSetterWithLegacyUnforgeablePropertiesDOMConstructor = JSDOMConstructorNotConstructable<JSTestNamedSetterWithLegacyUnforgeableProperties>;

/* Hash table */

static const struct CompactHashIndex JSTestNamedSetterWithLegacyUnforgeablePropertiesTableIndex[4] = {
    { -1, -1 },
    { 1, -1 },
    { -1, -1 },
    { 0, -1 },
};


static const HashTableValue JSTestNamedSetterWithLegacyUnforgeablePropertiesTableValues[] =
{
    { "unforgeableAttribute"_s, static_cast<unsigned>(JSC::PropertyAttribute::DontDelete | JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::CustomAccessor | JSC::PropertyAttribute::DOMAttribute), NoIntrinsic, { (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestNamedSetterWithLegacyUnforgeableProperties_unforgeableAttribute), (intptr_t) static_cast<PutPropertySlot::PutValueFunc>(0) } },
    { "unforgeableOperation"_s, static_cast<unsigned>(JSC::PropertyAttribute::DontDelete | JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::Function), NoIntrinsic, { (intptr_t)static_cast<RawNativeFunction>(jsTestNamedSetterWithLegacyUnforgeablePropertiesInstanceFunction_unforgeableOperation), (intptr_t) (0) } },
};

static const HashTable JSTestNamedSetterWithLegacyUnforgeablePropertiesTable = { 2, 3, true, JSTestNamedSetterWithLegacyUnforgeableProperties::info(), JSTestNamedSetterWithLegacyUnforgeablePropertiesTableValues, JSTestNamedSetterWithLegacyUnforgeablePropertiesTableIndex };
template<> const ClassInfo JSTestNamedSetterWithLegacyUnforgeablePropertiesDOMConstructor::s_info = { "TestNamedSetterWithLegacyUnforgeableProperties"_s, &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestNamedSetterWithLegacyUnforgeablePropertiesDOMConstructor) };

template<> JSValue JSTestNamedSetterWithLegacyUnforgeablePropertiesDOMConstructor::prototypeForStructure(JSC::VM& vm, const JSDOMGlobalObject& globalObject)
{
    UNUSED_PARAM(vm);
    return globalObject.functionPrototype();
}

template<> void JSTestNamedSetterWithLegacyUnforgeablePropertiesDOMConstructor::initializeProperties(VM& vm, JSDOMGlobalObject& globalObject)
{
    putDirect(vm, vm.propertyNames->length, jsNumber(0), JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum);
    JSString* nameString = jsNontrivialString(vm, "TestNamedSetterWithLegacyUnforgeableProperties"_s);
    m_originalName.set(vm, this, nameString);
    putDirect(vm, vm.propertyNames->name, nameString, JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum);
    putDirect(vm, vm.propertyNames->prototype, JSTestNamedSetterWithLegacyUnforgeableProperties::prototype(vm, globalObject), JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum | JSC::PropertyAttribute::DontDelete);
}

/* Hash table for prototype */

static const HashTableValue JSTestNamedSetterWithLegacyUnforgeablePropertiesPrototypeTableValues[] =
{
    { "constructor"_s, static_cast<unsigned>(JSC::PropertyAttribute::DontEnum), NoIntrinsic, { (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestNamedSetterWithLegacyUnforgeablePropertiesConstructor), (intptr_t) static_cast<PutPropertySlot::PutValueFunc>(0) } },
};

const ClassInfo JSTestNamedSetterWithLegacyUnforgeablePropertiesPrototype::s_info = { "TestNamedSetterWithLegacyUnforgeableProperties"_s, &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestNamedSetterWithLegacyUnforgeablePropertiesPrototype) };

void JSTestNamedSetterWithLegacyUnforgeablePropertiesPrototype::finishCreation(VM& vm)
{
    Base::finishCreation(vm);
    reifyStaticProperties(vm, JSTestNamedSetterWithLegacyUnforgeableProperties::info(), JSTestNamedSetterWithLegacyUnforgeablePropertiesPrototypeTableValues, *this);
    JSC_TO_STRING_TAG_WITHOUT_TRANSITION();
}

const ClassInfo JSTestNamedSetterWithLegacyUnforgeableProperties::s_info = { "TestNamedSetterWithLegacyUnforgeableProperties"_s, &Base::s_info, &JSTestNamedSetterWithLegacyUnforgeablePropertiesTable, nullptr, CREATE_METHOD_TABLE(JSTestNamedSetterWithLegacyUnforgeableProperties) };

JSTestNamedSetterWithLegacyUnforgeableProperties::JSTestNamedSetterWithLegacyUnforgeableProperties(Structure* structure, JSDOMGlobalObject& globalObject, Ref<TestNamedSetterWithLegacyUnforgeableProperties>&& impl)
    : JSDOMWrapper<TestNamedSetterWithLegacyUnforgeableProperties>(structure, globalObject, WTFMove(impl))
{
}

void JSTestNamedSetterWithLegacyUnforgeableProperties::finishCreation(VM& vm)
{
    Base::finishCreation(vm);
    ASSERT(inherits(info()));

    static_assert(!std::is_base_of<ActiveDOMObject, TestNamedSetterWithLegacyUnforgeableProperties>::value, "Interface is not marked as [ActiveDOMObject] even though implementation class subclasses ActiveDOMObject.");

}

JSObject* JSTestNamedSetterWithLegacyUnforgeableProperties::createPrototype(VM& vm, JSDOMGlobalObject& globalObject)
{
    return JSTestNamedSetterWithLegacyUnforgeablePropertiesPrototype::create(vm, &globalObject, JSTestNamedSetterWithLegacyUnforgeablePropertiesPrototype::createStructure(vm, &globalObject, globalObject.objectPrototype()));
}

JSObject* JSTestNamedSetterWithLegacyUnforgeableProperties::prototype(VM& vm, JSDOMGlobalObject& globalObject)
{
    return getDOMPrototype<JSTestNamedSetterWithLegacyUnforgeableProperties>(vm, globalObject);
}

JSValue JSTestNamedSetterWithLegacyUnforgeableProperties::getConstructor(VM& vm, const JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSTestNamedSetterWithLegacyUnforgeablePropertiesDOMConstructor, DOMConstructorID::TestNamedSetterWithLegacyUnforgeableProperties>(vm, *jsCast<const JSDOMGlobalObject*>(globalObject));
}

void JSTestNamedSetterWithLegacyUnforgeableProperties::destroy(JSC::JSCell* cell)
{
    JSTestNamedSetterWithLegacyUnforgeableProperties* thisObject = static_cast<JSTestNamedSetterWithLegacyUnforgeableProperties*>(cell);
    thisObject->JSTestNamedSetterWithLegacyUnforgeableProperties::~JSTestNamedSetterWithLegacyUnforgeableProperties();
}

bool JSTestNamedSetterWithLegacyUnforgeableProperties::getOwnPropertySlot(JSObject* object, JSGlobalObject* lexicalGlobalObject, PropertyName propertyName, PropertySlot& slot)
{
    auto throwScope = DECLARE_THROW_SCOPE(JSC::getVM(lexicalGlobalObject));
    auto* thisObject = jsCast<JSTestNamedSetterWithLegacyUnforgeableProperties*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, info());
    using GetterIDLType = IDLDOMString;
    auto getterFunctor = visibleNamedPropertyItemAccessorFunctor<GetterIDLType, JSTestNamedSetterWithLegacyUnforgeableProperties>([] (JSTestNamedSetterWithLegacyUnforgeableProperties& thisObject, PropertyName propertyName) -> decltype(auto) {
        return thisObject.wrapped().namedItem(propertyNameToAtomString(propertyName));
    });
    if (auto namedProperty = accessVisibleNamedProperty<LegacyOverrideBuiltIns::No>(*lexicalGlobalObject, *thisObject, propertyName, getterFunctor)) {
        auto value = toJS<IDLDOMString>(*lexicalGlobalObject, throwScope, WTFMove(namedProperty.value()));
        RETURN_IF_EXCEPTION(throwScope, false);
        slot.setValue(thisObject, static_cast<unsigned>(0), value);
        return true;
    }
    return JSObject::getOwnPropertySlot(object, lexicalGlobalObject, propertyName, slot);
}

bool JSTestNamedSetterWithLegacyUnforgeableProperties::getOwnPropertySlotByIndex(JSObject* object, JSGlobalObject* lexicalGlobalObject, unsigned index, PropertySlot& slot)
{
    VM& vm = JSC::getVM(lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    auto* thisObject = jsCast<JSTestNamedSetterWithLegacyUnforgeableProperties*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, info());
    auto propertyName = Identifier::from(vm, index);
    using GetterIDLType = IDLDOMString;
    auto getterFunctor = visibleNamedPropertyItemAccessorFunctor<GetterIDLType, JSTestNamedSetterWithLegacyUnforgeableProperties>([] (JSTestNamedSetterWithLegacyUnforgeableProperties& thisObject, PropertyName propertyName) -> decltype(auto) {
        return thisObject.wrapped().namedItem(propertyNameToAtomString(propertyName));
    });
    if (auto namedProperty = accessVisibleNamedProperty<LegacyOverrideBuiltIns::No>(*lexicalGlobalObject, *thisObject, propertyName, getterFunctor)) {
        auto value = toJS<IDLDOMString>(*lexicalGlobalObject, throwScope, WTFMove(namedProperty.value()));
        RETURN_IF_EXCEPTION(throwScope, false);
        slot.setValue(thisObject, static_cast<unsigned>(0), value);
        return true;
    }
    return JSObject::getOwnPropertySlotByIndex(object, lexicalGlobalObject, index, slot);
}

void JSTestNamedSetterWithLegacyUnforgeableProperties::getOwnPropertyNames(JSObject* object, JSGlobalObject* lexicalGlobalObject, PropertyNameArray& propertyNames, DontEnumPropertiesMode mode)
{
    VM& vm = JSC::getVM(lexicalGlobalObject);
    auto* thisObject = jsCast<JSTestNamedSetterWithLegacyUnforgeableProperties*>(object);
    ASSERT_GC_OBJECT_INHERITS(object, info());
    for (auto& propertyName : thisObject->wrapped().supportedPropertyNames())
        propertyNames.add(Identifier::fromString(vm, propertyName));
    JSObject::getOwnPropertyNames(object, lexicalGlobalObject, propertyNames, mode);
}

bool JSTestNamedSetterWithLegacyUnforgeableProperties::put(JSCell* cell, JSGlobalObject* lexicalGlobalObject, PropertyName propertyName, JSValue value, PutPropertySlot& putPropertySlot)
{
    auto* thisObject = jsCast<JSTestNamedSetterWithLegacyUnforgeableProperties*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, info());

    if (UNLIKELY(thisObject != putPropertySlot.thisValue()))
        return JSObject::put(thisObject, lexicalGlobalObject, propertyName, value, putPropertySlot);
    auto throwScope = DECLARE_THROW_SCOPE(lexicalGlobalObject->vm());

    if (!propertyName.isSymbol()) {
        PropertySlot slot { thisObject, PropertySlot::InternalMethodType::VMInquiry, &lexicalGlobalObject->vm() };
        JSValue prototype = thisObject->getPrototypeDirect();
        bool found = prototype.isObject() && asObject(prototype)->getPropertySlot(lexicalGlobalObject, propertyName, slot);
        slot.disallowVMEntry.reset();
        RETURN_IF_EXCEPTION(throwScope, false);
        if (!found) {
            auto nativeValue = convert<IDLDOMString>(*lexicalGlobalObject, value);
            RETURN_IF_EXCEPTION(throwScope, true);
            invokeFunctorPropagatingExceptionIfNecessary(*lexicalGlobalObject, throwScope, [&] { return thisObject->wrapped().setNamedItem(propertyNameToString(propertyName), WTFMove(nativeValue)); });
            return true;
        }
    }

    throwScope.assertNoException();
    RELEASE_AND_RETURN(throwScope, JSObject::put(thisObject, lexicalGlobalObject, propertyName, value, putPropertySlot));
}

bool JSTestNamedSetterWithLegacyUnforgeableProperties::putByIndex(JSCell* cell, JSGlobalObject* lexicalGlobalObject, unsigned index, JSValue value, bool shouldThrow)
{
    auto* thisObject = jsCast<JSTestNamedSetterWithLegacyUnforgeableProperties*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, info());

    VM& vm = JSC::getVM(lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);

    auto propertyName = Identifier::from(vm, index);
    PropertySlot slot { thisObject, PropertySlot::InternalMethodType::VMInquiry, &vm };
    JSValue prototype = thisObject->getPrototypeDirect();
    bool found = prototype.isObject() && asObject(prototype)->getPropertySlot(lexicalGlobalObject, propertyName, slot);
    slot.disallowVMEntry.reset();
    RETURN_IF_EXCEPTION(throwScope, false);
    if (!found) {
        auto nativeValue = convert<IDLDOMString>(*lexicalGlobalObject, value);
        RETURN_IF_EXCEPTION(throwScope, true);
        invokeFunctorPropagatingExceptionIfNecessary(*lexicalGlobalObject, throwScope, [&] { return thisObject->wrapped().setNamedItem(propertyNameToString(propertyName), WTFMove(nativeValue)); });
        return true;
    }

    throwScope.assertNoException();
    RELEASE_AND_RETURN(throwScope, JSObject::putByIndex(cell, lexicalGlobalObject, index, value, shouldThrow));
}

bool JSTestNamedSetterWithLegacyUnforgeableProperties::defineOwnProperty(JSObject* object, JSGlobalObject* lexicalGlobalObject, PropertyName propertyName, const PropertyDescriptor& propertyDescriptor, bool shouldThrow)
{
    auto* thisObject = jsCast<JSTestNamedSetterWithLegacyUnforgeableProperties*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, info());

    auto throwScope = DECLARE_THROW_SCOPE(lexicalGlobalObject->vm());

    if (!propertyName.isSymbol()) {
static bool isLegacyUnforgeablePropertyName(PropertyName propertyName)
{
    return propertyName == "unforgeableAttribute"_s || propertyName == "unforgeableOperation"_s;
}

        if (!isLegacyUnforgeablePropertyName(propertyName)) {
            PropertySlot slot { thisObject, PropertySlot::InternalMethodType::VMInquiry, &lexicalGlobalObject->vm() };
            bool found = JSObject::getOwnPropertySlot(thisObject, lexicalGlobalObject, propertyName, slot);
            slot.disallowVMEntry.reset();
            RETURN_IF_EXCEPTION(throwScope, false);
            if (!found) {
                if (!propertyDescriptor.isDataDescriptor())
                    return false;
                auto nativeValue = convert<IDLDOMString>(*lexicalGlobalObject, propertyDescriptor.value());
                RETURN_IF_EXCEPTION(throwScope, true);
                invokeFunctorPropagatingExceptionIfNecessary(*lexicalGlobalObject, throwScope, [&] { return thisObject->wrapped().setNamedItem(propertyNameToString(propertyName), WTFMove(nativeValue)); });
                return true;
            }
        }
    }

    PropertyDescriptor newPropertyDescriptor = propertyDescriptor;
    newPropertyDescriptor.setConfigurable(true);
    throwScope.release();
    return JSObject::defineOwnProperty(object, lexicalGlobalObject, propertyName, newPropertyDescriptor, shouldThrow);
}

JSC_DEFINE_CUSTOM_GETTER(jsTestNamedSetterWithLegacyUnforgeablePropertiesConstructor, (JSGlobalObject* lexicalGlobalObject, EncodedJSValue thisValue, PropertyName))
{
    VM& vm = JSC::getVM(lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    auto* prototype = jsDynamicCast<JSTestNamedSetterWithLegacyUnforgeablePropertiesPrototype*>(JSValue::decode(thisValue));
    if (UNLIKELY(!prototype))
        return throwVMTypeError(lexicalGlobalObject, throwScope);
    return JSValue::encode(JSTestNamedSetterWithLegacyUnforgeableProperties::getConstructor(JSC::getVM(lexicalGlobalObject), prototype->globalObject()));
}

static inline JSValue jsTestNamedSetterWithLegacyUnforgeableProperties_unforgeableAttributeGetter(JSGlobalObject& lexicalGlobalObject, JSTestNamedSetterWithLegacyUnforgeableProperties& thisObject)
{
    auto& vm = JSC::getVM(&lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    auto& impl = thisObject.wrapped();
    RELEASE_AND_RETURN(throwScope, (toJS<IDLDOMString>(lexicalGlobalObject, throwScope, impl.unforgeableAttribute())));
}

JSC_DEFINE_CUSTOM_GETTER(jsTestNamedSetterWithLegacyUnforgeableProperties_unforgeableAttribute, (JSGlobalObject* lexicalGlobalObject, EncodedJSValue thisValue, PropertyName attributeName))
{
    return IDLAttribute<JSTestNamedSetterWithLegacyUnforgeableProperties>::get<jsTestNamedSetterWithLegacyUnforgeableProperties_unforgeableAttributeGetter, CastedThisErrorBehavior::Assert>(*lexicalGlobalObject, thisValue, attributeName);
}

static inline JSC::EncodedJSValue jsTestNamedSetterWithLegacyUnforgeablePropertiesInstanceFunction_unforgeableOperationBody(JSC::JSGlobalObject* lexicalGlobalObject, JSC::CallFrame* callFrame, typename IDLOperation<JSTestNamedSetterWithLegacyUnforgeableProperties>::ClassParameter castedThis)
{
    auto& vm = JSC::getVM(lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    UNUSED_PARAM(throwScope);
    UNUSED_PARAM(callFrame);
    auto& impl = castedThis->wrapped();
    RELEASE_AND_RETURN(throwScope, JSValue::encode(toJS<IDLUndefined>(*lexicalGlobalObject, throwScope, [&]() -> decltype(auto) { return impl.unforgeableOperation(); })));
}

JSC_DEFINE_HOST_FUNCTION(jsTestNamedSetterWithLegacyUnforgeablePropertiesInstanceFunction_unforgeableOperation, (JSGlobalObject* lexicalGlobalObject, CallFrame* callFrame))
{
    return IDLOperation<JSTestNamedSetterWithLegacyUnforgeableProperties>::call<jsTestNamedSetterWithLegacyUnforgeablePropertiesInstanceFunction_unforgeableOperationBody>(*lexicalGlobalObject, *callFrame, "unforgeableOperation");
}

JSC::GCClient::IsoSubspace* JSTestNamedSetterWithLegacyUnforgeableProperties::subspaceForImpl(JSC::VM& vm)
{
    return WebCore::subspaceForImpl<JSTestNamedSetterWithLegacyUnforgeableProperties, UseCustomHeapCellType::No>(vm,
        [] (auto& spaces) { return spaces.m_clientSubspaceForTestNamedSetterWithLegacyUnforgeableProperties.get(); },
        [] (auto& spaces, auto&& space) { spaces.m_clientSubspaceForTestNamedSetterWithLegacyUnforgeableProperties = WTFMove(space); },
        [] (auto& spaces) { return spaces.m_subspaceForTestNamedSetterWithLegacyUnforgeableProperties.get(); },
        [] (auto& spaces, auto&& space) { spaces.m_subspaceForTestNamedSetterWithLegacyUnforgeableProperties = WTFMove(space); }
    );
}

void JSTestNamedSetterWithLegacyUnforgeableProperties::analyzeHeap(JSCell* cell, HeapAnalyzer& analyzer)
{
    auto* thisObject = jsCast<JSTestNamedSetterWithLegacyUnforgeableProperties*>(cell);
    analyzer.setWrappedObjectForCell(cell, &thisObject->wrapped());
    if (thisObject->scriptExecutionContext())
        analyzer.setLabelForCell(cell, "url " + thisObject->scriptExecutionContext()->url().string());
    Base::analyzeHeap(cell, analyzer);
}

bool JSTestNamedSetterWithLegacyUnforgeablePropertiesOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, AbstractSlotVisitor& visitor, const char** reason)
{
    UNUSED_PARAM(handle);
    UNUSED_PARAM(visitor);
    UNUSED_PARAM(reason);
    return false;
}

void JSTestNamedSetterWithLegacyUnforgeablePropertiesOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    auto* jsTestNamedSetterWithLegacyUnforgeableProperties = static_cast<JSTestNamedSetterWithLegacyUnforgeableProperties*>(handle.slot()->asCell());
    auto& world = *static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, &jsTestNamedSetterWithLegacyUnforgeableProperties->wrapped(), jsTestNamedSetterWithLegacyUnforgeableProperties);
}

#if ENABLE(BINDING_INTEGRITY)
#if PLATFORM(WIN)
#pragma warning(disable: 4483)
extern "C" { extern void (*const __identifier("??_7TestNamedSetterWithLegacyUnforgeableProperties@WebCore@@6B@")[])(); }
#else
extern "C" { extern void* _ZTVN7WebCore46TestNamedSetterWithLegacyUnforgeablePropertiesE[]; }
#endif
#endif

JSC::JSValue toJSNewlyCreated(JSC::JSGlobalObject*, JSDOMGlobalObject* globalObject, Ref<TestNamedSetterWithLegacyUnforgeableProperties>&& impl)
{

    if constexpr (std::is_polymorphic_v<TestNamedSetterWithLegacyUnforgeableProperties>) {
#if ENABLE(BINDING_INTEGRITY)
        const void* actualVTablePointer = getVTablePointer(impl.ptr());
#if PLATFORM(WIN)
        void* expectedVTablePointer = __identifier("??_7TestNamedSetterWithLegacyUnforgeableProperties@WebCore@@6B@");
#else
        void* expectedVTablePointer = &_ZTVN7WebCore46TestNamedSetterWithLegacyUnforgeablePropertiesE[2];
#endif

        // If you hit this assertion you either have a use after free bug, or
        // TestNamedSetterWithLegacyUnforgeableProperties has subclasses. If TestNamedSetterWithLegacyUnforgeableProperties has subclasses that get passed
        // to toJS() we currently require TestNamedSetterWithLegacyUnforgeableProperties you to opt out of binding hardening
        // by adding the SkipVTableValidation attribute to the interface IDL definition
        RELEASE_ASSERT(actualVTablePointer == expectedVTablePointer);
#endif
    }
    return createWrapper<TestNamedSetterWithLegacyUnforgeableProperties>(globalObject, WTFMove(impl));
}

JSC::JSValue toJS(JSC::JSGlobalObject* lexicalGlobalObject, JSDOMGlobalObject* globalObject, TestNamedSetterWithLegacyUnforgeableProperties& impl)
{
    return wrap(lexicalGlobalObject, globalObject, impl);
}

TestNamedSetterWithLegacyUnforgeableProperties* JSTestNamedSetterWithLegacyUnforgeableProperties::toWrapped(JSC::VM&, JSC::JSValue value)
{
    if (auto* wrapper = jsDynamicCast<JSTestNamedSetterWithLegacyUnforgeableProperties*>(value))
        return &wrapper->wrapped();
    return nullptr;
}

}
