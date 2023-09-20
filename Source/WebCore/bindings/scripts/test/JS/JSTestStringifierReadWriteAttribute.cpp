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
#include "JSTestStringifierReadWriteAttribute.h"

#include "ActiveDOMObject.h"
#include "ExtendedDOMClientIsoSubspaces.h"
#include "ExtendedDOMIsoSubspaces.h"
#include "JSDOMAttribute.h"
#include "JSDOMBinding.h"
#include "JSDOMConstructorNotConstructable.h"
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

static JSC_DECLARE_HOST_FUNCTION(jsTestStringifierReadWriteAttributePrototypeFunction_toString);

// Attributes

static JSC_DECLARE_CUSTOM_GETTER(jsTestStringifierReadWriteAttributeConstructor);
static JSC_DECLARE_CUSTOM_GETTER(jsTestStringifierReadWriteAttribute_identifier);
static JSC_DECLARE_CUSTOM_SETTER(setJSTestStringifierReadWriteAttribute_identifier);

class JSTestStringifierReadWriteAttributePrototype final : public JSC::JSNonFinalObject {
public:
    using Base = JSC::JSNonFinalObject;
    static JSTestStringifierReadWriteAttributePrototype* create(JSC::VM& vm, JSDOMGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSTestStringifierReadWriteAttributePrototype* ptr = new (NotNull, JSC::allocateCell<JSTestStringifierReadWriteAttributePrototype>(vm)) JSTestStringifierReadWriteAttributePrototype(vm, globalObject, structure);
        ptr->finishCreation(vm);
        return ptr;
    }

    DECLARE_INFO;
    template<typename CellType, JSC::SubspaceAccess>
    static JSC::GCClient::IsoSubspace* subspaceFor(JSC::VM& vm)
    {
        STATIC_ASSERT_ISO_SUBSPACE_SHARABLE(JSTestStringifierReadWriteAttributePrototype, Base);
        return &vm.plainObjectSpace();
    }
    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

private:
    JSTestStringifierReadWriteAttributePrototype(JSC::VM& vm, JSC::JSGlobalObject*, JSC::Structure* structure)
        : JSC::JSNonFinalObject(vm, structure)
    {
    }

    void finishCreation(JSC::VM&);
};
STATIC_ASSERT_ISO_SUBSPACE_SHARABLE(JSTestStringifierReadWriteAttributePrototype, JSTestStringifierReadWriteAttributePrototype::Base);

using JSTestStringifierReadWriteAttributeDOMConstructor = JSDOMConstructorNotConstructable<JSTestStringifierReadWriteAttribute>;

template<> const ClassInfo JSTestStringifierReadWriteAttributeDOMConstructor::s_info = { "TestStringifierReadWriteAttribute"_s, &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestStringifierReadWriteAttributeDOMConstructor) };

template<> JSValue JSTestStringifierReadWriteAttributeDOMConstructor::prototypeForStructure(JSC::VM& vm, const JSDOMGlobalObject& globalObject)
{
    UNUSED_PARAM(vm);
    return globalObject.functionPrototype();
}

template<> void JSTestStringifierReadWriteAttributeDOMConstructor::initializeProperties(VM& vm, JSDOMGlobalObject& globalObject)
{
    putDirect(vm, vm.propertyNames->length, jsNumber(0), JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum);
    JSString* nameString = jsNontrivialString(vm, "TestStringifierReadWriteAttribute"_s);
    m_originalName.set(vm, this, nameString);
    putDirect(vm, vm.propertyNames->name, nameString, JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum);
    putDirect(vm, vm.propertyNames->prototype, JSTestStringifierReadWriteAttribute::prototype(vm, globalObject), JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum | JSC::PropertyAttribute::DontDelete);
}

/* Hash table for prototype */

static const HashTableValue JSTestStringifierReadWriteAttributePrototypeTableValues[] =
{
    { "constructor"_s, static_cast<unsigned>(PropertyAttribute::DontEnum), NoIntrinsic, { HashTableValue::GetterSetterType, jsTestStringifierReadWriteAttributeConstructor, 0 } },
    { "identifier"_s, JSC::PropertyAttribute::CustomAccessor | JSC::PropertyAttribute::DOMAttribute, NoIntrinsic, { HashTableValue::GetterSetterType, jsTestStringifierReadWriteAttribute_identifier, setJSTestStringifierReadWriteAttribute_identifier } },
    { "toString"_s, static_cast<unsigned>(JSC::PropertyAttribute::Function), NoIntrinsic, { HashTableValue::NativeFunctionType, jsTestStringifierReadWriteAttributePrototypeFunction_toString, 0 } },
};

const ClassInfo JSTestStringifierReadWriteAttributePrototype::s_info = { "TestStringifierReadWriteAttribute"_s, &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestStringifierReadWriteAttributePrototype) };

void JSTestStringifierReadWriteAttributePrototype::finishCreation(VM& vm)
{
    Base::finishCreation(vm);
    reifyStaticProperties(vm, JSTestStringifierReadWriteAttribute::info(), JSTestStringifierReadWriteAttributePrototypeTableValues, *this);
    JSC_TO_STRING_TAG_WITHOUT_TRANSITION();
}

