/*
 * Copyright (C) 2012, 2014-2015 Apple Inc. All rights reserved.
 * Copyright (C) 2019 Igalia S.L.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "ScrollingTreeFrameScrollingNodeNicosia.h"

#if ENABLE(ASYNC_SCROLLING) && USE(NICOSIA)

#include "LocalFrameView.h"
#include "Logging.h"
#include "NicosiaCompositionLayer.h"
#include "ScrollingStateFrameScrollingNode.h"
#include "ScrollingTreeScrollingNodeDelegateNicosia.h"
#include "ThreadedScrollingTree.h"

namespace WebCore {

Ref<ScrollingTreeFrameScrollingNode> ScrollingTreeFrameScrollingNodeNicosia::create(ScrollingTree& scrollingTree, ScrollingNodeType nodeType, ScrollingNodeID nodeID)
{
    return adoptRef(*new ScrollingTreeFrameScrollingNodeNicosia(scrollingTree, nodeType, nodeID));
}

ScrollingTreeFrameScrollingNodeNicosia::ScrollingTreeFrameScrollingNodeNicosia(ScrollingTree& scrollingTree, ScrollingNodeType nodeType, ScrollingNodeID nodeID)
    : ScrollingTreeFrameScrollingNode(scrollingTree, nodeType, nodeID)
{
    m_delegate = makeUnique<ScrollingTreeScrollingNodeDelegateNicosia>(*this, downcast<ThreadedScrollingTree>(scrollingTree).scrollAnimatorEnabled());
}

ScrollingTreeFrameScrollingNodeNicosia::~ScrollingTreeFrameScrollingNodeNicosia() = default;

ScrollingTreeScrollingNodeDelegateNicosia& ScrollingTreeFrameScrollingNodeNicosia::delegate() const
{
    return *static_cast<ScrollingTreeScrollingNodeDelegateNicosia*>(m_delegate.get());
}

bool ScrollingTreeFrameScrollingNodeNicosia::commitStateBeforeChildren(const ScrollingStateNode& stateNode)
{
    if (!ScrollingTreeFrameScrollingNode::commitStateBeforeChildren(stateNode))
        return false;

    if (!is<ScrollingStateFrameScrollingNode>(stateNode))
        return false;

    const auto& scrollingStateNode = downcast<ScrollingStateFrameScrollingNode>(stateNode);

    if (scrollingStateNode.hasChangedProperty(ScrollingStateNode::Property::RootContentsLayer)) {
        auto* layer = static_cast<Nicosia::PlatformLayer*>(scrollingStateNode.rootContentsLayer());
        m_rootContentsLayer = downcast<Nicosia::CompositionLayer>(layer);
    }
    if (scrollingStateNode.hasChangedProperty(ScrollingStateNode::Property::CounterScrollingLayer)) {
        auto* layer = static_cast<Nicosia::PlatformLayer*>(scrollingStateNode.counterScrollingLayer());
        m_counterScrollingLayer = downcast<Nicosia::CompositionLayer>(layer);
    }
    if (scrollingStateNode.hasChangedProperty(ScrollingStateNode::Property::InsetClipLayer)) {
        auto* layer = static_cast<Nicosia::PlatformLayer*>(scrollingStateNode.insetClipLayer());
        m_insetClipLayer = downcast<Nicosia::CompositionLayer>(layer);
    }
    if (scrollingStateNode.hasChangedProperty(ScrollingStateNode::Property::ContentShadowLayer)) {
        auto* layer = static_cast<Nicosia::PlatformLayer*>(scrollingStateNode.contentShadowLayer());
        m_contentShadowLayer = downcast<Nicosia::CompositionLayer>(layer);
    }
    if (scrollingStateNode.hasChangedProperty(ScrollingStateNode::Property::HeaderLayer)) {
        auto* layer = static_cast<Nicosia::PlatformLayer*>(scrollingStateNode.headerLayer());
        m_headerLayer = downcast<Nicosia::CompositionLayer>(layer);
    }
    if (scrollingStateNode.hasChangedProperty(ScrollingStateNode::Property::FooterLayer)) {
        auto* layer = static_cast<Nicosia::PlatformLayer*>(scrollingStateNode.footerLayer());
        m_footerLayer = downcast<Nicosia::CompositionLayer>(layer);
    }

    m_delegate->updateFromStateNode(scrollingStateNode);
    return true;
}

WheelEventHandlingResult ScrollingTreeFrameScrollingNodeNicosia::handleWheelEvent(const PlatformWheelEvent& wheelEvent, EventTargeting eventTargeting)
{
    if (!canHandleWheelEvent(wheelEvent, eventTargeting))
        return WheelEventHandlingResult::unhandled();

    bool handled = delegate().handleWheelEvent(wheelEvent);
    delegate().updateSnapScrollState();
    return WheelEventHandlingResult::result(handled);
}

void ScrollingTreeFrameScrollingNodeNicosia::currentScrollPositionChanged(ScrollType scrollType, ScrollingLayerPositionAction action)
{
    LOG_WITH_STREAM(Scrolling, stream << "ScrollingTreeFrameScrollingNodeNicosia::currentScrollPositionChanged to " << currentScrollPosition() << " min: " << minimumScrollPosition() << " max: " << maximumScrollPosition() << " sync: " << hasSynchronousScrollingReasons());

    ScrollingTreeFrameScrollingNode::currentScrollPositionChanged(scrollType, hasSynchronousScrollingReasons() ? ScrollingLayerPositionAction::Set : action);
}

void ScrollingTreeFrameScrollingNodeNicosia::repositionScrollingLayers()
{
    auto* scrollLayer = static_cast<Nicosia::PlatformLayer*>(scrolledContentsLayer());
    ASSERT(scrollLayer);
    auto& compositionLayer = downcast<Nicosia::CompositionLayer>(*scrollLayer);

    auto scrollPosition = currentScrollPosition();

    compositionLayer.accessPending(
        [&scrollPosition](Nicosia::CompositionLayer::LayerState& state)
        {
            state.position = -scrollPosition;
            state.delta.positionChanged = true;
        });
}

void ScrollingTreeFrameScrollingNodeNicosia::repositionRelatedLayers()
{
    auto scrollPosition = currentScrollPosition();
    auto layoutViewport = this->layoutViewport();

    FloatRect visibleContentRect(scrollPosition, scrollableAreaSize());

    auto applyLayerPosition =
        [](auto& layer, auto&& position)
        {
            layer.accessPending(
                [&position](Nicosia::CompositionLayer::LayerState& state)
                {
                    state.position = position;
                    state.delta.positionChanged = true;
                });
        };

    if (m_counterScrollingLayer)
        applyLayerPosition(*m_counterScrollingLayer, layoutViewport.location());

    float topContentInset = this->topContentInset();
    if (m_insetClipLayer && m_rootContentsLayer) {
        m_insetClipLayer->accessPending(
            [&scrollPosition, &topContentInset](Nicosia::CompositionLayer::LayerState& state)
            {
                state.position = { state.position.x(), LocalFrameView::yPositionForInsetClipLayer(scrollPosition, topContentInset) };
                state.delta.positionChanged = true;
            });

        auto rootContentsPosition = LocalFrameView::positionForRootContentLayer(scrollPosition, scrollOrigin(), topContentInset, headerHeight());
        applyLayerPosition(*m_rootContentsLayer, rootContentsPosition);
        if (m_contentShadowLayer)
            applyLayerPosition(*m_contentShadowLayer, rootContentsPosition);
    }

    if (m_headerLayer || m_footerLayer) {
        // Generally the banners should have the same horizontal-position computation as a fixed element. However,
        // the banners are not affected by the frameScaleFactor(), so if there is currently a non-1 frameScaleFactor()
        // then we should recompute layoutViewport.x() for the banner with a scale factor of 1.
        float horizontalScrollOffsetForBanner = layoutViewport.x();
        if (m_headerLayer)
            applyLayerPosition(*m_headerLayer, FloatPoint(horizontalScrollOffsetForBanner, LocalFrameView::yPositionForHeaderLayer(scrollPosition, topContentInset)));
        if (m_footerLayer)
            applyLayerPosition(*m_footerLayer, FloatPoint(horizontalScrollOffsetForBanner, LocalFrameView::yPositionForFooterLayer(scrollPosition, topContentInset, totalContentsSize().height(), footerHeight())));
    }

    delegate().updateVisibleLengths();
}

} // namespace WebCore

#endif // ENABLE(ASYNC_SCROLLING) && USE(NICOSIA)
