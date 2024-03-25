/*
 * Copyright (C) 1999-2003 Lars Knoll (knoll@kde.org)
 *               1999 Waldo Bastian (bastian@kde.org)
 * Copyright (C) 2004-2022 Apple Inc. All rights reserved.
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

#include "CSSParserContext.h"
#include "QualifiedName.h"
#include "RenderStyleConstants.h"
#include <wtf/EnumTraits.h>
#include <wtf/FixedVector.h>

namespace WebCore {

class CSSSelectorList;
struct CSSSelectorParserContext;

struct PossiblyQuotedIdentifier {
    AtomString identifier;
    bool wasQuoted { false };

    bool isNull() const { return identifier.isNull(); }
};

    enum class SelectorSpecificityIncrement {
        ClassA = 0x10000,
        ClassB = 0x100,
        ClassC = 1
    };

    // Selector for a StyleRule.
    DECLARE_ALLOCATOR_WITH_HEAP_IDENTIFIER(CSSSelectorRareData);
    class CSSSelector {
        WTF_MAKE_FAST_ALLOCATED;
    public:
        CSSSelector() = default;
        CSSSelector(const CSSSelector&);
        explicit CSSSelector(const QualifiedName&, bool tagIsForNamespaceRule = false);

        ~CSSSelector();

        // Re-create selector text from selector's data.
        String selectorText(StringView separator = { }, StringView rightSide = { }) const;

        unsigned computeSpecificity() const;
        std::array<uint8_t, 3> computeSpecificityTuple() const;
        unsigned specificityForPage() const;

        bool visitAllSimpleSelectors(auto& apply) const;

        bool hasExplicitNestingParent() const;
        bool hasExplicitPseudoClassScope() const;
        void resolveNestingParentSelectors(const CSSSelectorList& parent);
        void replaceNestingParentByPseudoClassScope();

        // How the attribute value has to match. Default is Exact.
        enum class Match : uint8_t {
            Unknown = 0,
            Tag,
            Id,
            Class,
            Exact,
            Set,
            List,
            Hyphen,
            PseudoClass,
            PseudoElement,
            Contain, // css3: E[foo*="bar"]
            Begin, // css3: E[foo^="bar"]
            End, // css3: E[foo$="bar"]
            PagePseudoClass,
            NestingParent, // &
            ForgivingUnknown,
            ForgivingUnknownNestContaining
        };

        enum class Relation : uint8_t {
            Subselector,
            DescendantSpace,
            Child,
            DirectAdjacent,
            IndirectAdjacent,
            ShadowDescendant,
            ShadowPartDescendant,
            ShadowSlotted
        };

        enum class PseudoClass : uint8_t {
            Empty,
            FirstChild,
            FirstOfType,
            LastChild,
            LastOfType,
            OnlyChild,
            OnlyOfType,
            NthChild,
            NthOfType,
            NthLastChild,
            NthLastOfType,
            Link,
            Visited,
            Any,
            AnyLink,
            AnyLinkDeprecated,
            Autofill,
            AutofillAndObscured,
            AutofillStrongPassword,
            AutofillStrongPasswordViewable,
            Hover,
            Drag,
            Focus,
            FocusVisible,
            FocusWithin,
            Active,
            Checked,
            Enabled,
            FullPageMedia,
            Default,
            Disabled,
            HtmlDocument, // for internal use only with forms in table case in UA stylesheet
            Is,
            Matches, // obsolete synonym for PseudoClassIs
            Where,
            Optional,
            PlaceholderShown,
            Required,
            ReadOnly,
            ReadWrite,
            Valid,
            Invalid,
            Indeterminate,
            Target,
            Lang,
            Not,
            Root,
            Scope,
            State,
            HasScope, // for internal use, matches the :has() scope
            WindowInactive,
            CornerPresent,
            Decrement,
            Increment,
            Has,
            Horizontal,
            Vertical,
            Start,
            End,
            DoubleButton,
            SingleButton,
            NoButton,
#if ENABLE(FULLSCREEN_API)
            Fullscreen,
            WebkitFullScreen,
            FullScreenDocument,
            FullScreenAncestor,
            AnimatingFullScreenTransition,
            FullScreenControlsHidden,
#endif
#if ENABLE(PICTURE_IN_PICTURE_API)
            PictureInPicture,
#endif
            InRange,
            OutOfRange,
#if ENABLE(VIDEO)
            Future,
            Past,
            Playing,
            Paused,
            Seeking,
            Buffering,
            Stalled,
            Muted,
            VolumeLocked,
#endif
            Dir,
            Host,
            Defined,
#if ENABLE(ATTACHMENT_ELEMENT)
            HasAttachment,
#endif
            Modal,
            PopoverOpen,
            UserInvalid,
            UserValid
        };

        enum class PseudoElement : uint8_t {
            After,
            Backdrop,
            Before,
#if ENABLE(VIDEO)
            Cue,
#endif
            FirstLetter,
            FirstLine,
            GrammarError,
            Highlight,
            Marker,
            Part,
            Resizer,
            Scrollbar,
            ScrollbarButton,
            ScrollbarCorner,
            ScrollbarThumb,
            ScrollbarTrack,
            ScrollbarTrackPiece,
            Selection,
            Slotted,
            SpellingError,
            ViewTransition,
            ViewTransitionGroup,
            ViewTransitionImagePair,
            ViewTransitionOld,
            ViewTransitionNew,
            WebKitCustom,

            // WebKitCustom that appeared in an old prefixed form
            // and need special handling.
            WebKitCustomLegacyPrefixed,
        };

        enum class PagePseudoClass : uint8_t {
            First,
            Left,
            Right,
        };

        static PseudoId pseudoId(PseudoElement);
        static std::optional<PseudoElement> parsePseudoElement(StringView, const CSSSelectorParserContext&);
        static std::optional<PseudoId> parseStandalonePseudoElement(StringView, const CSSSelectorParserContext&);

        // Selectors are kept in an array by CSSSelectorList.
        // The next component of the selector is the next item in the array.
        const CSSSelector* tagHistory() const { return m_isLastInTagHistory ? nullptr : this + 1; }
        const CSSSelector* firstInCompound() const;

        const QualifiedName& tagQName() const;
        const AtomString& tagLowercaseLocalName() const;

        const AtomString& value() const;
        const AtomString& serializingValue() const;
        const QualifiedName& attribute() const;
        const AtomString& argument() const { return m_hasRareData ? m_data.rareData->argument : nullAtom(); }
        bool attributeValueMatchingIsCaseInsensitive() const;
        const FixedVector<PossiblyQuotedIdentifier>* argumentList() const { return m_hasRareData ? &m_data.rareData->argumentList : nullptr; }
        const CSSSelectorList* selectorList() const { return m_hasRareData ? m_data.rareData->selectorList.get() : nullptr; }
        CSSSelectorList* selectorList() { return m_hasRareData ? m_data.rareData->selectorList.get() : nullptr; }

        void setValue(const AtomString&, bool matchLowerCase = false);

        enum AttributeMatchType { CaseSensitive, CaseInsensitive };
        void setAttribute(const QualifiedName&, AttributeMatchType);
        void setNth(int a, int b);
        void setArgument(const AtomString&);
        void setArgumentList(FixedVector<PossiblyQuotedIdentifier>);
        void setSelectorList(std::unique_ptr<CSSSelectorList>);

        bool matchNth(int count) const;
        int nthA() const;
        int nthB() const;

        bool hasDescendantRelation() const { return relation() == Relation::DescendantSpace; }

        bool hasDescendantOrChildRelation() const { return relation() == Relation::Child || hasDescendantRelation(); }

        PseudoClass pseudoClass() const;
        void setPseudoClass(PseudoClass);

        PseudoElement pseudoElement() const;
        void setPseudoElement(PseudoElement);

        PagePseudoClass pagePseudoClass() const;
        void setPagePseudoClass(PagePseudoClass);

        bool matchesPseudoElement() const;
        bool isWebKitCustomPseudoElement() const;
        bool isSiblingSelector() const;
        bool isAttributeSelector() const;

        Relation relation() const { return static_cast<Relation>(m_relation); }
        void setRelation(Relation);

        Match match() const { return static_cast<Match>(m_match); }
        void setMatch(Match);

        bool isLastInSelectorList() const { return m_isLastInSelectorList; }
        void setLastInSelectorList() { m_isLastInSelectorList = true; }
        void setNotLastInSelectorList() { m_isLastInSelectorList = false; }

        bool isFirstInTagHistory() const { return m_isFirstInTagHistory; }
        void setNotFirstInTagHistory() { m_isFirstInTagHistory = false; }

        bool isLastInTagHistory() const { return m_isLastInTagHistory; }
        void setNotLastInTagHistory() { m_isLastInTagHistory = false; }
        void setLastInTagHistory() { m_isLastInTagHistory = true; }

        bool isForPage() const { return m_isForPage; }
        void setForPage() { m_isForPage = true; }

        void setImplicit() { m_isImplicit = true; }
        // Implicit means that this selector is not author/UA written.
        bool isImplicit() const { return m_isImplicit; }

    private:
        unsigned m_relation : 4 { enumToUnderlyingType(Relation::DescendantSpace) };
        mutable unsigned m_match : 5 { enumToUnderlyingType(Match::Unknown) };
        mutable unsigned m_pseudoType : 8 { 0 }; // PseudoType.
        // 17 bits
        unsigned m_isLastInSelectorList : 1 { false };
        unsigned m_isFirstInTagHistory : 1 { true };
        unsigned m_isLastInTagHistory : 1 { true };
        unsigned m_hasRareData : 1 { false };
        unsigned m_isForPage : 1 { false };
        unsigned m_tagIsForNamespaceRule : 1 { false };
        unsigned m_caseInsensitiveAttributeValueMatching : 1 { false };
        unsigned m_isImplicit : 1 { false };
        // 25 bits
#if !ASSERT_WITH_SECURITY_IMPLICATION_DISABLED
        unsigned m_destructorHasBeenCalled : 1 { false };
#endif

        unsigned simpleSelectorSpecificityForPage() const;
        CSSSelector* tagHistory() { return m_isLastInTagHistory ? nullptr : this + 1; }

        CSSSelector& operator=(const CSSSelector&) = delete;
        CSSSelector(CSSSelector&&) = delete;

        struct RareData : public RefCounted<RareData> {
            WTF_MAKE_STRUCT_FAST_ALLOCATED_WITH_HEAP_IDENTIFIER(CSSSelectorRareData);
            static Ref<RareData> create(AtomString);
            WEBCORE_EXPORT ~RareData();

            bool matchNth(int count);

            // For quirks mode, class and id are case-insensitive. In the case where uppercase
            // letters are used in quirks mode, |m_matchingValue| holds the lowercase class/id
            // and |m_serializingValue| holds the original string.
            AtomString matchingValue;
            AtomString serializingValue;

            int a { 0 }; // Used for :nth-*
            int b { 0 }; // Used for :nth-*
            QualifiedName attribute; // used for attribute selector
            AtomString argument; // Used for :contains and :nth-*
            FixedVector<PossiblyQuotedIdentifier> argumentList; // Used for :lang and ::part arguments.
            std::unique_ptr<CSSSelectorList> selectorList; // Used for :is(), :matches(), and :not().

            Ref<RareData> deepCopy() const;

        private:
            RareData(AtomString&& value);
            RareData(const RareData& other);
        };
        void createRareData();

        union DataUnion {
            AtomStringImpl* value { nullptr };
            QualifiedName::QualifiedNameImpl* tagQName;
            RareData* rareData;
        } m_data;
    };

inline bool operator==(const AtomString& a, const PossiblyQuotedIdentifier& b) { return a == b.identifier; }
inline bool operator==(const PossiblyQuotedIdentifier& a, const AtomString& b) { return a.identifier == b; }

inline const QualifiedName& CSSSelector::attribute() const
{
    ASSERT(isAttributeSelector());
    ASSERT(m_hasRareData);
    return m_data.rareData->attribute;
}

inline bool CSSSelector::matchesPseudoElement() const
{
    return match() == Match::PseudoElement;
}

inline bool CSSSelector::isWebKitCustomPseudoElement() const
{
    return pseudoElement() == PseudoElement::WebKitCustom || pseudoElement() == PseudoElement::WebKitCustomLegacyPrefixed;
}

static inline bool pseudoClassIsRelativeToSiblings(CSSSelector::PseudoClass type)
{
    return type == CSSSelector::PseudoClass::Empty
        || type == CSSSelector::PseudoClass::FirstChild
        || type == CSSSelector::PseudoClass::FirstOfType
        || type == CSSSelector::PseudoClass::LastChild
        || type == CSSSelector::PseudoClass::LastOfType
        || type == CSSSelector::PseudoClass::OnlyChild
        || type == CSSSelector::PseudoClass::OnlyOfType
        || type == CSSSelector::PseudoClass::NthChild
        || type == CSSSelector::PseudoClass::NthOfType
        || type == CSSSelector::PseudoClass::NthLastChild
        || type == CSSSelector::PseudoClass::NthLastOfType;
}

static inline bool isTreeStructuralPseudoClass(CSSSelector::PseudoClass type)
{
    return pseudoClassIsRelativeToSiblings(type) || type == CSSSelector::PseudoClass::Root;
}

inline bool isLogicalCombinationPseudoClass(CSSSelector::PseudoClass pseudoClass)
{
    switch (pseudoClass) {
    case CSSSelector::PseudoClass::Is:
    case CSSSelector::PseudoClass::Where:
    case CSSSelector::PseudoClass::Not:
    case CSSSelector::PseudoClass::Any:
    case CSSSelector::PseudoClass::Matches:
    case CSSSelector::PseudoClass::Has:
        return true;
    default:
        return false;
    }
}

inline bool CSSSelector::isSiblingSelector() const
{
    return relation() == Relation::DirectAdjacent
        || relation() == Relation::IndirectAdjacent
        || (match() == CSSSelector::Match::PseudoClass && pseudoClassIsRelativeToSiblings(pseudoClass()));
}

inline bool CSSSelector::isAttributeSelector() const
{
    return match() == CSSSelector::Match::Exact
        || match() == CSSSelector::Match::Set
        || match() == CSSSelector::Match::List
        || match() == CSSSelector::Match::Hyphen
        || match() == CSSSelector::Match::Contain
        || match() == CSSSelector::Match::Begin
        || match() == CSSSelector::Match::End;
}

inline void CSSSelector::setValue(const AtomString& value, bool matchLowerCase)
{
    ASSERT(match() != Match::Tag);
    AtomString matchingValue = matchLowerCase ? value.convertToASCIILowercase() : value;
    if (!m_hasRareData && matchingValue != value)
        createRareData();
    
    // Need to do ref counting manually for the union.
    if (!m_hasRareData) {
        if (m_data.value)
            m_data.value->deref();
        m_data.value = value.impl();
        m_data.value->ref();
        return;
    }

    m_data.rareData->matchingValue = WTFMove(matchingValue);
    m_data.rareData->serializingValue = value;
}

inline CSSSelector::~CSSSelector()
{
    ASSERT_WITH_SECURITY_IMPLICATION(!m_destructorHasBeenCalled);
#if !ASSERT_WITH_SECURITY_IMPLICATION_DISABLED
    m_destructorHasBeenCalled = true;
#endif
    if (m_hasRareData) {
        m_data.rareData->deref();
        m_data.rareData = nullptr;
        m_hasRareData = false;
    } else if (match() == Match::Tag) {
        m_data.tagQName->deref();
        m_data.tagQName = nullptr;
        m_match = enumToUnderlyingType(Match::Unknown);
    } else if (m_data.value) {
        m_data.value->deref();
        m_data.value = nullptr;
    }
}

inline const QualifiedName& CSSSelector::tagQName() const
{
    return *reinterpret_cast<const QualifiedName*>(&m_data.tagQName);
}

inline const AtomString& CSSSelector::tagLowercaseLocalName() const
{
    return tagQName().localNameLowercase();
}

inline const AtomString& CSSSelector::value() const
{
    ASSERT(match() != Match::Tag);
    if (m_hasRareData)
        return m_data.rareData->matchingValue;

    // AtomString is really just an AtomStringImpl* so the cast below is safe.
    return *reinterpret_cast<const AtomString*>(&m_data.value);
}

inline const AtomString& CSSSelector::serializingValue() const
{
    ASSERT(match() != Match::Tag);
    if (m_hasRareData)
        return m_data.rareData->serializingValue;
    
    // AtomString is really just an AtomStringImpl* so the cast below is safe.
    return *reinterpret_cast<const AtomString*>(&m_data.value);
}

inline bool CSSSelector::attributeValueMatchingIsCaseInsensitive() const
{
    return m_caseInsensitiveAttributeValueMatching;
}

inline auto CSSSelector::pseudoClass() const -> PseudoClass
{
    ASSERT(match() == Match::PseudoClass);
    return static_cast<PseudoClass>(m_pseudoType);
}

inline void CSSSelector::setPseudoClass(PseudoClass pseudoClass)
{
    m_pseudoType = enumToUnderlyingType(pseudoClass);
    ASSERT(static_cast<PseudoClass>(m_pseudoType) == pseudoClass);
}

inline auto CSSSelector::pseudoElement() const -> PseudoElement
{
    ASSERT(match() == Match::PseudoElement);
    return static_cast<PseudoElement>(m_pseudoType);
}

inline void CSSSelector::setPseudoElement(PseudoElement pseudoElement)
{
    m_pseudoType = enumToUnderlyingType(pseudoElement);
    ASSERT(static_cast<PseudoElement>(m_pseudoType) == pseudoElement);
}

inline auto CSSSelector::pagePseudoClass() const -> PagePseudoClass
{
    ASSERT(match() == Match::PagePseudoClass);
    return static_cast<PagePseudoClass>(m_pseudoType);
}

inline void CSSSelector::setPagePseudoClass(PagePseudoClass pagePseudoClass)
{
    m_pseudoType = enumToUnderlyingType(pagePseudoClass);
    ASSERT(static_cast<PagePseudoClass>(m_pseudoType) == pagePseudoClass);
}

inline void CSSSelector::setRelation(Relation relation)
{
    m_relation = enumToUnderlyingType(relation);
}

inline void CSSSelector::setMatch(Match match)
{
    m_match = enumToUnderlyingType(match);
}

} // namespace WebCore
