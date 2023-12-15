/*
 * Copyright (C) 2015 Igalia S.L. All rights reserved.
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

#include "RenderStyleConstants.h"

namespace WTF {
class TextStream;
}

namespace WebCore {

class StyleContentAlignmentData {
public:
    constexpr StyleContentAlignmentData() = default;

    // Style data for Content-Distribution properties: align-content, justify-content.
    // <content-distribution> || [ <overflow-position>? && <content-position> ]
    constexpr StyleContentAlignmentData(ContentPosition position, ContentDistribution distribution, OverflowAlignment overflow = OverflowAlignment::Default)
        : m_position(static_cast<uint16_t>(position))
        , m_distribution(static_cast<uint16_t>(distribution))
        , m_overflow(static_cast<uint16_t>(overflow))
    {
    }

    void setPosition(ContentPosition position) { m_position = static_cast<uint16_t>(position); }
    void setDistribution(ContentDistribution distribution) { m_distribution = static_cast<uint16_t>(distribution); }
    void setOverflow(OverflowAlignment overflow) { m_overflow = static_cast<uint16_t>(overflow); }

    ContentPosition position() const { return static_cast<ContentPosition>(m_position); }
    ContentDistribution distribution() const { return static_cast<ContentDistribution>(m_distribution); }
    OverflowAlignment overflow() const { return static_cast<OverflowAlignment>(m_overflow); }

    friend bool operator==(const StyleContentAlignmentData&, const StyleContentAlignmentData&) = default;

private:
    uint16_t m_position : 4 { 0 }; // ContentPosition
    uint16_t m_distribution : 3 { 0 }; // ContentDistribution
    uint16_t m_overflow : 2 { 0 }; // OverflowAlignment
};

WTF::TextStream& operator<<(WTF::TextStream&, const StyleContentAlignmentData&);

} // namespace WebCore
