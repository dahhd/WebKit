/*
 * Copyright (C) 2022 Apple Inc. All rights reserved.
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
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <wtf/Forward.h>
#include <wtf/OptionSet.h>
#include <wtf/text/AtomString.h>

namespace WebCore {

class CSSValue;
class Element;

namespace CQ {

struct ContainerCondition;
struct SizeCondition;
struct SizeFeature;

struct UnknownQuery {
    String name;
    String text;
};

using ContainerQuery = std::variant<ContainerCondition, SizeFeature, UnknownQuery>;

enum class LogicalOperator : uint8_t { And, Or, Not };
enum class ComparisonOperator : uint8_t { LessThan, LessThanOrEqual, Equal, GreaterThan, GreaterThanOrEqual };
enum class Syntax : uint8_t { Boolean, Colon, Range };

struct ContainerCondition {
    LogicalOperator logicalOperator { LogicalOperator::And };
    Vector<ContainerQuery> queries;
};

struct Comparison {
    ComparisonOperator op { ComparisonOperator::Equal };
    RefPtr<CSSValue> value;
};

struct SizeFeature {
    AtomString name;
    Syntax syntax;
    std::optional<Comparison> leftComparison;
    std::optional<Comparison> rightComparison;
};

namespace FeatureNames {
const AtomString& width();
const AtomString& height();
const AtomString& inlineSize();
const AtomString& blockSize();
const AtomString& aspectRatio();
const AtomString& orientation();
};

enum class Axis : uint8_t {
    Block   = 1 << 0,
    Inline  = 1 << 1,
    Width   = 1 << 2,
    Height  = 1 << 3,
};
OptionSet<Axis> requiredAxesForFeature(const AtomString&);

}

using ContainerQuery = CQ::ContainerQuery;

struct FilteredContainerQuery {
    AtomString nameFilter;
    OptionSet<CQ::Axis> axisFilter;
    ContainerQuery query;
};

using CachedQueryContainers = Vector<Ref<const Element>>;

void serialize(StringBuilder&, const ContainerQuery&);

}
