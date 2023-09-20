/*
 * Copyright (C) 2021-2023 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#if ENABLE(GPU_PROCESS)

#include "WebGPUObjectDescriptorBase.h"
#include <WebCore/WebGPUAddressMode.h>
#include <WebCore/WebGPUCompareFunction.h>
#include <WebCore/WebGPUFilterMode.h>
#include <cstdint>
#include <optional>

namespace WebKit::WebGPU {

struct SamplerDescriptor : public ObjectDescriptorBase {
    WebCore::WebGPU::AddressMode addressModeU { WebCore::WebGPU::AddressMode::ClampToEdge };
    WebCore::WebGPU::AddressMode addressModeV { WebCore::WebGPU::AddressMode::ClampToEdge };
    WebCore::WebGPU::AddressMode addressModeW { WebCore::WebGPU::AddressMode::ClampToEdge };
    WebCore::WebGPU::FilterMode magFilter { WebCore::WebGPU::FilterMode::Nearest };
    WebCore::WebGPU::FilterMode minFilter { WebCore::WebGPU::FilterMode::Nearest };
    WebCore::WebGPU::MipmapFilterMode mipmapFilter { WebCore::WebGPU::MipmapFilterMode::Nearest };
    float lodMinClamp { 0 };
    float lodMaxClamp { 32 };
    std::optional<WebCore::WebGPU::CompareFunction> compare;
    uint16_t maxAnisotropy { 1 };
};

} // namespace WebKit::WebGPU

#endif // ENABLE(GPU_PROCESS)
