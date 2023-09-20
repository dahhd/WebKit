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

#pragma once

#include "JSDOMWrapper.h"
#include "TestConditionalIncludes.h"
#include <wtf/NeverDestroyed.h>

namespace WebCore {

class JSTestConditionalIncludes : public JSDOMWrapper<TestConditionalIncludes> {
public:
    using Base = JSDOMWrapper<TestConditionalIncludes>;
    static JSTestConditionalIncludes* create(JSC::Structure* structure, JSDOMGlobalObject* globalObject, Ref<TestConditionalIncludes>&& impl)
    {
        JSTestConditionalIncludes* ptr = new (NotNull, JSC::allocateCell<JSTestConditionalIncludes>(globalObject->vm())) JSTestConditionalIncludes(structure, *globalObject, WTFMove(impl));
        ptr->finishCreation(globalObject->vm());
        return ptr;
    }

    static JSC::JSObject* createPrototype(JSC::VM&, JSDOMGlobalObject&);
    static JSC::JSObject* prototype(JSC::VM&, JSDOMGlobalObject&);
    static TestConditionalIncludes* toWrapped(JSC::VM&, JSC::JSValue);
    static void destroy(JSC::JSCell*);

    DECLARE_INFO;

    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info(), JSC::NonArray);
    }

    static JSC::JSValue getConstructor(JSC::VM&, const JSC::JSGlobalObject*);
    template<typename, JSC::SubspaceAccess mode> static JSC::GCClient::IsoSubspace* subspaceFor(JSC::VM& vm)
    {
        if constexpr (mode == JSC::SubspaceAccess::Concurrently)
            return nullptr;
        return subspaceForImpl(vm);
    }
    static JSC::GCClient::IsoSubspace* subspaceForImpl(JSC::VM& vm);
    static void analyzeHeap(JSCell*, JSC::HeapAnalyzer&);

    // Custom attributes
#if ENABLE(Condition22) || ENABLE(Condition23)
    JSC::JSValue mixinCustomAttribute(JSC::JSGlobalObject&) const;
#endif
#if ENABLE(Condition22) || ENABLE(Condition23)
    void setMixinCustomAttribute(JSC::JSGlobalObject&, JSC::JSValue);
#endif

    // Custom functions
#if (ENABLE(Condition12) && ENABLE(Condition22)) || ENABLE(Condition23)
    JSC::JSValue mixinCustomOperation(JSC::JSGlobalObject&, JSC::CallFrame&);
#endif
protected:
    JSTestConditionalIncludes(JSC::Structure*, JSDOMGlobalObject&, Ref<TestConditionalIncludes>&&);

    DECLARE_DEFAULT_FINISH_CREATION;
};

class JSTestConditionalIncludesOwner final : public JSC::WeakHandleOwner {
public:
    bool isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown>, void* context, JSC::AbstractSlotVisitor&, const char**) final;
    void finalize(JSC::Handle<JSC::Unknown>, void* context) final;
};

inline JSC::WeakHandleOwner* wrapperOwner(DOMWrapperWorld&, TestConditionalIncludes*)
{
    static NeverDestroyed<JSTestConditionalIncludesOwner> owner;
    return &owner.get();
}

inline void* wrapperKey(TestConditionalIncludes* wrappableObject)
{
    return wrappableObject;
}

JSC::JSValue toJS(JSC::JSGlobalObject*, JSDOMGlobalObject*, TestConditionalIncludes&);
inline JSC::JSValue toJS(JSC::JSGlobalObject* lexicalGlobalObject, JSDOMGlobalObject* globalObject, TestConditionalIncludes* impl) { return impl ? toJS(lexicalGlobalObject, globalObject, *impl) : JSC::jsNull(); }
JSC::JSValue toJSNewlyCreated(JSC::JSGlobalObject*, JSDOMGlobalObject*, Ref<TestConditionalIncludes>&&);
inline JSC::JSValue toJSNewlyCreated(JSC::JSGlobalObject* lexicalGlobalObject, JSDOMGlobalObject* globalObject, RefPtr<TestConditionalIncludes>&& impl) { return impl ? toJSNewlyCreated(lexicalGlobalObject, globalObject, impl.releaseNonNull()) : JSC::jsNull(); }

template<> struct JSDOMWrapperConverterTraits<TestConditionalIncludes> {
    using WrapperClass = JSTestConditionalIncludes;
    using ToWrappedReturnType = TestConditionalIncludes*;
};

} // namespace WebCore
