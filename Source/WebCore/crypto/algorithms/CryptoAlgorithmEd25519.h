/*
 * Copyright (C) 2023 Apple Inc. All rights reserved.
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

#include "CryptoAlgorithm.h"

namespace WebCore {

class CryptoKeyOKP;

class CryptoAlgorithmEd25519 final : public CryptoAlgorithm {
public:
    static constexpr ASCIILiteral s_name = "Ed25519"_s;
    static constexpr CryptoAlgorithmIdentifier s_identifier = CryptoAlgorithmIdentifier::Ed25519;
    static Ref<CryptoAlgorithm> create();

private:
    CryptoAlgorithmEd25519() = default;
    CryptoAlgorithmIdentifier identifier() const final;

    void generateKey(const CryptoAlgorithmParameters& , bool extractable, CryptoKeyUsageBitmap usages, KeyOrKeyPairCallback&& , ExceptionCallback&& , ScriptExecutionContext&);
    void sign(const CryptoAlgorithmParameters&, Ref<CryptoKey>&&, Vector<uint8_t>&&, VectorCallback&&, ExceptionCallback&&, ScriptExecutionContext&, WorkQueue&) final;
    void verify(const CryptoAlgorithmParameters&, Ref<CryptoKey>&&, Vector<uint8_t>&& signature, Vector<uint8_t>&&, BoolCallback&&, ExceptionCallback&&, ScriptExecutionContext&, WorkQueue&) final;
    void importKey(CryptoKeyFormat, KeyData&&, const CryptoAlgorithmParameters&, bool extractable, CryptoKeyUsageBitmap, KeyCallback&&, ExceptionCallback&&) final;
    void exportKey(CryptoKeyFormat, Ref<CryptoKey>&&, KeyDataCallback&&, ExceptionCallback&&) final;

    static ExceptionOr<Vector<uint8_t>> platformSign(const CryptoKeyOKP&, const Vector<uint8_t>&);
    static ExceptionOr<bool> platformVerify(const CryptoKeyOKP&, const Vector<uint8_t>&, const Vector<uint8_t>&);
};

inline CryptoAlgorithmIdentifier CryptoAlgorithmEd25519::identifier() const
{
    return s_identifier;
}

} // namespace WebCore
