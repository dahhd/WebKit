/*
 * Copyright (C) 2016-2021 Apple Inc. All rights reserved.
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

#include "config.h"
#include "WebAssemblyFunction.h"

#if ENABLE(WEBASSEMBLY)

#include "JITOpaqueByproducts.h"
#include "JSCJSValueInlines.h"
#include "JSObject.h"
#include "JSObjectInlines.h"
#include "JSToWasm.h"
#include "JSWebAssemblyHelpers.h"
#include "JSWebAssemblyInstance.h"
#include "JSWebAssemblyMemory.h"
#include "JSWebAssemblyRuntimeError.h"
#include "LLIntThunks.h"
#include "LinkBuffer.h"
#include "ProtoCallFrameInlines.h"
#include "SlotVisitorInlines.h"
#include "StructureInlines.h"
#include "WasmCallee.h"
#include "WasmCallingConvention.h"
#include "WasmContext.h"
#include "WasmFormat.h"
#include "WasmMemory.h"
#include "WasmMemoryInformation.h"
#include "WasmModuleInformation.h"
#include "WasmTypeDefinitionInlines.h"
#include <wtf/StackPointer.h>
#include <wtf/SystemTracing.h>

namespace JSC {

const ClassInfo WebAssemblyFunction::s_info = { "WebAssemblyFunction"_s, &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(WebAssemblyFunction) };

static JSC_DECLARE_HOST_FUNCTION(callWebAssemblyFunction);

JSC_DEFINE_HOST_FUNCTION(callWebAssemblyFunction, (JSGlobalObject* globalObject, CallFrame* callFrame))
{
    VM& vm = globalObject->vm();
    auto scope = DECLARE_THROW_SCOPE(vm);
    WebAssemblyFunction* wasmFunction = jsCast<WebAssemblyFunction*>(callFrame->jsCallee());
    Wasm::TypeIndex typeIndex = wasmFunction->typeIndex();
    const Wasm::FunctionSignature& signature = Wasm::TypeInformation::getFunctionSignature(typeIndex);

    // Make sure that the memory we think we are going to run with matches the one we expect.
    ASSERT(wasmFunction->instance()->instance().calleeGroup()->isSafeToRun(wasmFunction->instance()->memory()->memory().mode()));

    std::optional<TraceScope> traceScope;
    if (Options::useTracePoints())
        traceScope.emplace(WebAssemblyExecuteStart, WebAssemblyExecuteEnd);

    Vector<EncodedJSValue, MarkedArgumentBuffer::inlineCapacity> boxedArgs;
    JSWebAssemblyInstance* jsInstance = wasmFunction->instance();
    Wasm::Instance* wasmInstance = &jsInstance->instance();

    if (signature.argumentsOrResultsIncludeV128())
        return throwVMTypeError(globalObject, scope, Wasm::errorMessageForExceptionType(Wasm::ExceptionType::TypeErrorInvalidV128Use));

    for (unsigned argIndex = 0; argIndex < signature.argumentCount(); ++argIndex) {
        uint64_t value = fromJSValue(globalObject, signature.argumentType(argIndex), callFrame->argument(argIndex));
        RETURN_IF_EXCEPTION(scope, encodedJSValue());
        boxedArgs.append(value);
    }

    EncodedJSValue* args = boxedArgs.data();
    int argCount = boxedArgs.size() + 1;

    // Note: we specifically use the WebAssemblyFunction as the callee to begin with in the ProtoCallFrame.
    // The reason for this is that calling into the llint may stack overflow, and the stack overflow
    // handler might read the global object from the callee.
    // For wasm, we setup |codeBlock| and |this| differently.
    //     |this| : We keep JSWebAssemblyInstance in |this| to anchor it for GC.
    //     |codeBlock| : JS entry wrapper expects a Wasm::Instance* as the |codeBlock| value argument.
    ProtoCallFrame protoCallFrame;
    protoCallFrame.init(nullptr, globalObject, wasmFunction, jsInstance, argCount, args);
    protoCallFrame.setWasmInstance(wasmInstance);

    // FIXME Do away with this entire function, and only use the entrypoint generated by B3. https://bugs.webkit.org/show_bug.cgi?id=166486
    {
        // We do the stack check here for the wrapper function because we don't
        // want to emit a stack check inside every wrapper function.
        const uintptr_t sp = bitwise_cast<uintptr_t>(currentStackPointer());
        const uintptr_t frameSize = (boxedArgs.size() + CallFrame::headerSizeInRegisters) * sizeof(Register);
        const uintptr_t stackSpaceUsed = 2 * frameSize; // We're making two calls. One to the wrapper, and one to the actual wasm code.
        if (UNLIKELY((sp < stackSpaceUsed) || ((sp - stackSpaceUsed) < bitwise_cast<uintptr_t>(vm.softStackLimit()))))
            return JSValue::encode(throwException(globalObject, scope, createStackOverflowError(globalObject)));
    }
    ASSERT(wasmFunction->instance());
    EncodedJSValue rawResult = vmEntryToWasm(wasmFunction->jsEntrypoint(MustCheckArity).taggedPtr(), &vm, &protoCallFrame);
    RETURN_IF_EXCEPTION(scope, { });

    // We need to make sure this is in a register or on the stack since it's stored in Vector<JSValue>.
    // This probably isn't strictly necessary, since the WebAssemblyFunction* should keep the JSWebAssemblyInstance
    // alive. But it's good hygiene.
    jsInstance->use();

    return rawResult;
}

bool WebAssemblyFunction::usesTagRegisters() const
{
    const auto& signature = Wasm::TypeInformation::getFunctionSignature(typeIndex());
    return signature.argumentCount() || !signature.returnsVoid();
}

RegisterSet WebAssemblyFunction::calleeSaves() const
{
    // Pessimistically save callee saves in BoundsChecking mode since the LLInt always bounds checks
    RegisterSetBuilder result = RegisterSetBuilder::wasmPinnedRegisters();
    if (usesTagRegisters()) {
        RegisterSetBuilder tagCalleeSaves = RegisterSetBuilder::vmCalleeSaveRegisters();
        tagCalleeSaves.filter(RegisterSetBuilder::runtimeTagRegisters());
        result.merge(tagCalleeSaves);
    }
    return result.buildAndValidate();
}

RegisterAtOffsetList WebAssemblyFunction::usedCalleeSaveRegisters() const
{
    return RegisterAtOffsetList { calleeSaves(), RegisterAtOffsetList::OffsetBaseType::FramePointerBased };
}

#if ENABLE(JIT)
static size_t trampolineReservedStackSize()
{
    // If we are jumping to the function which can have stack-overflow check,
    // then, trampoline does not need to do the check again if it is smaller than a threshold.
    // 1. Caller of this trampoline ensures that at least our stack is lower than softStackLimit.
    // 2. Callee may omit stack check if the frame size is less than reservedZoneSize and it does not have a call.
    // Based on that, trampoline between 1 and 2 can use (softReservedZoneSize - reservedZoneSize) / 2 size safely at least.
    // Note that minimumReservedZoneSize is 16KB, and we ensure that softReservedZoneSize - reservedZoneSize is at least 16KB.
    return (Options::softReservedZoneSize() - Options::reservedZoneSize()) / 2;
}

CodePtr<JSEntryPtrTag> WebAssemblyFunction::jsCallEntrypointSlow()
{
    if (Options::forceICFailure())
        return nullptr;

    VM& vm = this->vm();
    CCallHelpers jit;

    JIT_COMMENT(jit, "jsCallEntrypointSlow");

    const auto& typeDefinition = Wasm::TypeInformation::get(typeIndex()).expand();
    const auto& signature = *typeDefinition.as<Wasm::FunctionSignature>();
    RegisterAtOffsetList registersToSpill = usedCalleeSaveRegisters();

    const Wasm::WasmCallingConvention& wasmCC = Wasm::wasmCallingConvention();
    Wasm::CallInformation wasmCallInfo = wasmCC.callInformationFor(typeDefinition);
    if (wasmCallInfo.argumentsOrResultsIncludeV128)
        return nullptr;
    Wasm::CallInformation jsCallInfo = Wasm::jsCallingConvention().callInformationFor(typeDefinition, Wasm::CallRole::Callee);
    RegisterAtOffsetList savedResultRegisters = wasmCallInfo.computeResultsOffsetList();

    unsigned totalFrameSize = registersToSpill.sizeOfAreaInBytes();
    totalFrameSize += sizeof(CPURegister); // Slot for the VM's previous wasm instance.
    totalFrameSize += wasmCallInfo.headerAndArgumentStackSizeInBytes;
    totalFrameSize += savedResultRegisters.sizeOfAreaInBytes();
    totalFrameSize = WTF::roundUpToMultipleOf(stackAlignmentBytes(), totalFrameSize);

#if USE(JSVALUE32_64)
    if (wasmCallInfo.argumentsIncludeI64)
        return nullptr;
#endif

    jit.emitFunctionPrologue();
    jit.subPtr(MacroAssembler::TrustedImm32(totalFrameSize), MacroAssembler::stackPointerRegister);
    jit.emitSave(registersToSpill);

    JSValueRegs scratchJSR {
#if USE(JSVALUE32_64)
        Wasm::wasmCallingConvention().prologueScratchGPRs[2],
#endif
        Wasm::wasmCallingConvention().prologueScratchGPRs[1]
    };
    GPRReg stackLimitGPR = Wasm::wasmCallingConvention().prologueScratchGPRs[0];

    CCallHelpers::JumpList slowPath;

    if (totalFrameSize >= trampolineReservedStackSize()) {
        jit.loadPtr(vm.addressOfSoftStackLimit(), stackLimitGPR);

        slowPath.append(jit.branchPtr(CCallHelpers::Above, MacroAssembler::stackPointerRegister, GPRInfo::callFrameRegister));
        slowPath.append(jit.branchPtr(CCallHelpers::Below, MacroAssembler::stackPointerRegister, stackLimitGPR));
    }

    // Ensure:
    // argCountPlusThis - 1 >= signature.argumentCount()
    // argCountPlusThis >= signature.argumentCount() + 1
    // FIXME: We should handle mismatched arity
    // https://bugs.webkit.org/show_bug.cgi?id=196564
    if (signature.argumentCount() > 0) {
        slowPath.append(jit.branch32(CCallHelpers::Below,
            CCallHelpers::payloadFor(CallFrameSlot::argumentCountIncludingThis), CCallHelpers::TrustedImm32(signature.argumentCount() + 1)));
    }

    if (usesTagRegisters())
        jit.emitMaterializeTagCheckRegisters();

    // Loop backwards so we can use the first floating point argument as a scratch.
    FPRReg scratchFPR = Wasm::wasmCallingConvention().fprArgs[0];
    for (unsigned i = signature.argumentCount(); i--;) {
        CCallHelpers::Address calleeFrame = CCallHelpers::Address(MacroAssembler::stackPointerRegister, 0);
        CCallHelpers::Address jsParam(GPRInfo::callFrameRegister, jsCallInfo.params[i].location.offsetFromFP());
        bool isStack = wasmCallInfo.params[i].location.isStackArgument();

        auto type = signature.argumentType(i);
        switch (type.kind) {
        case Wasm::TypeKind::I32: {
            jit.loadValue(jsParam, scratchJSR);
            slowPath.append(jit.branchIfNotInt32(scratchJSR));
            if (isStack) {
                CCallHelpers::Address addr { calleeFrame.withOffset(wasmCallInfo.params[i].location.offsetFromSP()) };
                jit.store32(scratchJSR.payloadGPR(), addr.withOffset(PayloadOffset));
#if USE(JSVALUE32_64)
                jit.store32(CCallHelpers::TrustedImm32(0), addr.withOffset(TagOffset));
#endif
            } else {
                jit.zeroExtend32ToWord(scratchJSR.payloadGPR(), wasmCallInfo.params[i].location.jsr().payloadGPR());
#if USE(JSVALUE32_64)
                jit.move(CCallHelpers::TrustedImm32(0), wasmCallInfo.params[i].location.jsr().tagGPR());
#endif
            }
            break;
        }
        case Wasm::TypeKind::I64: {
#if USE(JSVALUE64)
            static_assert(noOverlap(GPRInfo::wasmContextInstancePointer, GPRInfo::numberTagRegister, GPRInfo::notCellMaskRegister));
            jit.loadValue(jsParam, scratchJSR);
            slowPath.append(jit.branchIfNotCell(scratchJSR));
            slowPath.append(jit.branchIfNotHeapBigInt(scratchJSR.payloadGPR()));
            if (isStack) {
                jit.toBigInt64(scratchJSR.payloadGPR(), stackLimitGPR, GPRInfo::notCellMaskRegister, GPRInfo::numberTagRegister);
                jit.store64(stackLimitGPR, calleeFrame.withOffset(wasmCallInfo.params[i].location.offsetFromSP()));
                jit.emitMaterializeTagCheckRegisters();
            } else
                jit.toBigInt64(scratchJSR.payloadGPR(), wasmCallInfo.params[i].location.jsr().payloadGPR(), stackLimitGPR, GPRInfo::wasmContextInstancePointer);
#endif
            break;
        }
        case Wasm::TypeKind::Ref:
        case Wasm::TypeKind::RefNull:
        case Wasm::TypeKind::Funcref:
        case Wasm::TypeKind::Externref: {
            if (Wasm::isFuncref(type) || (Wasm::isRefWithTypeIndex(type) && Wasm::TypeInformation::get(type.index).is<Wasm::FunctionSignature>())) {
                // Ensure we have a WASM exported function.
                jit.loadValue(jsParam, scratchJSR);
                auto isNull = jit.branchIfNull(scratchJSR);
                if (!type.isNullable())
                    slowPath.append(isNull);
                slowPath.append(jit.branchIfNotCell(scratchJSR));

                jit.emitLoadStructure(vm, scratchJSR.payloadGPR(), scratchJSR.payloadGPR());
                jit.loadCompactPtr(CCallHelpers::Address(scratchJSR.payloadGPR(), Structure::classInfoOffset()), scratchJSR.payloadGPR());

                static_assert(std::is_final<WebAssemblyFunction>::value, "We do not check for subtypes below");
                static_assert(std::is_final<WebAssemblyWrapperFunction>::value, "We do not check for subtypes below");

                auto isWasmFunction = jit.branchPtr(CCallHelpers::Equal, scratchJSR.payloadGPR(), CCallHelpers::TrustedImmPtr(WebAssemblyFunction::info()));
                slowPath.append(jit.branchPtr(CCallHelpers::NotEqual, scratchJSR.payloadGPR(), CCallHelpers::TrustedImmPtr(WebAssemblyWrapperFunction::info())));

                isWasmFunction.link(&jit);
                if (Wasm::isRefWithTypeIndex(type)) {
                    jit.loadPtr(jsParam, scratchJSR.payloadGPR());
                    jit.loadPtr(CCallHelpers::Address(scratchJSR.payloadGPR(), WebAssemblyFunctionBase::offsetOfSignatureIndex()), scratchJSR.payloadGPR());
                    slowPath.append(jit.branchPtr(CCallHelpers::NotEqual, scratchJSR.payloadGPR(), CCallHelpers::TrustedImmPtr(type.index)));
                }

                if (type.isNullable())
                    isNull.link(&jit);
            } else if (!Wasm::isExternref(type)) {
                // FIXME: this should implement some fast paths for, e.g., i31refs and other
                // types that can be easily handled.
                slowPath.append(jit.jump());
            }

            if (isStack) {
                jit.loadValue(jsParam, scratchJSR);
                if (!type.isNullable())
                    slowPath.append(jit.branchIfNull(scratchJSR));
                jit.storeValue(scratchJSR, calleeFrame.withOffset(wasmCallInfo.params[i].location.offsetFromSP()));
            } else {
                auto externJSR = wasmCallInfo.params[i].location.jsr();
                jit.loadValue(jsParam, externJSR);
                if (!type.isNullable())
                    slowPath.append(jit.branchIfNull(externJSR));
            }
            break;
        }
        case Wasm::TypeKind::F32:
        case Wasm::TypeKind::F64: {
            if (!isStack)
                scratchFPR = wasmCallInfo.params[i].location.fpr();

            jit.loadValue(jsParam, scratchJSR);
#if USE(JSVALUE64)
            slowPath.append(jit.branchIfNotNumber(scratchJSR, InvalidGPRReg));
#elif USE(JSVALUE32_64)
            slowPath.append(jit.branchIfNotNumber(scratchJSR, stackLimitGPR));
#endif
            auto isInt32 = jit.branchIfInt32(scratchJSR);
#if USE(JSVALUE64)
            jit.unboxDouble(scratchJSR.payloadGPR(), scratchJSR.payloadGPR(), scratchFPR);
#elif USE(JSVALUE32_64)
            jit.unboxDouble(scratchJSR, scratchFPR);
#endif
            if (signature.argumentType(i).isF32())
                jit.convertDoubleToFloat(scratchFPR, scratchFPR);
            auto done = jit.jump();

            isInt32.link(&jit);
            if (signature.argumentType(i).isF32()) {
                jit.convertInt32ToFloat(scratchJSR.payloadGPR(), scratchFPR);
            } else {
                jit.convertInt32ToDouble(scratchJSR.payloadGPR(), scratchFPR);
            }
            done.link(&jit);
            if (isStack) {
                CCallHelpers::Address addr { calleeFrame.withOffset(wasmCallInfo.params[i].location.offsetFromSP()) };
                if (signature.argumentType(i).isF32()) {
                    jit.storeFloat(scratchFPR, addr.withOffset(PayloadOffset));
#if USE(JSVALUE32_64)
                    jit.store32(CCallHelpers::TrustedImm32(0), addr.withOffset(TagOffset));
#endif
                } else
                    jit.storeDouble(scratchFPR, addr);
            }
            break;
        }
        case Wasm::TypeKind::V128:
        default:
            RELEASE_ASSERT_NOT_REACHED();
        }
    }

    // At this point, we're committed to doing a fast call.

    jit.move(CCallHelpers::TrustedImmPtr(&instance()->instance()), GPRInfo::wasmContextInstancePointer);

#if !CPU(ARM) // ARM has no pinned registers for Wasm Memory, so no need to set them up
    if (!!instance()->instance().module().moduleInformation().memory) {
        auto mode = instance()->memoryMode();
        if (mode == MemoryMode::Signaling || (mode == MemoryMode::BoundsChecking && instance()->instance().memory()->sharingMode() == MemorySharingMode::Shared)) {
            // Capacity and basePointer will not be changed.
            if (mode == MemoryMode::BoundsChecking)
                jit.move(CCallHelpers::TrustedImm64(instance()->instance().memory()->mappedCapacity()), GPRInfo::wasmBoundsCheckingSizeRegister);
            jit.move(CCallHelpers::TrustedImmPtr(instance()->instance().memory()->basePointer()), GPRInfo::wasmBaseMemoryPointer);
        } else {
            GPRReg scratchOrBoundsCheckingSize = InvalidGPRReg;
            if (isARM64E()) {
                if (mode == MemoryMode::BoundsChecking)
                    scratchOrBoundsCheckingSize = GPRInfo::wasmBoundsCheckingSizeRegister;
                else
                    scratchOrBoundsCheckingSize = stackLimitGPR;
                jit.loadPairPtr(GPRInfo::wasmContextInstancePointer, CCallHelpers::TrustedImm32(Wasm::Instance::offsetOfCachedMemory()), GPRInfo::wasmBaseMemoryPointer, scratchOrBoundsCheckingSize);
            } else {
                if (mode == MemoryMode::BoundsChecking)
                    jit.loadPairPtr(GPRInfo::wasmContextInstancePointer, CCallHelpers::TrustedImm32(Wasm::Instance::offsetOfCachedMemory()), GPRInfo::wasmBaseMemoryPointer, GPRInfo::wasmBoundsCheckingSizeRegister);
                else
                    jit.loadPtr(CCallHelpers::Address(GPRInfo::wasmContextInstancePointer, Wasm::Instance::offsetOfCachedMemory()), GPRInfo::wasmBaseMemoryPointer);
            }
            jit.cageConditionally(Gigacage::Primitive, GPRInfo::wasmBaseMemoryPointer, scratchOrBoundsCheckingSize, scratchJSR.payloadGPR());
        }
    }
#endif
    // We use this callee to indicate how to unwind past these types of frames:
    // 1. We need to know where to get callee saves.
    // 2. We need to know to restore the previous wasm context.
    ASSERT(!m_jsToWasmICCallee);
    RefPtr<Wasm::JSToWasmICCallee> jsToWasmICCallee = Wasm::JSToWasmICCallee::create();
    jit.move(CCallHelpers::TrustedImmPtr(CalleeBits::boxNativeCallee(jsToWasmICCallee.get())), scratchJSR.payloadGPR());
    // We do not need to set up |this| in this IC since the caller of this IC itself already set up arguments and its |this| should be WebAssemblyFunction,
    // which anchors JSWebAssemblyInstance correctly from GC.
#if USE(JSVALUE32_64)
    jit.storePtr(scratchJSR.payloadGPR(), CCallHelpers::addressFor(CallFrameSlot::callee));
    jit.store32(CCallHelpers::TrustedImm32(JSValue::NativeCalleeTag), CCallHelpers::addressFor(CallFrameSlot::callee).withOffset(TagOffset));
    jit.storePtr(GPRInfo::wasmContextInstancePointer, CCallHelpers::addressFor(CallFrameSlot::codeBlock));
#else
    static_assert(CallFrameSlot::codeBlock + 1 == CallFrameSlot::callee);
    jit.storePairPtr(GPRInfo::wasmContextInstancePointer, scratchJSR.payloadGPR(), GPRInfo::callFrameRegister, CCallHelpers::TrustedImm32(CallFrameSlot::codeBlock * sizeof(Register)));
#endif

    // FIXME: Currently we just do an indirect jump. But we should teach the Module
    // how to repatch us:
    // https://bugs.webkit.org/show_bug.cgi?id=196570
    jit.loadPtr(entrypointLoadLocation(), scratchJSR.payloadGPR());
    jit.call(scratchJSR.payloadGPR(), WasmEntryPtrTag);

    // Restore stack pointer after call
    jit.addPtr(MacroAssembler::TrustedImm32(-static_cast<int32_t>(totalFrameSize)), MacroAssembler::framePointerRegister, MacroAssembler::stackPointerRegister);

    marshallJSResult(jit, typeDefinition, wasmCallInfo, savedResultRegisters);

    ASSERT(!RegisterSetBuilder::runtimeTagRegisters().contains(GPRInfo::nonPreservedNonReturnGPR, IgnoreVectors));

    jit.emitRestore(registersToSpill, GPRInfo::callFrameRegister);
    jit.emitFunctionEpilogue();
    jit.ret();

    slowPath.link(&jit);
    jit.emitRestore(registersToSpill, GPRInfo::callFrameRegister);
    jit.move(CCallHelpers::TrustedImmPtr(this), GPRInfo::regT0);
    jit.emitFunctionEpilogue();
#if CPU(ARM64E)
    jit.untagReturnAddress(scratchJSR.payloadGPR());
#endif
    jit.jumpThunk(CodeLocationLabel<JSEntryPtrTag> { executable()->entrypointFor(CodeForCall, MustCheckArity) });

    LinkBuffer linkBuffer(jit, nullptr, LinkBuffer::Profile::WasmThunk, JITCompilationCanFail);
    if (UNLIKELY(linkBuffer.didFailToAllocate()))
        return nullptr;

    auto compilation = makeUnique<Compilation>(FINALIZE_WASM_CODE(linkBuffer, JITCompilationPtrTag, "JS->Wasm IC"), nullptr);
    jsToWasmICCallee->setEntrypoint({ WTFMove(compilation), WTFMove(registersToSpill) });

    // Successfully compiled and linked the IC.
    m_jsToWasmICCallee = jsToWasmICCallee;

    return m_jsToWasmICCallee->entrypoint().retagged<JSEntryPtrTag>();
}
#endif // ENABLE(JIT)

WebAssemblyFunction* WebAssemblyFunction::create(VM& vm, JSGlobalObject* globalObject, Structure* structure, unsigned length, const String& name, JSWebAssemblyInstance* instance, Wasm::Callee& jsEntrypoint, Wasm::WasmToWasmImportableFunction::LoadLocation wasmToWasmEntrypointLoadLocation, Wasm::TypeIndex typeIndex, RefPtr<const Wasm::RTT> rtt)
{
    NativeExecutable* base = vm.getHostFunction(callWebAssemblyFunction, ImplementationVisibility::Public, WasmFunctionIntrinsic, callHostFunctionAsConstructor, nullptr, String());
    // Since ClosureCall uses this executable as an identity for Wasm CallIC thunk, we need to make it diversified.
    NativeExecutable* executable = NativeExecutable::create(vm, base->generatedJITCodeForCall(), callWebAssemblyFunction, base->generatedJITCodeForConstruct(), callHostFunctionAsConstructor, ImplementationVisibility::Public, name);
    WebAssemblyFunction* function = new (NotNull, allocateCell<WebAssemblyFunction>(vm)) WebAssemblyFunction(vm, executable, globalObject, structure, instance, jsEntrypoint, wasmToWasmEntrypointLoadLocation, typeIndex, rtt);
    function->finishCreation(vm, executable, length, name);
    return function;
}

Structure* WebAssemblyFunction::createStructure(VM& vm, JSGlobalObject* globalObject, JSValue prototype)
{
    ASSERT(globalObject);
    return Structure::create(vm, globalObject, prototype, TypeInfo(JSFunctionType, StructureFlags), info());
}

WebAssemblyFunction::WebAssemblyFunction(VM& vm, NativeExecutable* executable, JSGlobalObject* globalObject, Structure* structure, JSWebAssemblyInstance* instance, Wasm::Callee& jsEntrypoint, Wasm::WasmToWasmImportableFunction::LoadLocation wasmToWasmEntrypointLoadLocation, Wasm::TypeIndex typeIndex, RefPtr<const Wasm::RTT> rtt)
    : Base { vm, executable, globalObject, structure, instance, Wasm::WasmToWasmImportableFunction { typeIndex, wasmToWasmEntrypointLoadLocation }, rtt }
    , m_jsEntrypoint { jsEntrypoint.entrypoint() }
{ }

template<typename Visitor>
void WebAssemblyFunction::visitChildrenImpl(JSCell* cell, Visitor& visitor)
{
    WebAssemblyFunction* thisObject = jsCast<WebAssemblyFunction*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, info());

    Base::visitChildren(thisObject, visitor);
}

DEFINE_VISIT_CHILDREN(WebAssemblyFunction);

void WebAssemblyFunction::destroy(JSCell* cell)
{
    static_cast<WebAssemblyFunction*>(cell)->WebAssemblyFunction::~WebAssemblyFunction();
}

} // namespace JSC

#endif // ENABLE(WEBASSEMBLY)
