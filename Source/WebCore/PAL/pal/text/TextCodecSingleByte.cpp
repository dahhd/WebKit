/*
 * Copyright (C) 2020 Apple Inc. All rights reserved.
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
#include "TextCodecSingleByte.h"

#include "EncodingTables.h"
#include <mutex>
#include <wtf/IteratorRange.h>
#include <wtf/text/CodePointIterator.h>
#include <wtf/text/StringBuilder.h>
#include <wtf/unicode/CharacterNames.h>

namespace PAL {

enum class TextCodecSingleByte::Encoding : uint8_t {
    ISO_8859_3,
    ISO_8859_6,
    ISO_8859_7,
    ISO_8859_8,
    Windows_874,
    Windows_1253,
    Windows_1255,
    Windows_1257,
    IBM866,
    KOI8U,
};

using SingleByteDecodeTable = std::array<UChar, 128>;
using SingleByteEncodeTableEntry = std::pair<UChar, uint8_t>;
using SingleByteEncodeTable = IteratorRange<const SingleByteEncodeTableEntry*>;

// From https://encoding.spec.whatwg.org/index-iso-8859-3.txt with 0xFFFD filling the gaps
static constexpr SingleByteDecodeTable iso88593 {
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087, 0x0088, 0x0089, 0x008A, 0x008B, 0x008C, 0x008D, 0x008E, 0x008F,
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097, 0x0098, 0x0099, 0x009A, 0x009B, 0x009C, 0x009D, 0x009E, 0x009F,
    0x00A0, 0x0126, 0x02D8, 0x00A3, 0x00A4, 0xFFFD, 0x0124, 0x00A7, 0x00A8, 0x0130, 0x015E, 0x011E, 0x0134, 0x00AD, 0xFFFD, 0x017B,
    0x00B0, 0x0127, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x0125, 0x00B7, 0x00B8, 0x0131, 0x015F, 0x011F, 0x0135, 0x00BD, 0xFFFD, 0x017C,
    0x00C0, 0x00C1, 0x00C2, 0xFFFD, 0x00C4, 0x010A, 0x0108, 0x00C7, 0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF,
    0xFFFD, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x0120, 0x00D6, 0x00D7, 0x011C, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x016C, 0x015C, 0x00DF,
    0x00E0, 0x00E1, 0x00E2, 0xFFFD, 0x00E4, 0x010B, 0x0109, 0x00E7, 0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF,
    0xFFFD, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x0121, 0x00F6, 0x00F7, 0x011D, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x016D, 0x015D, 0x02D9
};

// From https://encoding.spec.whatwg.org/index-iso-8859-6.txt with 0xFFFD filling the gaps
static constexpr SingleByteDecodeTable iso88596 {
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087, 0x0088, 0x0089, 0x008A, 0x008B, 0x008C, 0x008D, 0x008E, 0x008F,
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097, 0x0098, 0x0099, 0x009A, 0x009B, 0x009C, 0x009D, 0x009E, 0x009F,
    0x00A0, 0xFFFD, 0xFFFD, 0xFFFD, 0x00A4, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0x060C, 0x00AD, 0xFFFD, 0xFFFD,
    0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0x061B, 0xFFFD, 0xFFFD, 0xFFFD, 0x061F,
    0xFFFD, 0x0621, 0x0622, 0x0623, 0x0624, 0x0625, 0x0626, 0x0627, 0x0628, 0x0629, 0x062A, 0x062B, 0x062C, 0x062D, 0x062E, 0x062F,
    0x0630, 0x0631, 0x0632, 0x0633, 0x0634, 0x0635, 0x0636, 0x0637, 0x0638, 0x0639, 0x063A, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD,
    0x0640, 0x0641, 0x0642, 0x0643, 0x0644, 0x0645, 0x0646, 0x0647, 0x0648, 0x0649, 0x064A, 0x064B, 0x064C, 0x064D, 0x064E, 0x064F,
    0x0650, 0x0651, 0x0652, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD
};

// From https://encoding.spec.whatwg.org/index-iso-8859-7.txt with 0xFFFD filling the gaps
static constexpr SingleByteDecodeTable iso88597 {
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087, 0x0088, 0x0089, 0x008A, 0x008B, 0x008C, 0x008D, 0x008E, 0x008F,
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097, 0x0098, 0x0099, 0x009A, 0x009B, 0x009C, 0x009D, 0x009E, 0x009F,
    0x00A0, 0x2018, 0x2019, 0x00A3, 0x20AC, 0x20AF, 0x00A6, 0x00A7, 0x00A8, 0x00A9, 0x037A, 0x00AB, 0x00AC, 0x00AD, 0xFFFD, 0x2015,
    0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x0384, 0x0385, 0x0386, 0x00B7, 0x0388, 0x0389, 0x038A, 0x00BB, 0x038C, 0x00BD, 0x038E, 0x038F,
    0x0390, 0x0391, 0x0392, 0x0393, 0x0394, 0x0395, 0x0396, 0x0397, 0x0398, 0x0399, 0x039A, 0x039B, 0x039C, 0x039D, 0x039E, 0x039F,
    0x03A0, 0x03A1, 0xFFFD, 0x03A3, 0x03A4, 0x03A5, 0x03A6, 0x03A7, 0x03A8, 0x03A9, 0x03AA, 0x03AB, 0x03AC, 0x03AD, 0x03AE, 0x03AF,
    0x03B0, 0x03B1, 0x03B2, 0x03B3, 0x03B4, 0x03B5, 0x03B6, 0x03B7, 0x03B8, 0x03B9, 0x03BA, 0x03BB, 0x03BC, 0x03BD, 0x03BE, 0x03BF,
    0x03C0, 0x03C1, 0x03C2, 0x03C3, 0x03C4, 0x03C5, 0x03C6, 0x03C7, 0x03C8, 0x03C9, 0x03CA, 0x03CB, 0x03CC, 0x03CD, 0x03CE, 0xFFFD
};

// From https://encoding.spec.whatwg.org/index-iso-8859-8.txt with 0xFFFD filling the gaps
static constexpr SingleByteDecodeTable iso88598 {
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087, 0x0088, 0x0089, 0x008A, 0x008B, 0x008C, 0x008D, 0x008E, 0x008F,
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097, 0x0098, 0x0099, 0x009A, 0x009B, 0x009C, 0x009D, 0x009E, 0x009F,
    0x00A0, 0xFFFD, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7, 0x00A8, 0x00A9, 0x00D7, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF,
    0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7, 0x00B8, 0x00B9, 0x00F7, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0xFFFD,
    0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD,
    0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0x2017,
    0x05D0, 0x05D1, 0x05D2, 0x05D3, 0x05D4, 0x05D5, 0x05D6, 0x05D7, 0x05D8, 0x05D9, 0x05DA, 0x05DB, 0x05DC, 0x05DD, 0x05DE, 0x05DF,
    0x05E0, 0x05E1, 0x05E2, 0x05E3, 0x05E4, 0x05E5, 0x05E6, 0x05E7, 0x05E8, 0x05E9, 0x05EA, 0xFFFD, 0xFFFD, 0x200E, 0x200F, 0xFFFD
};

// From https://encoding.spec.whatwg.org/index-windows-874.txt with 0xFFFD filling the gaps
static constexpr SingleByteDecodeTable windows874 {
    0x20AC, 0x0081, 0x0082, 0x0083, 0x0084, 0x2026, 0x0086, 0x0087, 0x0088, 0x0089, 0x008A, 0x008B, 0x008C, 0x008D, 0x008E, 0x008F,
    0x0090, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014, 0x0098, 0x0099, 0x009A, 0x009B, 0x009C, 0x009D, 0x009E, 0x009F,
    0x00A0, 0x0E01, 0x0E02, 0x0E03, 0x0E04, 0x0E05, 0x0E06, 0x0E07, 0x0E08, 0x0E09, 0x0E0A, 0x0E0B, 0x0E0C, 0x0E0D, 0x0E0E, 0x0E0F,
    0x0E10, 0x0E11, 0x0E12, 0x0E13, 0x0E14, 0x0E15, 0x0E16, 0x0E17, 0x0E18, 0x0E19, 0x0E1A, 0x0E1B, 0x0E1C, 0x0E1D, 0x0E1E, 0x0E1F,
    0x0E20, 0x0E21, 0x0E22, 0x0E23, 0x0E24, 0x0E25, 0x0E26, 0x0E27, 0x0E28, 0x0E29, 0x0E2A, 0x0E2B, 0x0E2C, 0x0E2D, 0x0E2E, 0x0E2F,
    0x0E30, 0x0E31, 0x0E32, 0x0E33, 0x0E34, 0x0E35, 0x0E36, 0x0E37, 0x0E38, 0x0E39, 0x0E3A, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0x0E3F,
    0x0E40, 0x0E41, 0x0E42, 0x0E43, 0x0E44, 0x0E45, 0x0E46, 0x0E47, 0x0E48, 0x0E49, 0x0E4A, 0x0E4B, 0x0E4C, 0x0E4D, 0x0E4E, 0x0E4F,
    0x0E50, 0x0E51, 0x0E52, 0x0E53, 0x0E54, 0x0E55, 0x0E56, 0x0E57, 0x0E58, 0x0E59, 0x0E5A, 0x0E5B, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD
};

// From https://encoding.spec.whatwg.org/index-windows-1253.txt with 0xFFFD filling the gaps
static constexpr SingleByteDecodeTable windows1253 {
    0x20AC, 0x0081, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021, 0x0088, 0x2030, 0x008A, 0x2039, 0x008C, 0x008D, 0x008E, 0x008F,
    0x0090, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014, 0x0098, 0x2122, 0x009A, 0x203A, 0x009C, 0x009D, 0x009E, 0x009F,
    0x00A0, 0x0385, 0x0386, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7, 0x00A8, 0x00A9, 0xFFFD, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x2015,
    0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x0384, 0x00B5, 0x00B6, 0x00B7, 0x0388, 0x0389, 0x038A, 0x00BB, 0x038C, 0x00BD, 0x038E, 0x038F,
    0x0390, 0x0391, 0x0392, 0x0393, 0x0394, 0x0395, 0x0396, 0x0397, 0x0398, 0x0399, 0x039A, 0x039B, 0x039C, 0x039D, 0x039E, 0x039F,
    0x03A0, 0x03A1, 0xFFFD, 0x03A3, 0x03A4, 0x03A5, 0x03A6, 0x03A7, 0x03A8, 0x03A9, 0x03AA, 0x03AB, 0x03AC, 0x03AD, 0x03AE, 0x03AF,
    0x03B0, 0x03B1, 0x03B2, 0x03B3, 0x03B4, 0x03B5, 0x03B6, 0x03B7, 0x03B8, 0x03B9, 0x03BA, 0x03BB, 0x03BC, 0x03BD, 0x03BE, 0x03BF,
    0x03C0, 0x03C1, 0x03C2, 0x03C3, 0x03C4, 0x03C5, 0x03C6, 0x03C7, 0x03C8, 0x03C9, 0x03CA, 0x03CB, 0x03CC, 0x03CD, 0x03CE, 0xFFFD
};

static constexpr SingleByteDecodeTable windows1255 {
    0x20AC, 0x0081, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021, 0x02C6, 0x2030, 0x008A, 0x2039, 0x008C, 0x008D, 0x008E, 0x008F,
    0x0090, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014, 0x02DC, 0x2122, 0x009A, 0x203A, 0x009C, 0x009D, 0x009E, 0x009F,
    0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x20AA, 0x00A5, 0x00A6, 0x00A7, 0x00A8, 0x00A9, 0x00D7, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF,
    0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7, 0x00B8, 0x00B9, 0x00F7, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF,
    0x05B0, 0x05B1, 0x05B2, 0x05B3, 0x05B4, 0x05B5, 0x05B6, 0x05B7, 0x05B8, 0x05B9, 0x05BA, 0x05BB, 0x05BC, 0x05BD, 0x05BE, 0x05BF,
    0x05C0, 0x05C1, 0x05C2, 0x05C3, 0x05F0, 0x05F1, 0x05F2, 0x05F3, 0x05F4, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD,
    0x05D0, 0x05D1, 0x05D2, 0x05D3, 0x05D4, 0x05D5, 0x05D6, 0x05D7, 0x05D8, 0x05D9, 0x05DA, 0x05DB, 0x05DC, 0x05DD, 0x05DE, 0x05DF,
    0x05E0, 0x05E1, 0x05E2, 0x05E3, 0x05E4, 0x05E5, 0x05E6, 0x05E7, 0x05E8, 0x05E9, 0x05EA, 0xFFFD, 0xFFFD, 0x200E, 0x200F, 0xFFFD
};

// From https://encoding.spec.whatwg.org/index-windows-1257.txt with 0xFFFD filling the gaps
static constexpr SingleByteDecodeTable windows1257 {
    0x20AC, 0x0081, 0x201A, 0x0083, 0x201E, 0x2026, 0x2020, 0x2021, 0x0088, 0x2030, 0x008A, 0x2039, 0x008C, 0x00A8, 0x02C7, 0x00B8,
    0x0090, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014, 0x0098, 0x2122, 0x009A, 0x203A, 0x009C, 0x00AF, 0x02DB, 0x009F,
    0x00A0, 0xFFFD, 0x00A2, 0x00A3, 0x00A4, 0xFFFD, 0x00A6, 0x00A7, 0x00D8, 0x00A9, 0x0156, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00C6,
    0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7, 0x00F8, 0x00B9, 0x0157, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00E6,
    0x0104, 0x012E, 0x0100, 0x0106, 0x00C4, 0x00C5, 0x0118, 0x0112, 0x010C, 0x00C9, 0x0179, 0x0116, 0x0122, 0x0136, 0x012A, 0x013B,
    0x0160, 0x0143, 0x0145, 0x00D3, 0x014C, 0x00D5, 0x00D6, 0x00D7, 0x0172, 0x0141, 0x015A, 0x016A, 0x00DC, 0x017B, 0x017D, 0x00DF,
    0x0105, 0x012F, 0x0101, 0x0107, 0x00E4, 0x00E5, 0x0119, 0x0113, 0x010D, 0x00E9, 0x017A, 0x0117, 0x0123, 0x0137, 0x012B, 0x013C,
    0x0161, 0x0144, 0x0146, 0x00F3, 0x014D, 0x00F5, 0x00F6, 0x00F7, 0x0173, 0x0142, 0x015B, 0x016B, 0x00FC, 0x017C, 0x017E, 0x02D9
};

// From https://encoding.spec.whatwg.org/index-koi8-u.txt
static constexpr SingleByteDecodeTable koi8u {
    0x2500, 0x2502, 0x250C, 0x2510, 0x2514, 0x2518, 0x251C, 0x2524, 0x252C, 0x2534, 0x253C, 0x2580, 0x2584, 0x2588, 0x258C, 0x2590,
    0x2591, 0x2592, 0x2593, 0x2320, 0x25A0, 0x2219, 0x221A, 0x2248, 0x2264, 0x2265, 0x00A0, 0x2321, 0x00B0, 0x00B2, 0x00B7, 0x00F7,
    0x2550, 0x2551, 0x2552, 0x0451, 0x0454, 0x2554, 0x0456, 0x0457, 0x2557, 0x2558, 0x2559, 0x255A, 0x255B, 0x0491, 0x045E, 0x255E,
    0x255F, 0x2560, 0x2561, 0x0401, 0x0404, 0x2563, 0x0406, 0x0407, 0x2566, 0x2567, 0x2568, 0x2569, 0x256A, 0x0490, 0x040E, 0x00A9,
    0x044E, 0x0430, 0x0431, 0x0446, 0x0434, 0x0435, 0x0444, 0x0433, 0x0445, 0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E,
    0x043F, 0x044F, 0x0440, 0x0441, 0x0442, 0x0443, 0x0436, 0x0432, 0x044C, 0x044B, 0x0437, 0x0448, 0x044D, 0x0449, 0x0447, 0x044A,
    0x042E, 0x0410, 0x0411, 0x0426, 0x0414, 0x0415, 0x0424, 0x0413, 0x0425, 0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E,
    0x041F, 0x042F, 0x0420, 0x0421, 0x0422, 0x0423, 0x0416, 0x0412, 0x042C, 0x042B, 0x0417, 0x0428, 0x042D, 0x0429, 0x0427, 0x042A
};

// From https://encoding.spec.whatwg.org/index-ibm866.txt
static constexpr SingleByteDecodeTable ibm866 {
    0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417, 0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E, 0x041F,
    0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427, 0x0428, 0x0429, 0x042A, 0x042B, 0x042C, 0x042D, 0x042E, 0x042F,
    0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437, 0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E, 0x043F,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556, 0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
    0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B, 0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
    0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447, 0x0448, 0x0449, 0x044A, 0x044B, 0x044C, 0x044D, 0x044E, 0x044F,
    0x0401, 0x0451, 0x0404, 0x0454, 0x0407, 0x0457, 0x040E, 0x045E, 0x00B0, 0x2219, 0x00B7, 0x221A, 0x2116, 0x00A4, 0x25A0, 0x00A0
};

template<const SingleByteDecodeTable& decodeTable> SingleByteEncodeTable tableForEncoding()
{
    // Allocate this at runtime because building it at compile time would make the binary much larger and this is often not used.
    // FIXME: With the C++20 version of std::count, we should be able to change this from const to constexpr and compute the size at compile time.
    static const auto size = std::size(decodeTable) - std::count(std::begin(decodeTable), std::end(decodeTable), replacementCharacter);
    static const SingleByteEncodeTableEntry* entries;
    static std::once_flag once;
    std::call_once(once, [&] {
        auto* mutableEntries = new SingleByteEncodeTableEntry[size];
        size_t j = 0;
        for (uint8_t i = 0; i < std::size(decodeTable); i++) {
            if (decodeTable[i] != replacementCharacter)
                mutableEntries[j++] = { decodeTable[i], i + 0x80 };
        }
        ASSERT(j == size);
        auto collection = WTF::makeIteratorRange(&mutableEntries[0], &mutableEntries[size]);
        sortByFirst(collection);
        ASSERT(sortedFirstsAreUnique(collection));
        entries = mutableEntries;
    });
    return WTF::makeIteratorRange(&entries[0], &entries[size]);
}

static SingleByteEncodeTable tableForEncoding(TextCodecSingleByte::Encoding encoding)
{
    switch (encoding) {
    case TextCodecSingleByte::Encoding::ISO_8859_3:
        return tableForEncoding<iso88593>();
    case TextCodecSingleByte::Encoding::ISO_8859_6:
        return tableForEncoding<iso88596>();
    case TextCodecSingleByte::Encoding::ISO_8859_7:
        return tableForEncoding<iso88597>();
    case TextCodecSingleByte::Encoding::ISO_8859_8:
        return tableForEncoding<iso88598>();
    case TextCodecSingleByte::Encoding::Windows_874:
        return tableForEncoding<windows874>();
    case TextCodecSingleByte::Encoding::Windows_1253:
        return tableForEncoding<windows1253>();
    case TextCodecSingleByte::Encoding::Windows_1255:
        return tableForEncoding<windows1255>();
    case TextCodecSingleByte::Encoding::Windows_1257:
        return tableForEncoding<windows1257>();
    case TextCodecSingleByte::Encoding::IBM866:
        return tableForEncoding<ibm866>();
    case TextCodecSingleByte::Encoding::KOI8U:
        return tableForEncoding<koi8u>();
    }
    RELEASE_ASSERT_NOT_REACHED();
}

static const SingleByteDecodeTable& tableForDecoding(TextCodecSingleByte::Encoding encoding)
{
    switch (encoding) {
    case TextCodecSingleByte::Encoding::ISO_8859_3:
        return iso88593;
    case TextCodecSingleByte::Encoding::ISO_8859_6:
        return iso88596;
    case TextCodecSingleByte::Encoding::ISO_8859_7:
        return iso88597;
    case TextCodecSingleByte::Encoding::ISO_8859_8:
        return iso88598;
    case TextCodecSingleByte::Encoding::Windows_874:
        return windows874;
    case TextCodecSingleByte::Encoding::Windows_1253:
        return windows1253;
    case TextCodecSingleByte::Encoding::Windows_1255:
        return windows1255;
    case TextCodecSingleByte::Encoding::Windows_1257:
        return windows1257;
    case TextCodecSingleByte::Encoding::IBM866:
        return ibm866;
    case TextCodecSingleByte::Encoding::KOI8U:
        return koi8u;
    }
    RELEASE_ASSERT_NOT_REACHED();
}

// https://encoding.spec.whatwg.org/#single-byte-encoder
static Vector<uint8_t> encode(const SingleByteEncodeTable& table, StringView string, Function<void(char32_t, Vector<uint8_t>&)>&& unencodableHandler)
{
    // FIXME: Consider adding an ASCII fast path like the one in TextCodecLatin1::decode.
    Vector<uint8_t> result;
    result.reserveInitialCapacity(string.length());
    for (auto codePoint : string.codePoints()) {
        if (isASCII(codePoint)) {
            result.append(codePoint);
            continue;
        }
        auto byte = findFirstInSortedPairs(table, codePoint);
        if (!byte) {
            unencodableHandler(codePoint, result);
            continue;
        }
        result.append(*byte);
    }
    return result;
}

// https://encoding.spec.whatwg.org/#single-byte-decoder
static String decode(const SingleByteDecodeTable& table, const uint8_t* bytes, size_t length, bool, bool stopOnError, bool& sawError)
{
    StringBuilder result;
    result.reserveCapacity(length);
    auto parseByte = [&] (uint8_t byte) {
        if (isASCII(byte)) {
            result.append(byte);
            return;
        }
        UChar codePoint = table[byte - 0x80];
        if (codePoint == replacementCharacter)
            sawError = true;
        result.append(codePoint);
    };
    if (stopOnError) {
        for (size_t i = 0; i < length; i++) {
            parseByte(bytes[i]);
            if (sawError)
                return result.toString();
        }
    } else {
        for (size_t i = 0; i < length; i++)
            parseByte(bytes[i]);
    }
    return result.toString();
}

Vector<uint8_t> TextCodecSingleByte::encode(StringView string, UnencodableHandling handling) const
{
    return PAL::encode(tableForEncoding(m_encoding), string, unencodableHandler(handling));
}

String TextCodecSingleByte::decode(const char* bytes, size_t length, bool flush, bool stopOnError, bool& sawError)
{
    return PAL::decode(tableForDecoding(m_encoding), reinterpret_cast<const uint8_t*>(bytes), length, flush, stopOnError, sawError);
}

TextCodecSingleByte::TextCodecSingleByte(Encoding encoding)
    : m_encoding(encoding)
{
}

void TextCodecSingleByte::registerEncodingNames(EncodingNameRegistrar registrar)
{
    // https://encoding.spec.whatwg.org/#names-and-labels
    auto registerAliases = [&] (std::initializer_list<ASCIILiteral> list) {
        for (auto& alias : list)
            registrar(alias, *list.begin());
    };
    registerAliases({
        "ISO-8859-3"_s,
        "csisolatin3"_s,
        "iso-ir-109"_s,
        "iso8859-3"_s,
        "iso88593"_s,
        "iso_8859-3"_s,
        "iso_8859-3:1988"_s,
        "l3"_s,
        "latin3"_s
    });

    registerAliases({
        "ISO-8859-6"_s,
        "arabic"_s,
        "asmo-708"_s,
        "csiso88596e"_s,
        "csiso88596i"_s,
        "csisolatinarabic"_s,
        "ecma-114"_s,
        "iso-8859-6-e"_s,
        "iso-8859-6-i"_s,
        "iso-ir-127"_s,
        "iso8859-6"_s,
        "iso88596"_s,
        "iso_8859-6"_s,
        "iso_8859-6:1987"_s
    });

    registerAliases({
        "ISO-8859-7"_s,
        "csisolatingreek"_s,
        "ecma-118"_s,
        "elot_928"_s,
        "greek"_s,
        "greek8"_s,
        "iso-ir-126"_s,
        "iso8859-7"_s,
        "iso88597"_s,
        "iso_8859-7"_s,
        "iso_8859-7:1987"_s,
        "sun_eu_greek"_s
    });

    registerAliases({
        "ISO-8859-8"_s,
        "csiso88598e"_s,
        "csisolatinhebrew"_s,
        "hebrew"_s,
        "iso-8859-8-e"_s,
        "iso-ir-138"_s,
        "iso8859-8"_s,
        "iso88598"_s,
        "iso_8859-8"_s,
        "iso_8859-8:1988"_s,
        "visual"_s
    });

    registerAliases({
        "ISO-8859-8-I"_s,
        "csiso88598i"_s,
        "logical"_s
    });

    registerAliases({
        "windows-874"_s,
        "dos-874"_s,
        "iso-8859-11"_s,
        "iso8859-11"_s,
        "iso885911"_s,
        "tis-620"_s
    });

    registerAliases({
        "windows-1253"_s,
        "cp1253"_s,
        "x-cp1253"_s
    });
    
    registerAliases({
        "windows-1255"_s,
        "cp1255"_s,
        "x-cp1255"_s
    });
    
    registerAliases({
        "windows-1257"_s,
        "cp1257"_s,
        "x-cp1257"_s
    });
    
    registerAliases({
        "KOI8-U"_s,
        "koi8-ru"_s
    });

    registerAliases({
        "IBM866"_s,
        "866"_s,
        "cp866"_s,
        "csibm866"_s
    });
}

void TextCodecSingleByte::registerCodecs(TextCodecRegistrar registrar)
{
    registrar("ISO-8859-3"_s, [] {
        return makeUnique<TextCodecSingleByte>(Encoding::ISO_8859_3);
    });
    registrar("ISO-8859-6"_s, [] {
        return makeUnique<TextCodecSingleByte>(Encoding::ISO_8859_6);
    });
    registrar("ISO-8859-7"_s, [] {
        return makeUnique<TextCodecSingleByte>(Encoding::ISO_8859_7);
    });
    registrar("ISO-8859-8"_s, [] {
        return makeUnique<TextCodecSingleByte>(Encoding::ISO_8859_8);
    });
    registrar("ISO-8859-8-I"_s, [] {
        return makeUnique<TextCodecSingleByte>(Encoding::ISO_8859_8);
    });
    registrar("windows-874"_s, [] {
        return makeUnique<TextCodecSingleByte>(Encoding::Windows_874);
    });
    registrar("windows-1253"_s, [] {
        return makeUnique<TextCodecSingleByte>(Encoding::Windows_1253);
    });
    registrar("windows-1255"_s, [] {
        return makeUnique<TextCodecSingleByte>(Encoding::Windows_1255);
    });
    registrar("windows-1257"_s, [] {
        return makeUnique<TextCodecSingleByte>(Encoding::Windows_1257);
    });
    registrar("KOI8-U"_s, [] {
        return makeUnique<TextCodecSingleByte>(Encoding::KOI8U);
    });
    registrar("IBM866"_s, [] {
        return makeUnique<TextCodecSingleByte>(Encoding::IBM866);
    });
}

}
