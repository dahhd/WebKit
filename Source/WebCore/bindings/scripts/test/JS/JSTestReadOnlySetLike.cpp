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
#include "JSTestReadOnlySetLike.h"

#include "ActiveDOMObject.h"
#include "ExtendedDOMClientIsoSubspaces.h"
#include "ExtendedDOMIsoSubspaces.h"
#include "JSDOMAttribute.h"
#include "JSDOMBinding.h"
#include "JSDOMConstructorNotConstructable.h"
#include "JSDOMConvertAny.h"
#include "JSDOMConvertStrings.h"
#include "JSDOMExceptionHandling.h"
#include "JSDOMGlobalObjectInlines.h"
#include "JSDOMOperation.h"
#include "JSDOMSetLike.h"
#include "JSDOMWrapperCache.h"
#include "ScriptExecutionContext.h"
#include "WebCoreJSClientData.h"
#include <JavaScriptCore/BuiltinNames.h>
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

static JSC_DECLARE_HOST_FUNCTION(jsTestReadOnlySetLikePrototypeFunction_has);
static JSC_DECLARE_HOST_FUNCTION(jsTestReadOnlySetLikePrototypeFunction_entries);
static JSC_DECLARE_HOST_FUNCTION(jsTestReadOnlySetLikePrototypeFunction_keys);
static JSC_DECLARE_HOST_FUNCTION(jsTestReadOnlySetLikePrototypeFunction_values);
static JSC_DECLARE_HOST_FUNCTION(jsTestReadOnlySetLikePrototypeFunction_forEach);

// Attributes

static JSC_DECLARE_CUSTOM_GETTER(jsTestReadOnlySetLikeConstructor);
static JSC_DECLARE_CUSTOM_GETTER(jsTestReadOnlySetLike_size);

class JSTestReadOnlySetLikePrototype final : public JSC::JSNonFinalObject {
public:
    using Base = JSC::JSNonFinalObject;
    static JSTestReadOnlySetLikePrototype* create(JSC::VM& vm, JSDOMGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSTestReadOnlySetLikePrototype* ptr = new (NotNull, JSC::allocateCell<JSTestReadOnlySetLikePrototype>(vm)) JSTestReadOnlySetLikePrototype(vm, globalObject, structure);
        ptr->finishCreation(vm);
        return ptr;
    }

    DECLARE_INFO;
    template<typename CellType, JSC::SubspaceAccess>
    static JSC::GCClient::IsoSubspace* subspaceFor(JSC::VM& vm)
    {
        STATIC_ASSERT_ISO_SUBSPACE_SHARABLE(JSTestReadOnlySetLikePrototype, Base);
        return &vm.plainObjectSpace();
    }
    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

private:
    JSTestReadOnlySetLikePrototype(JSC::VM& vm, JSC::JSGlobalObject*, JSC::Structure* structure)
        : JSC::JSNonFinalObject(vm, structure)
    {
    }

    void finishCreation(JSC::VM&);
};
STATIC_ASSERT_ISO_SUBSPACE_SHARABLE(JSTestReadOnlySetLikePrototype, JSTestReadOnlySetLikePrototype::Base);

using JSTestReadOnlySetLikeDOMConstructor = JSDOMConstructorNotConstructable<JSTestReadOnlySetLike>;

template<> const ClassInfo JSTestReadOnlySetLikeDOMConstructor::s_info = { "TestReadOnlySetLike"_s, &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestReadOnlySetLikeDOMConstructor) };

template<> JSValue JSTestReadOnlySetLikeDOMConstructor::prototypeForStructure(JSC::VM& vm, const JSDOMGlobalObject& globalObject)
{
    UNUSED_PARAM(vm);
    return globalObject.functionPrototype();
}

template<> void JSTestReadOnlySetLikeDOMConstructor::initializeProperties(VM& vm, JSDOMGlobalObject& globalObject)
{
    putDirect(vm, vm.propertyNames->length, jsNumber(0), JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum);
    JSString* nameString = jsNontrivialString(vm, "TestReadOnlySetLike"_s);
    m_originalName.set(vm, this, nameString);
    putDirect(vm, vm.propertyNames->name, nameString, JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum);
    putDirect(vm, vm.propertyNames->prototype, JSTestReadOnlySetLike::prototype(vm, globalObject), JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum | JSC::PropertyAttribute::DontDelete);
}

