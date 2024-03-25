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
#include "JSTestPluginInterface.h"

#include "ActiveDOMObject.h"
#include "Document.h"
#include "DocumentInlines.h"
#include "ExtendedDOMClientIsoSubspaces.h"
#include "ExtendedDOMIsoSubspaces.h"
#include "JSDOMBinding.h"
#include "JSDOMConstructorNotConstructable.h"
#include "JSDOMExceptionHandling.h"
#include "JSDOMGlobalObjectInlines.h"
#include "JSDOMWrapperCache.h"
#include "JSPluginElementFunctions.h"
#include "Quirks.h"
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

// Attributes

static JSC_DECLARE_CUSTOM_GETTER(jsTestPluginInterfaceConstructor);

class JSTestPluginInterfacePrototype final : public JSC::JSNonFinalObject {
public:
    using Base = JSC::JSNonFinalObject;
    static JSTestPluginInterfacePrototype* create(JSC::VM& vm, JSDOMGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSTestPluginInterfacePrototype* ptr = new (NotNull, JSC::allocateCell<JSTestPluginInterfacePrototype>(vm)) JSTestPluginInterfacePrototype(vm, globalObject, structure);
        ptr->finishCreation(vm);
        return ptr;
    }

    DECLARE_INFO;
    template<typename CellType, JSC::SubspaceAccess>
    static JSC::GCClient::IsoSubspace* subspaceFor(JSC::VM& vm)
    {
        STATIC_ASSERT_ISO_SUBSPACE_SHARABLE(JSTestPluginInterfacePrototype, Base);
        return &vm.plainObjectSpace();
    }
    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

private:
    JSTestPluginInterfacePrototype(JSC::VM& vm, JSC::JSGlobalObject*, JSC::Structure* structure)
        : JSC::JSNonFinalObject(vm, structure)
    {
    }

    void finishCreation(JSC::VM&);
};
STATIC_ASSERT_ISO_SUBSPACE_SHARABLE(JSTestPluginInterfacePrototype, JSTestPluginInterfacePrototype::Base);

using JSTestPluginInterfaceDOMConstructor = JSDOMConstructorNotConstructable<JSTestPluginInterface>;

template<> const ClassInfo JSTestPluginInterfaceDOMConstructor::s_info = { "TestPluginInterface"_s, &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestPluginInterfaceDOMConstructor) };

template<> JSValue JSTestPluginInterfaceDOMConstructor::prototypeForStructure(JSC::VM& vm, const JSDOMGlobalObject& globalObject)
{
    UNUSED_PARAM(vm);
    return globalObject.functionPrototype();
}

template<> void JSTestPluginInterfaceDOMConstructor::initializeProperties(VM& vm, JSDOMGlobalObject& globalObject)
{
    putDirect(vm, vm.propertyNames->length, jsNumber(0), JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum);
    JSString* nameString = jsNontrivialString(vm, "TestPluginInterface"_s);
    m_originalName.set(vm, this, nameString);
    putDirect(vm, vm.propertyNames->name, nameString, JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum);
    putDirect(vm, vm.propertyNames->prototype, JSTestPluginInterface::prototype(vm, globalObject), JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum | JSC::PropertyAttribute::DontDelete);
}

/* Hash table for prototype */

static const HashTableValue JSTestPluginInterfacePrototypeTableValues[] =
{
    { "constructor"_s, static_cast<unsigned>(PropertyAttribute::DontEnum), NoIntrinsic, { HashTableValue::GetterSetterType, jsTestPluginInterfaceConstructor, 0 } },
};

const ClassInfo JSTestPluginInterfacePrototype::s_info = { "TestPluginInterface"_s, &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestPluginInterfacePrototype) };

void JSTestPluginInterfacePrototype::finishCreation(VM& vm)
{
    Base::finishCreation(vm);
    reifyStaticProperties(vm, JSTestPluginInterface::info(), JSTestPluginInterfacePrototypeTableValues, *this);
    JSC_TO_STRING_TAG_WITHOUT_TRANSITION();
}

const ClassInfo JSTestPluginInterface::s_info = { "TestPluginInterface"_s, &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestPluginInterface) };

JSTestPluginInterface::JSTestPluginInterface(Structure* structure, JSDOMGlobalObject& globalObject, Ref<TestPluginInterface>&& impl)
    : JSDOMWrapper<TestPluginInterface>(structure, globalObject, WTFMove(impl))
{
}

static_assert(!std::is_base_of<ActiveDOMObject, TestPluginInterface>::value, "Interface is not marked as [ActiveDOMObject] even though implementation class subclasses ActiveDOMObject.");

JSObject* JSTestPluginInterface::createPrototype(VM& vm, JSDOMGlobalObject& globalObject)
{
    auto* structure = JSTestPluginInterfacePrototype::createStructure(vm, &globalObject, globalObject.objectPrototype());
    structure->setMayBePrototype(true);
    return JSTestPluginInterfacePrototype::create(vm, &globalObject, structure);
}

JSObject* JSTestPluginInterface::prototype(VM& vm, JSDOMGlobalObject& globalObject)
{
    return getDOMPrototype<JSTestPluginInterface>(vm, globalObject);
}

