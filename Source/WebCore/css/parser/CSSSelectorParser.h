// Copyright 2014 The Chromium Authors. All rights reserved.
// Copyright (C) 2016-2020 Apple Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "CSSParserContext.h"
#include "CSSParserEnum.h"
#include "CSSParserSelector.h"
#include "CSSParserTokenRange.h"
#include "CSSSelectorList.h"
#include "CSSSelectorParserContext.h"
#include "StyleSheetContents.h"

namespace WebCore {

class CSSParserTokenRange;
class CSSSelectorList;
class StyleSheetContents;
class StyleRule;

using CSSParserSelectorList = Vector<std::unique_ptr<CSSParserSelector>>;

class CSSSelectorParser {
public:
    CSSSelectorParser(const CSSSelectorParserContext&, StyleSheetContents*, CSSParserEnum::IsNestedContext = CSSParserEnum::IsNestedContext::No);

    CSSParserSelectorList consumeComplexSelectorList(CSSParserTokenRange&);
    CSSParserSelectorList consumeNestedSelectorList(CSSParserTokenRange&);
    CSSParserSelectorList consumeComplexForgivingSelectorList(CSSParserTokenRange&);
    CSSParserSelectorList consumeNestedComplexForgivingSelectorList(CSSParserTokenRange&);

    static bool supportsComplexSelector(CSSParserTokenRange, const CSSSelectorParserContext&, CSSParserEnum::IsNestedContext);
    static CSSSelectorList resolveNestingParent(const CSSSelectorList& nestedSelectorList, const CSSSelectorList* parentResolvedSelectorList);

private:
    template<typename ConsumeSelector> CSSParserSelectorList consumeSelectorList(CSSParserTokenRange&, ConsumeSelector&&);
    template<typename ConsumeSelector> CSSParserSelectorList consumeForgivingSelectorList(CSSParserTokenRange&, ConsumeSelector&&);

    CSSParserSelectorList consumeCompoundSelectorList(CSSParserTokenRange&);
    CSSParserSelectorList consumeRelativeSelectorList(CSSParserTokenRange&);

    std::unique_ptr<CSSParserSelector> consumeComplexSelector(CSSParserTokenRange&);
    std::unique_ptr<CSSParserSelector> consumeNestedComplexSelector(CSSParserTokenRange&);
    std::unique_ptr<CSSParserSelector> consumeCompoundSelector(CSSParserTokenRange&);
    std::unique_ptr<CSSParserSelector> consumeRelativeScopeSelector(CSSParserTokenRange&);
    std::unique_ptr<CSSParserSelector> consumeRelativeNestedSelector(CSSParserTokenRange&);

    // This doesn't include element names, since they're handled specially.
    std::unique_ptr<CSSParserSelector> consumeSimpleSelector(CSSParserTokenRange&);

    bool consumeName(CSSParserTokenRange&, AtomString& name, AtomString& namespacePrefix);

    // These will return nullptr when the selector is invalid.
    std::unique_ptr<CSSParserSelector> consumeId(CSSParserTokenRange&);
    std::unique_ptr<CSSParserSelector> consumeClass(CSSParserTokenRange&);
    std::unique_ptr<CSSParserSelector> consumePseudo(CSSParserTokenRange&);
    std::unique_ptr<CSSParserSelector> consumeAttribute(CSSParserTokenRange&);
    std::unique_ptr<CSSParserSelector> consumeNesting(CSSParserTokenRange&);

    CSSSelector::Relation consumeCombinator(CSSParserTokenRange&);
    CSSSelector::Match consumeAttributeMatch(CSSParserTokenRange&);
    CSSSelector::AttributeMatchType consumeAttributeFlags(CSSParserTokenRange&);

    const AtomString& defaultNamespace() const;
    const AtomString& determineNamespace(const AtomString& prefix);
    void prependTypeSelectorIfNeeded(const AtomString& namespacePrefix, const AtomString& elementName, CSSParserSelector&);
    static std::unique_ptr<CSSParserSelector> splitCompoundAtImplicitShadowCrossingCombinator(std::unique_ptr<CSSParserSelector> compoundSelector, const CSSSelectorParserContext&);
    static bool containsUnknownWebKitPseudoElements(const CSSSelector& complexSelector);

    class DisallowPseudoElementsScope;

    const CSSSelectorParserContext m_context;
    const RefPtr<StyleSheetContents> m_styleSheet;
    CSSParserEnum::IsNestedContext m_isNestedContext { CSSParserEnum::IsNestedContext::No };

    // FIXME: This m_failedParsing is ugly and confusing, we should look into removing it (the return value of each function already convey this information).
    bool m_failedParsing { false };

    bool m_disallowPseudoElements { false };
    bool m_disallowHasPseudoClass { false };
    bool m_resistDefaultNamespace { false };
    bool m_ignoreDefaultNamespace { false };
    bool m_disableForgivingParsing { false };
    std::optional<CSSSelector::PseudoElement> m_precedingPseudoElement;
};

std::optional<CSSSelectorList> parseCSSSelectorList(CSSParserTokenRange, const CSSSelectorParserContext&, StyleSheetContents* = nullptr, CSSParserEnum::IsNestedContext = CSSParserEnum::IsNestedContext::No);
CSSParserSelectorList parseCSSParserSelectorList(CSSParserTokenRange&, const CSSSelectorParserContext&, StyleSheetContents*, CSSParserEnum::IsNestedContext, CSSParserEnum::IsForgiving);

} // namespace WebCore
