/*
 * Copyright (C) 2022-2023 Apple Inc. All rights reserved.
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

#include "GPUCanvasCompositingAlphaMode.h"
#include "GPUDevice.h"
#include "GPUExtent3DDict.h"
#include "GPUIntegralTypes.h"
#include "GPUTextureFormat.h"
#include "GPUTextureUsage.h"
#include <pal/graphics/WebGPU/WebGPUPresentationConfiguration.h>
#include <wtf/Vector.h>

namespace WebCore {

struct GPUPresentationConfiguration {
    PAL::WebGPU::PresentationConfiguration convertToBacking() const
    {
        ASSERT(device);
        return {
            device->backing(),
            WebCore::convertToBacking(format),
            convertTextureUsageFlagsToBacking(usage),
            viewFormats.map([] (auto& viewFormat) {
                return WebCore::convertToBacking(viewFormat);
            }),
            WebCore::convertToBacking(colorSpace),
            WebCore::convertToBacking(compositingAlphaMode),
            width,
            height,
        };
    }

    GPUDevice* device { nullptr };
    GPUTextureFormat format { GPUTextureFormat::Bgra8unorm };
    GPUTextureUsageFlags usage { GPUTextureUsage::RENDER_ATTACHMENT };
    Vector<GPUTextureFormat> viewFormats;
    GPUPredefinedColorSpace colorSpace { GPUPredefinedColorSpace::SRGB };
    GPUCanvasCompositingAlphaMode compositingAlphaMode { GPUCanvasCompositingAlphaMode::Opaque };
    uint32_t width { 0 };
    uint32_t height { 0 };
};

}
