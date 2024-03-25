/*
 * Copyright (C) 2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2008, 2009, 2010, 2014 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
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

#pragma once

#include "CSSSelector.h"
#include <wtf/text/AtomStringHash.h>

namespace WebCore {

struct CSSSelectorParserContext;

enum class CSSParserSelectorCombinator {
    Child,
    DescendantSpace,
    DirectAdjacent,
    IndirectAdjacent
};

class CSSParserSelector {
    WTF_MAKE_FAST_ALLOCATED;
public:
    static std::unique_ptr<CSSParserSelector> parsePseudoClassSelector(StringView);
    static std::unique_ptr<CSSParserSelector> parsePseudoElementSelector(StringView, const CSSSelectorParserContext&);
    static std::unique_ptr<CSSParserSelector> parsePagePseudoSelector(StringView);

    CSSParserSelector();

    // Recursively copy the selector chain.
    CSSParserSelector(const CSSSelector&);

    explicit CSSParserSelector(const QualifiedName&);

    ~CSSParserSelector();

    std::unique_ptr<CSSSelector> releaseSelector() { return WTFMove(m_selector); }
    const CSSSelector* selector() const { return m_selector.get(); };
    CSSSelector* selector() { return m_selector.get(); }

    void setValue(const AtomString& value, bool matchLowerCase = false) { m_selector->setValue(value, matchLowerCase); }

    void setAttribute(const QualifiedName& value, CSSSelector::AttributeMatchType type) { m_selector->setAttribute(value, type); }
    
    void setArgument(const AtomString& value) { m_selector->setArgument(value); }
    void setNth(int a, int b) { m_selector->setNth(a, b); }
    void setMatch(CSSSelector::Match value) { m_selector->setMatch(value); }
    void setRelation(CSSSelector::Relation value) { m_selector->setRelation(value); }
    void setForPage() { m_selector->setForPage(); }

    CSSSelector::Match match() const { return m_selector->match(); }
    CSSSelector::PseudoElement pseudoElement() const { return m_selector->pseudoElement(); }
    const CSSSelectorList* selectorList() const { return m_selector->selectorList(); }
    
    void setPseudoElement(CSSSelector::PseudoElement type) { m_selector->setPseudoElement(type); }
    void setPseudoClass(CSSSelector::PseudoClass type) { m_selector->setPseudoClass(type); }

    void adoptSelectorVector(Vector<std::unique_ptr<CSSParserSelector>>&&);
    void setArgumentList(FixedVector<PossiblyQuotedIdentifier>);
    void setSelectorList(std::unique_ptr<CSSSelectorList>);

    CSSSelector::PseudoClass pseudoClass() const { return m_selector->pseudoClass(); }

    bool isPseudoElementCueFunction() const;

    bool matchesPseudoElement() const;

    bool isHostPseudoSelector() const;

    bool hasExplicitNestingParent() const;
    bool hasExplicitPseudoClassScope() const;

    // FIXME-NEWPARSER: "slotted" was removed here for now, since it leads to a combinator
    // connection of ShadowDescendant, and the current shadow DOM code doesn't expect this. When
    // we do fix this issue, make sure to patch the namespace prependTag code to remove the slotted
    // special case, since it will be covered by this function once again.
    bool needsImplicitShadowCombinatorForMatching() const;

    CSSParserSelector* tagHistory() const { return m_tagHistory.get(); }
    CSSParserSelector* leftmostSimpleSelector();
    const CSSParserSelector* leftmostSimpleSelector() const;
    bool startsWithExplicitCombinator() const;
    void setTagHistory(std::unique_ptr<CSSParserSelector> selector) { m_tagHistory = WTFMove(selector); }
    void clearTagHistory() { m_tagHistory.reset(); }
    void insertTagHistory(CSSSelector::Relation before, std::unique_ptr<CSSParserSelector>, CSSSelector::Relation after);
    void appendTagHistory(CSSSelector::Relation, std::unique_ptr<CSSParserSelector>);
    void appendTagHistory(CSSParserSelectorCombinator, std::unique_ptr<CSSParserSelector>);
    void appendTagHistoryAsRelative(std::unique_ptr<CSSParserSelector>);
    void prependTagSelector(const QualifiedName&, bool tagIsForNamespaceRule = false);
    std::unique_ptr<CSSParserSelector> releaseTagHistory();

private:
    std::unique_ptr<CSSSelector> m_selector;
    std::unique_ptr<CSSParserSelector> m_tagHistory;
};

inline bool CSSParserSelector::needsImplicitShadowCombinatorForMatching() const
{
    return match() == CSSSelector::Match::PseudoElement
        && (pseudoElement() == CSSSelector::PseudoElement::WebKitCustom
#if ENABLE(VIDEO)
            || pseudoElement() == CSSSelector::PseudoElement::Cue
#endif
            || pseudoElement() == CSSSelector::PseudoElement::Part
            || pseudoElement() == CSSSelector::PseudoElement::Slotted
            || pseudoElement() == CSSSelector::PseudoElement::WebKitCustomLegacyPrefixed);
}

inline bool CSSParserSelector::isPseudoElementCueFunction() const
{
#if ENABLE(VIDEO)
    return m_selector->match() == CSSSelector::Match::PseudoElement && m_selector->pseudoElement() == CSSSelector::PseudoElement::Cue;
#else
    return false;
#endif
}

}