/* Hash table for prototype */

static const HashTableValue JSTestReadOnlySetLikePrototypeTableValues[] =
{
    { "constructor"_s, static_cast<unsigned>(JSC::PropertyAttribute::DontEnum), NoIntrinsic, { (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestReadOnlySetLikeConstructor), (intptr_t) static_cast<PutPropertySlot::PutValueFunc>(0) } },
    { "size"_s, static_cast<unsigned>(JSC::PropertyAttribute::DontEnum | JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::CustomAccessor), NoIntrinsic, { (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestReadOnlySetLike_size), (intptr_t) static_cast<PutPropertySlot::PutValueFunc>(0) } },
    { "has"_s, static_cast<unsigned>(JSC::PropertyAttribute::DontEnum | JSC::PropertyAttribute::Function), NoIntrinsic, { (intptr_t)static_cast<RawNativeFunction>(jsTestReadOnlySetLikePrototypeFunction_has), (intptr_t) (1) } },
    { "entries"_s, static_cast<unsigned>(JSC::PropertyAttribute::DontEnum | JSC::PropertyAttribute::Function), NoIntrinsic, { (intptr_t)static_cast<RawNativeFunction>(jsTestReadOnlySetLikePrototypeFunction_entries), (intptr_t) (0) } },
    { "keys"_s, static_cast<unsigned>(JSC::PropertyAttribute::DontEnum | JSC::PropertyAttribute::Function), NoIntrinsic, { (intptr_t)static_cast<RawNativeFunction>(jsTestReadOnlySetLikePrototypeFunction_keys), (intptr_t) (0) } },
    { "values"_s, static_cast<unsigned>(JSC::PropertyAttribute::DontEnum | JSC::PropertyAttribute::Function), NoIntrinsic, { (intptr_t)static_cast<RawNativeFunction>(jsTestReadOnlySetLikePrototypeFunction_values), (intptr_t) (0) } },
    { "forEach"_s, static_cast<unsigned>(JSC::PropertyAttribute::DontEnum | JSC::PropertyAttribute::Function), NoIntrinsic, { (intptr_t)static_cast<RawNativeFunction>(jsTestReadOnlySetLikePrototypeFunction_forEach), (intptr_t) (1) } },
};

const ClassInfo JSTestReadOnlySetLikePrototype::s_info = { "TestReadOnlySetLike"_s, &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestReadOnlySetLikePrototype) };

void JSTestReadOnlySetLikePrototype::finishCreation(VM& vm)
{
    Base::finishCreation(vm);
    reifyStaticProperties(vm, JSTestReadOnlySetLike::info(), JSTestReadOnlySetLikePrototypeTableValues, *this);
    putDirect(vm, vm.propertyNames->iteratorSymbol, getDirect(vm, vm.propertyNames->builtinNames().entriesPublicName()), static_cast<unsigned>(JSC::PropertyAttribute::DontEnum));
    JSC_TO_STRING_TAG_WITHOUT_TRANSITION();
}

const ClassInfo JSTestReadOnlySetLike::s_info = { "TestReadOnlySetLike"_s, &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestReadOnlySetLike) };

JSTestReadOnlySetLike::JSTestReadOnlySetLike(Structure* structure, JSDOMGlobalObject& globalObject, Ref<TestReadOnlySetLike>&& impl)
    : JSDOMWrapper<TestReadOnlySetLike>(structure, globalObject, WTFMove(impl))
{
}

void JSTestReadOnlySetLike::finishCreation(VM& vm)
{
    Base::finishCreation(vm);
    ASSERT(inherits(info()));

    static_assert(!std::is_base_of<ActiveDOMObject, TestReadOnlySetLike>::value, "Interface is not marked as [ActiveDOMObject] even though implementation class subclasses ActiveDOMObject.");

}

JSObject* JSTestReadOnlySetLike::createPrototype(VM& vm, JSDOMGlobalObject& globalObject)
{
    return JSTestReadOnlySetLikePrototype::create(vm, &globalObject, JSTestReadOnlySetLikePrototype::createStructure(vm, &globalObject, globalObject.objectPrototype()));
}

