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

#include "config.h"
#include "RemoteCompositorIntegration.h"

#if ENABLE(GPU_PROCESS)

#include "RemoteCompositorIntegrationMessages.h"
#include "StreamServerConnection.h"
#include "WebGPUObjectHeap.h"
#include <WebCore/WebGPUCompositorIntegration.h>

namespace WebKit {

RemoteCompositorIntegration::RemoteCompositorIntegration(WebCore::WebGPU::CompositorIntegration& compositorIntegration, WebGPU::ObjectHeap& objectHeap, Ref<IPC::StreamServerConnection>&& streamConnection, WebGPUIdentifier identifier)
    : m_backing(compositorIntegration)
    , m_objectHeap(objectHeap)
    , m_streamConnection(WTFMove(streamConnection))
    , m_identifier(identifier)
{
    m_streamConnection->startReceivingMessages(*this, Messages::RemoteCompositorIntegration::messageReceiverName(), m_identifier.toUInt64());
}

RemoteCompositorIntegration::~RemoteCompositorIntegration() = default;

void RemoteCompositorIntegration::destruct()
{
    m_objectHeap.removeObject(m_identifier);
}

void RemoteCompositorIntegration::stopListeningForIPC()
{
    m_streamConnection->stopReceivingMessages(Messages::RemoteCompositorIntegration::messageReceiverName(), m_identifier.toUInt64());
}

#if PLATFORM(COCOA)
void RemoteCompositorIntegration::recreateRenderBuffers(int width, int height, CompletionHandler<void(Vector<MachSendRight>&&)>&& callback)
{
    callback(m_backing->recreateRenderBuffers(width, height));
}
#endif

void RemoteCompositorIntegration::prepareForDisplay(CompletionHandler<void(bool)>&& completionHandler)
{
    m_backing->prepareForDisplay([completionHandler = WTFMove(completionHandler)]() mutable {
        completionHandler(true);
    });
}

} // namespace WebKit

#endif // ENABLE(GPU_PROCESS)