JSValue JSTestPluginInterface::getConstructor(VM& vm, const JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSTestPluginInterfaceDOMConstructor, DOMConstructorID::TestPluginInterface>(vm, *jsCast<const JSDOMGlobalObject*>(globalObject));
}

void JSTestPluginInterface::destroy(JSC::JSCell* cell)
{
    JSTestPluginInterface* thisObject = static_cast<JSTestPluginInterface*>(cell);
    thisObject->JSTestPluginInterface::~JSTestPluginInterface();
}

bool JSTestPluginInterface::legacyPlatformObjectGetOwnProperty(JSObject* object, JSGlobalObject* lexicalGlobalObject, PropertyName propertyName, PropertySlot& slot, bool ignoreNamedProperties)
{
    UNUSED_PARAM(ignoreNamedProperties);
    auto* thisObject = jsCast<JSTestPluginInterface*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, info());
    if (pluginElementCustomGetOwnPropertySlot(thisObject, lexicalGlobalObject, propertyName, slot))
        return true;
    ASSERT(slot.isTaintedByOpaqueObject());
    if (slot.isVMInquiry())
        return false;
    return JSObject::getOwnPropertySlot(object, lexicalGlobalObject, propertyName, slot);
}

bool JSTestPluginInterface::getOwnPropertySlot(JSObject* object, JSGlobalObject* lexicalGlobalObject, PropertyName propertyName, PropertySlot& slot)
{
    bool ignoreNamedProperties = false;
    return legacyPlatformObjectGetOwnProperty(object, lexicalGlobalObject, propertyName, slot, ignoreNamedProperties);
}

bool JSTestPluginInterface::getOwnPropertySlotByIndex(JSObject* object, JSGlobalObject* lexicalGlobalObject, unsigned index, PropertySlot& slot)
{
    VM& vm = JSC::getVM(lexicalGlobalObject);
    auto* thisObject = jsCast<JSTestPluginInterface*>(object);
    ASSERT_GC_OBJECT_INHERITS(thisObject, info());
    auto propertyName = Identifier::from(vm, index);
    if (pluginElementCustomGetOwnPropertySlot(thisObject, lexicalGlobalObject, propertyName, slot))
        return true;
    ASSERT(slot.isTaintedByOpaqueObject());
    if (slot.isVMInquiry())
        return false;
    return JSObject::getOwnPropertySlotByIndex(object, lexicalGlobalObject, index, slot);
}

bool JSTestPluginInterface::put(JSCell* cell, JSGlobalObject* lexicalGlobalObject, PropertyName propertyName, JSValue value, PutPropertySlot& putPropertySlot)
{
    auto* thisObject = jsCast<JSTestPluginInterface*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, info());

    if (UNLIKELY(thisObject != putPropertySlot.thisValue()))
        return JSObject::put(thisObject, lexicalGlobalObject, propertyName, value, putPropertySlot);

    // Temporary quirk for ungap/@custom-elements polyfill (rdar://problem/111008826), consider removing in 2025.
    if (auto* document = dynamicDowncast<Document>(jsDynamicCast<JSDOMGlobalObject*>(lexicalGlobalObject)->scriptExecutionContext())) {
        if (UNLIKELY(document->quirks().needsConfigurableIndexedPropertiesQuirk()))
            return JSObject::put(thisObject, lexicalGlobalObject, propertyName, value, putPropertySlot);
    }

    auto throwScope = DECLARE_THROW_SCOPE(lexicalGlobalObject->vm());

    bool putResult = false;
    bool success = pluginElementCustomPut(thisObject, lexicalGlobalObject, propertyName, value, putPropertySlot, putResult);
    RETURN_IF_EXCEPTION(throwScope, false);
    if (success)
        return putResult;

    throwScope.assertNoException();
    RELEASE_AND_RETURN(throwScope, JSObject::put(thisObject, lexicalGlobalObject, propertyName, value, putPropertySlot));
}

bool JSTestPluginInterface::putByIndex(JSCell* cell, JSGlobalObject* lexicalGlobalObject, unsigned index, JSValue value, bool shouldThrow)
{

    // Temporary quirk for ungap/@custom-elements polyfill (rdar://problem/111008826), consider removing in 2025.
    if (auto* document = dynamicDowncast<Document>(jsDynamicCast<JSDOMGlobalObject*>(lexicalGlobalObject)->scriptExecutionContext())) {
        if (UNLIKELY(document->quirks().needsConfigurableIndexedPropertiesQuirk()))
            return JSObject::putByIndex(cell, lexicalGlobalObject, index, value, shouldThrow);
    }

    auto* thisObject = jsCast<JSTestPluginInterface*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, info());

    VM& vm = JSC::getVM(lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);

    auto propertyName = Identifier::from(vm, index);
    PutPropertySlot putPropertySlot(thisObject, shouldThrow);
    bool putResult = false;
    bool success = pluginElementCustomPut(thisObject, lexicalGlobalObject, propertyName, value, putPropertySlot, putResult);
    RETURN_IF_EXCEPTION(throwScope, false);
    if (success)
        return putResult;

    throwScope.assertNoException();
    RELEASE_AND_RETURN(throwScope, JSObject::putByIndex(cell, lexicalGlobalObject, index, value, shouldThrow));
}