const ClassInfo JSTestStringifierReadWriteAttribute::s_info = { "TestStringifierReadWriteAttribute"_s, &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestStringifierReadWriteAttribute) };

JSTestStringifierReadWriteAttribute::JSTestStringifierReadWriteAttribute(Structure* structure, JSDOMGlobalObject& globalObject, Ref<TestStringifierReadWriteAttribute>&& impl)
    : JSDOMWrapper<TestStringifierReadWriteAttribute>(structure, globalObject, WTFMove(impl))
{
}

static_assert(!std::is_base_of<ActiveDOMObject, TestStringifierReadWriteAttribute>::value, "Interface is not marked as [ActiveDOMObject] even though implementation class subclasses ActiveDOMObject.");

JSObject* JSTestStringifierReadWriteAttribute::createPrototype(VM& vm, JSDOMGlobalObject& globalObject)
{
    auto* structure = JSTestStringifierReadWriteAttributePrototype::createStructure(vm, &globalObject, globalObject.objectPrototype());
    structure->setMayBePrototype(true);
    return JSTestStringifierReadWriteAttributePrototype::create(vm, &globalObject, structure);
}

JSObject* JSTestStringifierReadWriteAttribute::prototype(VM& vm, JSDOMGlobalObject& globalObject)
{
    return getDOMPrototype<JSTestStringifierReadWriteAttribute>(vm, globalObject);
}

JSValue JSTestStringifierReadWriteAttribute::getConstructor(VM& vm, const JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSTestStringifierReadWriteAttributeDOMConstructor, DOMConstructorID::TestStringifierReadWriteAttribute>(vm, *jsCast<const JSDOMGlobalObject*>(globalObject));
}

void JSTestStringifierReadWriteAttribute::destroy(JSC::JSCell* cell)
{
    JSTestStringifierReadWriteAttribute* thisObject = static_cast<JSTestStringifierReadWriteAttribute*>(cell);
    thisObject->JSTestStringifierReadWriteAttribute::~JSTestStringifierReadWriteAttribute();
}

JSC_DEFINE_CUSTOM_GETTER(jsTestStringifierReadWriteAttributeConstructor, (JSGlobalObject* lexicalGlobalObject, EncodedJSValue thisValue, PropertyName))
{
    VM& vm = JSC::getVM(lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    auto* prototype = jsDynamicCast<JSTestStringifierReadWriteAttributePrototype*>(JSValue::decode(thisValue));
    if (UNLIKELY(!prototype))
        return throwVMTypeError(lexicalGlobalObject, throwScope);
    return JSValue::encode(JSTestStringifierReadWriteAttribute::getConstructor(JSC::getVM(lexicalGlobalObject), prototype->globalObject()));
}

static inline JSValue jsTestStringifierReadWriteAttribute_identifierGetter(JSGlobalObject& lexicalGlobalObject, JSTestStringifierReadWriteAttribute& thisObject)
{
    auto& vm = JSC::getVM(&lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    auto& impl = thisObject.wrapped();
    RELEASE_AND_RETURN(throwScope, (toJS<IDLDOMString>(lexicalGlobalObject, throwScope, impl.identifier())));
}

JSC_DEFINE_CUSTOM_GETTER(jsTestStringifierReadWriteAttribute_identifier, (JSGlobalObject* lexicalGlobalObject, EncodedJSValue thisValue, PropertyName attributeName))
{
    return IDLAttribute<JSTestStringifierReadWriteAttribute>::get<jsTestStringifierReadWriteAttribute_identifierGetter, CastedThisErrorBehavior::Assert>(*lexicalGlobalObject, thisValue, attributeName);
}

static inline bool setJSTestStringifierReadWriteAttribute_identifierSetter(JSGlobalObject& lexicalGlobalObject, JSTestStringifierReadWriteAttribute& thisObject, JSValue value)
{
    auto& vm = JSC::getVM(&lexicalGlobalObject);
    UNUSED_PARAM(vm);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    auto& impl = thisObject.wrapped();
    auto nativeValue = convert<IDLDOMString>(lexicalGlobalObject, value);
    RETURN_IF_EXCEPTION(throwScope, false);
    invokeFunctorPropagatingExceptionIfNecessary(lexicalGlobalObject, throwScope, [&] {
        return impl.setIdentifier(WTFMove(nativeValue));
    });
    return true;
}

JSC_DEFINE_CUSTOM_SETTER(setJSTestStringifierReadWriteAttribute_identifier, (JSGlobalObject* lexicalGlobalObject, EncodedJSValue thisValue, EncodedJSValue encodedValue, PropertyName attributeName))
{
    return IDLAttribute<JSTestStringifierReadWriteAttribute>::set<setJSTestStringifierReadWriteAttribute_identifierSetter>(*lexicalGlobalObject, thisValue, encodedValue, attributeName);
}

static inline JSC::EncodedJSValue jsTestStringifierReadWriteAttributePrototypeFunction_toStringBody(JSC::JSGlobalObject* lexicalGlobalObject, JSC::CallFrame* callFrame, typename IDLOperation<JSTestStringifierReadWriteAttribute>::ClassParameter castedThis)
{
    auto& vm = JSC::getVM(lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    UNUSED_PARAM(throwScope);
    UNUSED_PARAM(callFrame);
    auto& impl = castedThis->wrapped();
    RELEASE_AND_RETURN(throwScope, JSValue::encode(toJS<IDLDOMString>(*lexicalGlobalObject, throwScope, impl.identifier())));
}

JSC_DEFINE_HOST_FUNCTION(jsTestStringifierReadWriteAttributePrototypeFunction_toString, (JSGlobalObject* lexicalGlobalObject, CallFrame* callFrame))
{
    return IDLOperation<JSTestStringifierReadWriteAttribute>::call<jsTestStringifierReadWriteAttributePrototypeFunction_toStringBody>(*lexicalGlobalObject, *callFrame, "toString");
}

JSC::GCClient::IsoSubspace* JSTestStringifierReadWriteAttribute::subspaceForImpl(JSC::VM& vm)
{
    return WebCore::subspaceForImpl<JSTestStringifierReadWriteAttribute, UseCustomHeapCellType::No>(vm,
        [] (auto& spaces) { return spaces.m_clientSubspaceForTestStringifierReadWriteAttribute.get(); },
        [] (auto& spaces, auto&& space) { spaces.m_clientSubspaceForTestStringifierReadWriteAttribute = std::forward<decltype(space)>(space); },
        [] (auto& spaces) { return spaces.m_subspaceForTestStringifierReadWriteAttribute.get(); },
        [] (auto& spaces, auto&& space) { spaces.m_subspaceForTestStringifierReadWriteAttribute = std::forward<decltype(space)>(space); }
    );
}

void JSTestStringifierReadWriteAttribute::analyzeHeap(JSCell* cell, HeapAnalyzer& analyzer)
{
    auto* thisObject = jsCast<JSTestStringifierReadWriteAttribute*>(cell);
    analyzer.setWrappedObjectForCell(cell, &thisObject->wrapped());
    if (thisObject->scriptExecutionContext())
        analyzer.setLabelForCell(cell, "url "_s + thisObject->scriptExecutionContext()->url().string());
    Base::analyzeHeap(cell, analyzer);
}

bool JSTestStringifierReadWriteAttributeOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, AbstractSlotVisitor& visitor, const char** reason)
{
    UNUSED_PARAM(handle);
    UNUSED_PARAM(visitor);
    UNUSED_PARAM(reason);
    return false;
}

void JSTestStringifierReadWriteAttributeOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    auto* jsTestStringifierReadWriteAttribute = static_cast<JSTestStringifierReadWriteAttribute*>(handle.slot()->asCell());
    auto& world = *static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, &jsTestStringifierReadWriteAttribute->wrapped(), jsTestStringifierReadWriteAttribute);
}

