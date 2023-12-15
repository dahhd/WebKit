/*
 * Copyright (C) 2023 Igalia S.L.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2,1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "CryptoAlgorithmX25519.h"

#if ENABLE(WEB_CRYPTO)

#include "CryptoAlgorithmEcKeyParams.h"
#include "CryptoAlgorithmX25519Params.h"
#include "CryptoKeyOKP.h"
#include "ScriptExecutionContext.h"

namespace WebCore {

Ref<CryptoAlgorithm> CryptoAlgorithmX25519::create()
{
    return adoptRef(*new CryptoAlgorithmX25519);
}

CryptoAlgorithmIdentifier CryptoAlgorithmX25519::identifier() const
{
    return s_identifier;
}

void CryptoAlgorithmX25519::generateKey(const CryptoAlgorithmParameters&, bool extractable, CryptoKeyUsageBitmap usages, KeyOrKeyPairCallback&& callback, ExceptionCallback&& exceptionCallback, ScriptExecutionContext&)
{
    if (usages & (CryptoKeyUsageEncrypt | CryptoKeyUsageDecrypt | CryptoKeyUsageSign | CryptoKeyUsageVerify | CryptoKeyUsageWrapKey | CryptoKeyUsageUnwrapKey)) {
        exceptionCallback(SyntaxError);
        return;
    }

    auto result = CryptoKeyOKP::generatePair(CryptoAlgorithmIdentifier::X25519, CryptoKeyOKP::NamedCurve::X25519, extractable, usages);
    if (result.hasException()) {
        exceptionCallback(result.releaseException().code());
        return;
    }

    auto pair = result.releaseReturnValue();
    pair.publicKey->setUsagesBitmap(0);
    pair.privateKey->setUsagesBitmap(pair.privateKey->usagesBitmap() & (CryptoKeyUsageDeriveKey | CryptoKeyUsageDeriveBits));
    callback(WTFMove(pair));
}

#if !PLATFORM(COCOA) && !USE(GCRYPT)
std::optional<Vector<uint8_t>> CryptoAlgorithmX25519::platformDeriveBits(const CryptoKeyOKP&, const CryptoKeyOKP&)
{
    return std::nullopt;
}
#endif

void CryptoAlgorithmX25519::deriveBits(const CryptoAlgorithmParameters& parameters, Ref<CryptoKey>&& baseKey, size_t length, VectorCallback&& callback, ExceptionCallback&& exceptionCallback, ScriptExecutionContext& context, WorkQueue& workQueue)
{
    if (baseKey->type() != CryptoKey::Type::Private) {
        exceptionCallback(InvalidAccessError);
        return;
    }
    auto& ecParameters = downcast<CryptoAlgorithmX25519Params>(parameters);
    ASSERT(ecParameters.publicKey);
    if (ecParameters.publicKey->type() != CryptoKey::Type::Public) {
        exceptionCallback(InvalidAccessError);
        return;
    }
    if (baseKey->algorithmIdentifier() != ecParameters.publicKey->algorithmIdentifier()) {
        exceptionCallback(InvalidAccessError);
        return;
    }
    auto& ecBaseKey = downcast<CryptoKeyOKP>(baseKey.get());
    auto& ecPublicKey = downcast<CryptoKeyOKP>(*(ecParameters.publicKey.get()));
    if (ecBaseKey.namedCurve() != ecPublicKey.namedCurve()) {
        exceptionCallback(InvalidAccessError);
        return;
    }

    auto unifiedCallback = [callback = WTFMove(callback), exceptionCallback = WTFMove(exceptionCallback)](std::optional<Vector<uint8_t>>&& derivedKey, size_t length) mutable {
        if (!derivedKey) {
            exceptionCallback(OperationError);
            return;
        }
        if (!length) {
            callback(WTFMove(*derivedKey));
            return;
        }
        auto lengthInBytes = std::ceil(length / 8.);
        if (lengthInBytes > (*derivedKey).size()) {
            exceptionCallback(OperationError);
            return;
        }
        (*derivedKey).shrink(lengthInBytes);
        callback(WTFMove(*derivedKey));
    };

    // This is a special case that can't use dispatchOperation() because it bundles
    // the result validation and callback dispatch into unifiedCallback.
    workQueue.dispatch(
        [baseKey = WTFMove(baseKey), publicKey = ecParameters.publicKey, length, unifiedCallback = WTFMove(unifiedCallback), contextIdentifier = context.identifier()]() mutable {
            auto derivedKey = platformDeriveBits(downcast<CryptoKeyOKP>(baseKey.get()), downcast<CryptoKeyOKP>(*publicKey));
            ScriptExecutionContext::postTaskTo(contextIdentifier, [derivedKey = WTFMove(derivedKey), length, unifiedCallback = WTFMove(unifiedCallback)](auto&) mutable {
                unifiedCallback(WTFMove(derivedKey), length);
            });
        });
}

void CryptoAlgorithmX25519::importKey(CryptoKeyFormat format, KeyData&& data, const CryptoAlgorithmParameters&, bool extractable, CryptoKeyUsageBitmap usages, KeyCallback&& callback, ExceptionCallback&& exceptionCallback)
{
    RefPtr<CryptoKeyOKP> result;
    switch (format) {
    case CryptoKeyFormat::Jwk: {
        JsonWebKey key = WTFMove(std::get<JsonWebKey>(data));

        bool isUsagesAllowed = false;
        if (!key.d.isNull()) {
            isUsagesAllowed = isUsagesAllowed || !(usages ^ CryptoKeyUsageDeriveKey);
            isUsagesAllowed = isUsagesAllowed || !(usages ^ CryptoKeyUsageDeriveBits);
            isUsagesAllowed = isUsagesAllowed || !(usages ^ (CryptoKeyUsageDeriveKey | CryptoKeyUsageDeriveBits));
        }
        isUsagesAllowed = isUsagesAllowed || !usages;
        if (!isUsagesAllowed) {
            exceptionCallback(SyntaxError);
            return;
        }

        if (usages && !key.use.isNull() && key.use != "enc"_s) {
            exceptionCallback(DataError);
            return;
        }

        result = CryptoKeyOKP::importJwk(CryptoAlgorithmIdentifier::X25519, CryptoKeyOKP::NamedCurve::X25519, WTFMove(key), extractable, usages);
        break;
    }
    case CryptoKeyFormat::Raw:
        if (usages) {
            exceptionCallback(SyntaxError);
            return;
        }
        if (!extractable)
            usages = CryptoKeyUsageDeriveBits;
        result = CryptoKeyOKP::importRaw(CryptoAlgorithmIdentifier::X25519, CryptoKeyOKP::NamedCurve::X25519, WTFMove(std::get<Vector<uint8_t>>(data)), extractable, usages);
        break;
    case CryptoKeyFormat::Spki:
        if (usages) {
            exceptionCallback(SyntaxError);
            return;
        }
        result = CryptoKeyOKP::importSpki(CryptoAlgorithmIdentifier::X25519, CryptoKeyOKP::NamedCurve::X25519, WTFMove(std::get<Vector<uint8_t>>(data)), extractable, usages);
        break;
    case CryptoKeyFormat::Pkcs8:
        if (usages && (usages ^ CryptoKeyUsageDeriveKey) && (usages ^ CryptoKeyUsageDeriveBits) && (usages ^ (CryptoKeyUsageDeriveKey | CryptoKeyUsageDeriveBits))) {
            exceptionCallback(SyntaxError);
            return;
        }
        result = CryptoKeyOKP::importPkcs8(CryptoAlgorithmIdentifier::X25519, CryptoKeyOKP::NamedCurve::X25519, WTFMove(std::get<Vector<uint8_t>>(data)), extractable, usages);
        break;
    }
    if (!result) {
        exceptionCallback(DataError);
        return;
    }

    callback(*result);
}

void CryptoAlgorithmX25519::exportKey(CryptoKeyFormat format, Ref<CryptoKey>&& key, KeyDataCallback&& callback, ExceptionCallback&& exceptionCallback)
{
    const auto& ecKey = downcast<CryptoKeyOKP>(key.get());

    if (!ecKey.keySizeInBits()) {
        exceptionCallback(OperationError);
        return;
    }

    KeyData result;
    switch (format) {
    case CryptoKeyFormat::Jwk: {
        auto jwk = ecKey.exportJwk();
        if (jwk.hasException()) {
            exceptionCallback(jwk.releaseException().code());
            return;
        }
        result = jwk.releaseReturnValue();
        break;
    }
    case CryptoKeyFormat::Raw: {
        auto raw = ecKey.exportRaw();
        if (raw.hasException()) {
            exceptionCallback(raw.releaseException().code());
            return;
        }
        result = raw.releaseReturnValue();
        break;
    }
    case CryptoKeyFormat::Spki: {
        auto spki = ecKey.exportSpki();
        if (spki.hasException()) {
            exceptionCallback(spki.releaseException().code());
            return;
        }
        result = spki.releaseReturnValue();
        break;
    }
    case CryptoKeyFormat::Pkcs8: {
        auto pkcs8 = ecKey.exportPkcs8();
        if (pkcs8.hasException()) {
            exceptionCallback(pkcs8.releaseException().code());
            return;
        }
        result = pkcs8.releaseReturnValue();
        break;
    }
    }

    callback(format, WTFMove(result));
}

} // namespace WebCore

#endif // ENABLE(WEB_CRYPTO)