CallData JSTestPluginInterface::getCallData(JSCell* cell)
{
    auto* thisObject = jsCast<JSTestPluginInterface*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, info());

    return pluginElementCustomGetCallData(thisObject);
}

JSC_DEFINE_CUSTOM_GETTER(jsTestPluginInterfaceConstructor, (JSGlobalObject* lexicalGlobalObject, EncodedJSValue thisValue, PropertyName))
{
    VM& vm = JSC::getVM(lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    auto* prototype = jsDynamicCast<JSTestPluginInterfacePrototype*>(JSValue::decode(thisValue));
    if (UNLIKELY(!prototype))
        return throwVMTypeError(lexicalGlobalObject, throwScope);
    return JSValue::encode(JSTestPluginInterface::getConstructor(JSC::getVM(lexicalGlobalObject), prototype->globalObject()));
}

JSC::GCClient::IsoSubspace* JSTestPluginInterface::subspaceForImpl(JSC::VM& vm)
{
    return WebCore::subspaceForImpl<JSTestPluginInterface, UseCustomHeapCellType::No>(vm,
        [] (auto& spaces) { return spaces.m_clientSubspaceForTestPluginInterface.get(); },
        [] (auto& spaces, auto&& space) { spaces.m_clientSubspaceForTestPluginInterface = std::forward<decltype(space)>(space); },
        [] (auto& spaces) { return spaces.m_subspaceForTestPluginInterface.get(); },
        [] (auto& spaces, auto&& space) { spaces.m_subspaceForTestPluginInterface = std::forward<decltype(space)>(space); }
    );
}

template<typename Visitor>
void JSTestPluginInterface::visitChildrenImpl(JSCell* cell, Visitor& visitor)
{
    auto* thisObject = jsCast<JSTestPluginInterface*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, info());
    Base::visitChildren(thisObject, visitor);
}

DEFINE_VISIT_CHILDREN(JSTestPluginInterface);

void JSTestPluginInterface::analyzeHeap(JSCell* cell, HeapAnalyzer& analyzer)
{
    auto* thisObject = jsCast<JSTestPluginInterface*>(cell);
    analyzer.setWrappedObjectForCell(cell, &thisObject->wrapped());
    if (thisObject->scriptExecutionContext())
        analyzer.setLabelForCell(cell, "url "_s + thisObject->scriptExecutionContext()->url().string());
    Base::analyzeHeap(cell, analyzer);
}

bool JSTestPluginInterfaceOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, AbstractSlotVisitor& visitor, const char** reason)
{
    UNUSED_PARAM(handle);
    UNUSED_PARAM(visitor);
    UNUSED_PARAM(reason);
    return false;
}

void JSTestPluginInterfaceOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    auto* jsTestPluginInterface = static_cast<JSTestPluginInterface*>(handle.slot()->asCell());
    auto& world = *static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, &jsTestPluginInterface->wrapped(), jsTestPluginInterface);
}

#if ENABLE(BINDING_INTEGRITY)
#if PLATFORM(WIN)
#pragma warning(disable: 4483)
extern "C" { extern void (*const __identifier("??_7TestPluginInterface@WebCore@@6B@")[])(); }
#else
extern "C" { extern void* _ZTVN7WebCore19TestPluginInterfaceE[]; }
#endif
#endif

JSC::JSValue toJSNewlyCreated(JSC::JSGlobalObject*, JSDOMGlobalObject* globalObject, Ref<TestPluginInterface>&& impl)
{

    if constexpr (std::is_polymorphic_v<TestPluginInterface>) {
#if ENABLE(BINDING_INTEGRITY)
        const void* actualVTablePointer = getVTablePointer(impl.ptr());
#if PLATFORM(WIN)
        void* expectedVTablePointer = __identifier("??_7TestPluginInterface@WebCore@@6B@");
#else
        void* expectedVTablePointer = &_ZTVN7WebCore19TestPluginInterfaceE[2];
#endif

        // If you hit this assertion you either have a use after free bug, or
        // TestPluginInterface has subclasses. If TestPluginInterface has subclasses that get passed
        // to toJS() we currently require TestPluginInterface you to opt out of binding hardening
        // by adding the SkipVTableValidation attribute to the interface IDL definition
        RELEASE_ASSERT(actualVTablePointer == expectedVTablePointer);
#endif
    }
    return createWrapper<TestPluginInterface>(globalObject, WTFMove(impl));
}

JSC::JSValue toJS(JSC::JSGlobalObject* lexicalGlobalObject, JSDOMGlobalObject* globalObject, TestPluginInterface& impl)
{
    return wrap(lexicalGlobalObject, globalObject, impl);
}

TestPluginInterface* JSTestPluginInterface::toWrapped(JSC::VM&, JSC::JSValue value)
{
    if (auto* wrapper = jsDynamicCast<JSTestPluginInterface*>(value))
        return &wrapper->wrapped();
    return nullptr;
}

}