#if ENABLE(BINDING_INTEGRITY)
#if PLATFORM(WIN)
#pragma warning(disable: 4483)
extern "C" { extern void (*const __identifier("??_7TestStringifierReadWriteAttribute@WebCore@@6B@")[])(); }
#else
extern "C" { extern void* _ZTVN7WebCore33TestStringifierReadWriteAttributeE[]; }
#endif
#endif

JSC::JSValue toJSNewlyCreated(JSC::JSGlobalObject*, JSDOMGlobalObject* globalObject, Ref<TestStringifierReadWriteAttribute>&& impl)
{

    if constexpr (std::is_polymorphic_v<TestStringifierReadWriteAttribute>) {
#if ENABLE(BINDING_INTEGRITY)
        const void* actualVTablePointer = getVTablePointer(impl.ptr());
#if PLATFORM(WIN)
        void* expectedVTablePointer = __identifier("??_7TestStringifierReadWriteAttribute@WebCore@@6B@");
#else
        void* expectedVTablePointer = &_ZTVN7WebCore33TestStringifierReadWriteAttributeE[2];
#endif

        // If you hit this assertion you either have a use after free bug, or
        // TestStringifierReadWriteAttribute has subclasses. If TestStringifierReadWriteAttribute has subclasses that get passed
        // to toJS() we currently require TestStringifierReadWriteAttribute you to opt out of binding hardening
        // by adding the SkipVTableValidation attribute to the interface IDL definition
        RELEASE_ASSERT(actualVTablePointer == expectedVTablePointer);
#endif
    }
    return createWrapper<TestStringifierReadWriteAttribute>(globalObject, WTFMove(impl));
}

JSC::JSValue toJS(JSC::JSGlobalObject* lexicalGlobalObject, JSDOMGlobalObject* globalObject, TestStringifierReadWriteAttribute& impl)
{
    return wrap(lexicalGlobalObject, globalObject, impl);
}

TestStringifierReadWriteAttribute* JSTestStringifierReadWriteAttribute::toWrapped(JSC::VM&, JSC::JSValue value)
{
    if (auto* wrapper = jsDynamicCast<JSTestStringifierReadWriteAttribute*>(value))
        return &wrapper->wrapped();
    return nullptr;
}

}