JSObject* JSTestReadOnlySetLike::prototype(VM& vm, JSDOMGlobalObject& globalObject)
{
    return getDOMPrototype<JSTestReadOnlySetLike>(vm, globalObject);
}

JSValue JSTestReadOnlySetLike::getConstructor(VM& vm, const JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSTestReadOnlySetLikeDOMConstructor, DOMConstructorID::TestReadOnlySetLike>(vm, *jsCast<const JSDOMGlobalObject*>(globalObject));
}

void JSTestReadOnlySetLike::destroy(JSC::JSCell* cell)
{
    JSTestReadOnlySetLike* thisObject = static_cast<JSTestReadOnlySetLike*>(cell);
    thisObject->JSTestReadOnlySetLike::~JSTestReadOnlySetLike();
}

JSC_DEFINE_CUSTOM_GETTER(jsTestReadOnlySetLikeConstructor, (JSGlobalObject* lexicalGlobalObject, EncodedJSValue thisValue, PropertyName))
{
    VM& vm = JSC::getVM(lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    auto* prototype = jsDynamicCast<JSTestReadOnlySetLikePrototype*>(JSValue::decode(thisValue));
    if (UNLIKELY(!prototype))
        return throwVMTypeError(lexicalGlobalObject, throwScope);
    return JSValue::encode(JSTestReadOnlySetLike::getConstructor(JSC::getVM(lexicalGlobalObject), prototype->globalObject()));
}

static inline JSValue jsTestReadOnlySetLike_sizeGetter(JSGlobalObject& lexicalGlobalObject, JSTestReadOnlySetLike& thisObject)
{
    auto& vm = JSC::getVM(&lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    RELEASE_AND_RETURN(throwScope, (toJS<IDLAny>(lexicalGlobalObject, throwScope, forwardSizeToSetLike(lexicalGlobalObject, thisObject))));
}

JSC_DEFINE_CUSTOM_GETTER(jsTestReadOnlySetLike_size, (JSGlobalObject* lexicalGlobalObject, EncodedJSValue thisValue, PropertyName attributeName))
{
    return IDLAttribute<JSTestReadOnlySetLike>::get<jsTestReadOnlySetLike_sizeGetter>(*lexicalGlobalObject, thisValue, attributeName);
}

static inline JSC::EncodedJSValue jsTestReadOnlySetLikePrototypeFunction_hasBody(JSC::JSGlobalObject* lexicalGlobalObject, JSC::CallFrame* callFrame, typename IDLOperation<JSTestReadOnlySetLike>::ClassParameter castedThis)
{
    auto& vm = JSC::getVM(lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    UNUSED_PARAM(throwScope);
    UNUSED_PARAM(callFrame);
    if (UNLIKELY(callFrame->argumentCount() < 1))
        return throwVMError(lexicalGlobalObject, throwScope, createNotEnoughArgumentsError(lexicalGlobalObject));
    EnsureStillAliveScope argument0 = callFrame->uncheckedArgument(0);
    auto key = convert<IDLDOMString>(*lexicalGlobalObject, argument0.value());
    RETURN_IF_EXCEPTION(throwScope, encodedJSValue());
    RELEASE_AND_RETURN(throwScope, JSValue::encode(toJS<IDLAny>(*lexicalGlobalObject, throwScope, forwardHasToSetLike(*lexicalGlobalObject, *callFrame, *castedThis, WTFMove(key)))));
}

JSC_DEFINE_HOST_FUNCTION(jsTestReadOnlySetLikePrototypeFunction_has, (JSGlobalObject* lexicalGlobalObject, CallFrame* callFrame))
{
    return IDLOperation<JSTestReadOnlySetLike>::call<jsTestReadOnlySetLikePrototypeFunction_hasBody>(*lexicalGlobalObject, *callFrame, "has");
}

static inline JSC::EncodedJSValue jsTestReadOnlySetLikePrototypeFunction_entriesBody(JSC::JSGlobalObject* lexicalGlobalObject, JSC::CallFrame* callFrame, typename IDLOperation<JSTestReadOnlySetLike>::ClassParameter castedThis)
{
    auto& vm = JSC::getVM(lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    UNUSED_PARAM(throwScope);
    UNUSED_PARAM(callFrame);
    RELEASE_AND_RETURN(throwScope, JSValue::encode(toJS<IDLAny>(*lexicalGlobalObject, throwScope, forwardEntriesToSetLike(*lexicalGlobalObject, *callFrame, *castedThis))));
}

JSC_DEFINE_HOST_FUNCTION(jsTestReadOnlySetLikePrototypeFunction_entries, (JSGlobalObject* lexicalGlobalObject, CallFrame* callFrame))
{
    return IDLOperation<JSTestReadOnlySetLike>::call<jsTestReadOnlySetLikePrototypeFunction_entriesBody>(*lexicalGlobalObject, *callFrame, "entries");
}

static inline JSC::EncodedJSValue jsTestReadOnlySetLikePrototypeFunction_keysBody(JSC::JSGlobalObject* lexicalGlobalObject, JSC::CallFrame* callFrame, typename IDLOperation<JSTestReadOnlySetLike>::ClassParameter castedThis)
{
    auto& vm = JSC::getVM(lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    UNUSED_PARAM(throwScope);
    UNUSED_PARAM(callFrame);
    RELEASE_AND_RETURN(throwScope, JSValue::encode(toJS<IDLAny>(*lexicalGlobalObject, throwScope, forwardKeysToSetLike(*lexicalGlobalObject, *callFrame, *castedThis))));
}

JSC_DEFINE_HOST_FUNCTION(jsTestReadOnlySetLikePrototypeFunction_keys, (JSGlobalObject* lexicalGlobalObject, CallFrame* callFrame))
{
    return IDLOperation<JSTestReadOnlySetLike>::call<jsTestReadOnlySetLikePrototypeFunction_keysBody>(*lexicalGlobalObject, *callFrame, "keys");
}

static inline JSC::EncodedJSValue jsTestReadOnlySetLikePrototypeFunction_valuesBody(JSC::JSGlobalObject* lexicalGlobalObject, JSC::CallFrame* callFrame, typename IDLOperation<JSTestReadOnlySetLike>::ClassParameter castedThis)
{
    auto& vm = JSC::getVM(lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    UNUSED_PARAM(throwScope);
    UNUSED_PARAM(callFrame);
    RELEASE_AND_RETURN(throwScope, JSValue::encode(toJS<IDLAny>(*lexicalGlobalObject, throwScope, forwardValuesToSetLike(*lexicalGlobalObject, *callFrame, *castedThis))));
}

JSC_DEFINE_HOST_FUNCTION(jsTestReadOnlySetLikePrototypeFunction_values, (JSGlobalObject* lexicalGlobalObject, CallFrame* callFrame))
{
    return IDLOperation<JSTestReadOnlySetLike>::call<jsTestReadOnlySetLikePrototypeFunction_valuesBody>(*lexicalGlobalObject, *callFrame, "values");
}

static inline JSC::EncodedJSValue jsTestReadOnlySetLikePrototypeFunction_forEachBody(JSC::JSGlobalObject* lexicalGlobalObject, JSC::CallFrame* callFrame, typename IDLOperation<JSTestReadOnlySetLike>::ClassParameter castedThis)
{
    auto& vm = JSC::getVM(lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    UNUSED_PARAM(throwScope);
    UNUSED_PARAM(callFrame);
    if (UNLIKELY(callFrame->argumentCount() < 1))
        return throwVMError(lexicalGlobalObject, throwScope, createNotEnoughArgumentsError(lexicalGlobalObject));
    EnsureStillAliveScope argument0 = callFrame->uncheckedArgument(0);
    auto callback = convert<IDLAny>(*lexicalGlobalObject, argument0.value());
    RETURN_IF_EXCEPTION(throwScope, encodedJSValue());
    RELEASE_AND_RETURN(throwScope, JSValue::encode(toJS<IDLAny>(*lexicalGlobalObject, throwScope, forwardForEachToSetLike(*lexicalGlobalObject, *callFrame, *castedThis, WTFMove(callback)))));
}

JSC_DEFINE_HOST_FUNCTION(jsTestReadOnlySetLikePrototypeFunction_forEach, (JSGlobalObject* lexicalGlobalObject, CallFrame* callFrame))
{
    return IDLOperation<JSTestReadOnlySetLike>::call<jsTestReadOnlySetLikePrototypeFunction_forEachBody>(*lexicalGlobalObject, *callFrame, "forEach");
}

JSC::GCClient::IsoSubspace* JSTestReadOnlySetLike::subspaceForImpl(JSC::VM& vm)
{
    return WebCore::subspaceForImpl<JSTestReadOnlySetLike, UseCustomHeapCellType::No>(vm,
        [] (auto& spaces) { return spaces.m_clientSubspaceForTestReadOnlySetLike.get(); },
        [] (auto& spaces, auto&& space) { spaces.m_clientSubspaceForTestReadOnlySetLike = WTFMove(space); },
        [] (auto& spaces) { return spaces.m_subspaceForTestReadOnlySetLike.get(); },
        [] (auto& spaces, auto&& space) { spaces.m_subspaceForTestReadOnlySetLike = WTFMove(space); }
    );
}

void JSTestReadOnlySetLike::analyzeHeap(JSCell* cell, HeapAnalyzer& analyzer)
{
    auto* thisObject = jsCast<JSTestReadOnlySetLike*>(cell);
    analyzer.setWrappedObjectForCell(cell, &thisObject->wrapped());
    if (thisObject->scriptExecutionContext())
        analyzer.setLabelForCell(cell, "url " + thisObject->scriptExecutionContext()->url().string());
    Base::analyzeHeap(cell, analyzer);
}

bool JSTestReadOnlySetLikeOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, AbstractSlotVisitor& visitor, const char** reason)
{
    UNUSED_PARAM(handle);
    UNUSED_PARAM(visitor);
    UNUSED_PARAM(reason);
    return false;
}

void JSTestReadOnlySetLikeOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    auto* jsTestReadOnlySetLike = static_cast<JSTestReadOnlySetLike*>(handle.slot()->asCell());
    auto& world = *static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, &jsTestReadOnlySetLike->wrapped(), jsTestReadOnlySetLike);
}

#if ENABLE(BINDING_INTEGRITY)
#if PLATFORM(WIN)
#pragma warning(disable: 4483)
extern "C" { extern void (*const __identifier("??_7TestReadOnlySetLike@WebCore@@6B@")[])(); }
#else
extern "C" { extern void* _ZTVN7WebCore19TestReadOnlySetLikeE[]; }
#endif
#endif

JSC::JSValue toJSNewlyCreated(JSC::JSGlobalObject*, JSDOMGlobalObject* globalObject, Ref<TestReadOnlySetLike>&& impl)
{

    if constexpr (std::is_polymorphic_v<TestReadOnlySetLike>) {
#if ENABLE(BINDING_INTEGRITY)
        const void* actualVTablePointer = getVTablePointer(impl.ptr());
#if PLATFORM(WIN)
        void* expectedVTablePointer = __identifier("??_7TestReadOnlySetLike@WebCore@@6B@");
#else
        void* expectedVTablePointer = &_ZTVN7WebCore19TestReadOnlySetLikeE[2];
#endif

        // If you hit this assertion you either have a use after free bug, or
        // TestReadOnlySetLike has subclasses. If TestReadOnlySetLike has subclasses that get passed
        // to toJS() we currently require TestReadOnlySetLike you to opt out of binding hardening
        // by adding the SkipVTableValidation attribute to the interface IDL definition
        RELEASE_ASSERT(actualVTablePointer == expectedVTablePointer);
#endif
    }
    return createWrapper<TestReadOnlySetLike>(globalObject, WTFMove(impl));
}

JSC::JSValue toJS(JSC::JSGlobalObject* lexicalGlobalObject, JSDOMGlobalObject* globalObject, TestReadOnlySetLike& impl)
{
    return wrap(lexicalGlobalObject, globalObject, impl);
}

TestReadOnlySetLike* JSTestReadOnlySetLike::toWrapped(JSC::VM&, JSC::JSValue value)
{
    if (auto* wrapper = jsDynamicCast<JSTestReadOnlySetLike*>(value))
        return &wrapper->wrapped();
    return nullptr;
}

}
