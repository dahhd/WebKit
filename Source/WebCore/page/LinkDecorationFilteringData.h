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

#include "RegistrableDomain.h"
#include <optional>

namespace WebCore {

struct LinkDecorationFilteringData {
    RegistrableDomain domain;
    String linkDecoration;

    LinkDecorationFilteringData(RegistrableDomain&& domain, const String& linkDecoration)
        : domain(WTFMove(domain))
        , linkDecoration(linkDecoration)
    {
    }

    LinkDecorationFilteringData(const String& domain, const String& linkDecoration)
        : domain(RegistrableDomain { URL { domain } } )
        , linkDecoration(linkDecoration)
    {
    }

    LinkDecorationFilteringData(const String& linkDecoration)
        : linkDecoration(linkDecoration)
    {
    }

    LinkDecorationFilteringData(const LinkDecorationFilteringData&) = default;
    LinkDecorationFilteringData& operator=(const LinkDecorationFilteringData&) = default;

    LinkDecorationFilteringData(LinkDecorationFilteringData&& data)
        : domain(WTFMove(data.domain))
        , linkDecoration(WTFMove(data.linkDecoration))
    {
    }

    LinkDecorationFilteringData& operator=(LinkDecorationFilteringData&& data)
    {
        domain = WTFMove(data.domain);
        linkDecoration = WTFMove(data.linkDecoration);
        return *this;
    }

    template<class Encoder> void encode(Encoder& encoder) const
    {
        encoder << domain;
        encoder << linkDecoration;
    }

    template<class Decoder> static std::optional<LinkDecorationFilteringData> decode(Decoder& decoder)
    {
        std::optional<RegistrableDomain> domain;
        decoder >> domain;
        if (!domain)
            return std::nullopt;

        std::optional<String> linkDecoration;
        decoder >> linkDecoration;
        if (!linkDecoration)
            return std::nullopt;

        return { { WTFMove(*domain), WTFMove(*linkDecoration) } };
    }
};

}
