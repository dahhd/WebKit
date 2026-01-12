# WebCore 渲染引擎完全指南

> 本文档深入分析 WebKit 的渲染引擎核心 - WebCore，帮助 iOS 开发者理解网页从 HTML 到屏幕像素的完整过程。

## 目录

1. [渲染引擎概述](#1-渲染引擎概述)
2. [DOM 树构建](#2-dom-树构建)
3. [CSSOM 与样式计算](#3-cssom-与样式计算)
4. [布局 (Layout)](#4-布局-layout)
5. [绘制与合成](#5-绘制与合成)
6. [动画系统](#6-动画系统)
7. [与 iOS UIKit 的对比](#7-与-ios-uikit-的对比)

---

## 1. 渲染引擎概述

### 1.1 完整渲染流水线

```
HTML + CSS + JS 输入
        │
        ▼
┌───────────────────────────────────────────────────────┐
│              1. 解析 (Parsing)                         │
│  HTML → DOM 树                                        │
│  CSS → CSSOM                                          │
│  JS → AST → 执行                                      │
└───────────────────────────────────────────────────────┘
        │
        ▼
┌───────────────────────────────────────────────────────┐
│              2. 样式计算 (Style)                       │
│  DOM + CSSOM → 带样式的 DOM                            │
│  计算每个元素的最终样式值                                │
└───────────────────────────────────────────────────────┘
        │
        ▼
┌───────────────────────────────────────────────────────┐
│              3. 布局 (Layout)                          │
│  计算每个元素的位置和大小                                │
│  生成 Layout Tree / Render Tree                       │
└───────────────────────────────────────────────────────┘
        │
        ▼
┌───────────────────────────────────────────────────────┐
│              4. 分层 (Layer)                           │
│  确定哪些内容需要独立图层                                │
│  生成 Layer Tree                                      │
└───────────────────────────────────────────────────────┘
        │
        ▼
┌───────────────────────────────────────────────────────┐
│              5. 绘制 (Paint)                           │
│  生成绘制指令列表                                       │
│  每个图层一个列表                                       │
└───────────────────────────────────────────────────────┘
        │
        ▼
┌───────────────────────────────────────────────────────┐
│              6. 合成 (Composite)                       │
│  GPU 将各图层合成最终画面                               │
│  输出到屏幕                                            │
└───────────────────────────────────────────────────────┘
        │
        ▼
    屏幕像素
```

### 1.2 核心类关系

```
Document                          Page
   │                               │
   │ 拥有                          │ 拥有
   ▼                               ▼
DOM Tree                        LocalFrame
(Node, Element, ...)               │
   │                               │ 拥有
   │ 对应                           ▼
   ▼                           LocalFrameView
Render Tree                        │
(RenderObject, ...)                │ 布局
   │                               ▼
   │ 包含                       RenderView
   ▼                               │
RenderLayer ←──────────────────────┘
   │
   │ 合成
   ▼
GraphicsLayer
   │
   │ GPU
   ▼
屏幕
```

---

## 2. DOM 树构建

### 2.1 HTML 解析器

**路径**: `Source/WebCore/html/parser/`

```cpp
// HTMLDocumentParser 是 HTML 解析的入口
class HTMLDocumentParser {
public:
    void append(const String& html) {
        // 分词
        m_tokenizer.tokenize(html);
        
        // 处理每个 Token
        while (auto token = m_tokenizer.nextToken()) {
            processToken(*token);
        }
    }
    
private:
    void processToken(HTMLToken& token) {
        switch (token.type()) {
        case HTMLToken::StartTag:
            processStartTag(token);
            break;
        case HTMLToken::EndTag:
            processEndTag(token);
            break;
        case HTMLToken::Character:
            processCharacter(token);
            break;
        // ...
        }
    }
    
    void processStartTag(HTMLToken& token) {
        // 创建对应的 Element
        auto element = createElementFor(token);
        
        // 添加到 DOM 树
        m_currentElement->appendChild(element);
        
        // 更新当前元素
        m_currentElement = element.get();
    }
};
```

### 2.2 DOM 树结构

```cpp
// 解析 HTML:
// <div id="container">
//   <p>Hello</p>
//   <span>World</span>
// </div>

// 生成的 DOM 树:
Document
└── <html>
    └── <body>
        └── <div id="container">
            ├── <p>
            │   └── Text: "Hello"
            └── <span>
                └── Text: "World"
```

### 2.3 DOM 节点创建

**路径**: `Source/WebCore/dom/Document.cpp`

```cpp
Ref<Element> Document::createElement(const AtomString& tagName)
{
    // 根据标签名创建对应的 Element 子类
    if (tagName == "div"_s)
        return HTMLDivElement::create(*this);
    if (tagName == "p"_s)
        return HTMLParagraphElement::create(*this);
    if (tagName == "span"_s)
        return HTMLSpanElement::create(*this);
    // ... 其他标签
    
    // 未知标签创建通用 HTMLElement
    return HTMLUnknownElement::create(tagName, *this);
}
```

---

## 3. CSSOM 与样式计算

### 3.1 CSS 解析

**路径**: `Source/WebCore/css/parser/`

```cpp
// CSS 源码解析成 StyleSheet 对象
Ref<StyleSheet> CSSParser::parseStyleSheet(const String& css)
{
    auto sheet = StyleSheetContents::create();
    
    CSSTokenizer tokenizer(css);
    CSSParserTokenStream stream(tokenizer);
    
    while (!stream.atEnd()) {
        // 解析规则
        if (auto rule = parseRule(stream))
            sheet->parserAppendRule(*rule);
    }
    
    return sheet;
}

// 解析单条规则
Ref<StyleRule> parseRule(CSSParserTokenStream& stream)
{
    // 选择器
    auto selectors = parseSelectors(stream);
    
    // 声明块 { ... }
    auto properties = parseDeclarationBlock(stream);
    
    return StyleRule::create(WTFMove(selectors), WTFMove(properties));
}
```

### 3.2 选择器匹配

**路径**: `Source/WebCore/css/SelectorChecker.cpp`

```cpp
bool SelectorChecker::match(const CSSSelector& selector, Element& element)
{
    // 从右到左匹配选择器
    // 例如: div .class > span
    // 先检查当前元素是否是 span
    // 再检查父元素是否有 .class
    // 再检查祖先是否有 div
    
    const CSSSelector* current = &selector;
    Element* currentElement = &element;
    
    while (current) {
        // 检查当前部分
        if (!checkOne(*current, *currentElement))
            return false;
        
        // 移动到上一部分
        switch (current->relation()) {
        case CSSSelector::Descendant:
            // 后代选择器：查找任意祖先
            currentElement = findMatchingAncestor(*current, *currentElement);
            break;
        case CSSSelector::Child:
            // 子选择器：只检查父元素
            currentElement = currentElement->parentElement();
            break;
        case CSSSelector::DirectAdjacent:
            // 相邻兄弟选择器
            currentElement = currentElement->previousElementSibling();
            break;
        }
        
        current = current->tagHistory();
    }
    
    return true;
}
```

### 3.3 样式级联

**路径**: `Source/WebCore/style/StyleResolver.cpp`

```cpp
// 样式优先级（从低到高）
enum CascadeLevel {
    UserAgentLevel,      // 浏览器默认样式
    UserLevel,           // 用户样式
    AuthorLevel,         // 页面作者样式
    AuthorImportantLevel,// !important
    UserImportantLevel,
    UserAgentImportantLevel
};

void StyleResolver::matchAllRules(Element& element, ElementRuleCollector& collector)
{
    // 1. 用户代理样式（浏览器默认）
    matchUARules(element, collector);
    
    // 2. 用户样式
    matchUserRules(element, collector);
    
    // 3. 作者样式
    matchAuthorRules(element, collector);
    
    // 4. 行内样式
    if (element.hasAttributeWithoutSynchronization(styleAttr))
        collector.addInlineStyle(element.inlineStyle());
    
    // 5. 按优先级排序
    collector.sortMatchedRules();
}
```

### 3.4 计算样式

**路径**: `Source/WebCore/style/StyleBuilderState.cpp`

```cpp
// 将匹配的 CSS 规则应用到元素，生成最终样式
void StyleBuilder::applyProperty(CSSPropertyID property, const CSSValue& value)
{
    switch (property) {
    case CSSPropertyColor:
        applyColor(value);
        break;
    case CSSPropertyWidth:
        applyWidth(value);
        break;
    case CSSPropertyDisplay:
        applyDisplay(value);
        break;
    // ... 数百个属性
    }
}

void StyleBuilder::applyWidth(const CSSValue& value)
{
    if (auto* primitiveValue = dynamicDowncast<CSSPrimitiveValue>(value)) {
        if (primitiveValue->isLength()) {
            // 转换单位（px, em, rem, % 等）到像素
            auto length = primitiveValue->convertToLength<Length>();
            m_style.setWidth(length);
        } else if (primitiveValue->isPercentage()) {
            m_style.setWidth(Length(primitiveValue->doubleValue(), Percent));
        }
    }
}
```

---

## 4. 布局 (Layout)

### 4.1 Render Tree 构建

**路径**: `Source/WebCore/rendering/RenderTreeUpdater.cpp`

```cpp
void RenderTreeUpdater::createRendererFor(Element& element)
{
    // 某些元素不创建渲染对象
    if (element.rendererIsNeeded()) {
        const RenderStyle& style = element.computedStyle();
        
        // 根据 display 属性创建不同类型的渲染对象
        RenderObject* renderer = nullptr;
        
        switch (style.display()) {
        case DisplayType::Block:
        case DisplayType::FlowRoot:
            renderer = new RenderBlockFlow(element, RenderStyle::clone(style));
            break;
        case DisplayType::Inline:
            renderer = new RenderInline(element, RenderStyle::clone(style));
            break;
        case DisplayType::Flex:
            renderer = new RenderFlexibleBox(element, RenderStyle::clone(style));
            break;
        case DisplayType::Grid:
            renderer = new RenderGrid(element, RenderStyle::clone(style));
            break;
        case DisplayType::None:
            return; // 不创建渲染对象
        }
        
        // 添加到 Render Tree
        insertRenderer(element, *renderer);
    }
}
```

### 4.2 盒模型

```
┌───────────────────────────────────────────────────────┐
│                      Margin                           │
│  ┌─────────────────────────────────────────────────┐  │
│  │                    Border                        │  │
│  │  ┌───────────────────────────────────────────┐  │  │
│  │  │                 Padding                    │  │  │
│  │  │  ┌─────────────────────────────────────┐  │  │  │
│  │  │  │                                      │  │  │  │
│  │  │  │            Content Box               │  │  │  │
│  │  │  │         (width × height)             │  │  │  │
│  │  │  │                                      │  │  │  │
│  │  │  └─────────────────────────────────────┘  │  │  │
│  │  │                                           │  │  │
│  │  └───────────────────────────────────────────┘  │  │
│  │                                                 │  │
│  └─────────────────────────────────────────────────┘  │
│                                                       │
└───────────────────────────────────────────────────────┘
```

**路径**: `Source/WebCore/rendering/RenderBox.h`

```cpp
class RenderBox : public RenderBoxModelObject {
public:
    // 内容区域
    LayoutUnit contentWidth() const;
    LayoutUnit contentHeight() const;
    
    // 加上 padding
    LayoutUnit paddingBoxWidth() const { return contentWidth() + paddingLeft() + paddingRight(); }
    LayoutUnit paddingBoxHeight() const { return contentHeight() + paddingTop() + paddingBottom(); }
    
    // 加上 border
    LayoutUnit borderBoxWidth() const { return paddingBoxWidth() + borderLeft() + borderRight(); }
    LayoutUnit borderBoxHeight() const { return paddingBoxHeight() + borderTop() + borderBottom(); }
    
    // 加上 margin
    LayoutUnit marginBoxWidth() const { return borderBoxWidth() + marginLeft() + marginRight(); }
    LayoutUnit marginBoxHeight() const { return borderBoxHeight() + marginTop() + marginBottom(); }
    
    // 位置
    LayoutPoint location() const { return m_frameRect.location(); }
    LayoutSize size() const { return m_frameRect.size(); }
    
private:
    LayoutRect m_frameRect; // 边框盒的位置和大小
};
```

### 4.3 块级布局

**路径**: `Source/WebCore/rendering/RenderBlockFlow.cpp`

```cpp
void RenderBlockFlow::layoutBlock(bool relayoutChildren)
{
    // 1. 计算宽度
    computeLogicalWidth();
    
    // 2. 设置起始位置
    LayoutUnit logicalTop = borderAndPaddingBefore();
    
    // 3. 布局子元素
    for (auto* child = firstChildBox(); child; child = child->nextSiblingBox()) {
        if (child->isOutOfFlowPositioned())
            continue; // 绝对定位的元素另外处理
        
        // 处理 margin collapse
        LayoutUnit childMarginBefore = child->marginBefore();
        logicalTop += std::max(LayoutUnit(0), childMarginBefore);
        
        // 设置子元素位置
        child->setLogicalTop(logicalTop);
        
        // 递归布局子元素
        child->layoutIfNeeded();
        
        // 更新位置
        logicalTop += child->logicalHeight() + child->marginAfter();
    }
    
    // 4. 计算高度
    computeLogicalHeight();
}
```

### 4.4 Flex 布局

**路径**: `Source/WebCore/rendering/RenderFlexibleBox.cpp`

```cpp
void RenderFlexibleBox::layoutFlexItems()
{
    Vector<FlexItem> items;
    
    // 1. 收集 flex items
    for (auto* child = firstChildBox(); child; child = child->nextSiblingBox()) {
        items.append(FlexItem(child));
    }
    
    // 2. 计算主轴大小
    LayoutUnit mainAxisExtent = computeMainAxisExtent();
    
    // 3. 根据 flex-grow/shrink 分配空间
    LayoutUnit availableSpace = mainAxisExtent - usedMainAxisSpace(items);
    
    if (availableSpace > 0) {
        // 有剩余空间，按 flex-grow 分配
        distributePositiveSpace(items, availableSpace);
    } else if (availableSpace < 0) {
        // 空间不足，按 flex-shrink 收缩
        distributeNegativeSpace(items, availableSpace);
    }
    
    // 4. 交叉轴对齐
    alignItems(items);
    
    // 5. 布局各项
    for (auto& item : items) {
        layoutFlexItem(item);
    }
}
```

### 4.5 Grid 布局

**路径**: `Source/WebCore/rendering/RenderGrid.cpp`

```cpp
void RenderGrid::layoutGrid()
{
    // 1. 确定网格结构
    placeItemsOnGrid();
    
    // 2. 计算轨道大小
    computeTrackSizes();
    
    // 3. 布局网格项
    for (auto* item : gridItems()) {
        // 获取项目在网格中的位置
        GridArea area = m_grid.gridItemArea(*item);
        
        // 计算位置和大小
        LayoutUnit x = columnPositions[area.columns.startLine()];
        LayoutUnit y = rowPositions[area.rows.startLine()];
        LayoutUnit width = columnPositions[area.columns.endLine()] - x;
        LayoutUnit height = rowPositions[area.rows.endLine()] - y;
        
        // 设置位置
        item->setLocation(LayoutPoint(x, y));
        item->setLogicalWidth(width);
        
        // 递归布局
        item->layoutIfNeeded();
    }
}
```

---

## 5. 绘制与合成

### 5.1 RenderLayer 图层系统

**路径**: `Source/WebCore/rendering/RenderLayer.h`

```cpp
class RenderLayer {
public:
    // 图层属性
    bool isComposited() const;           // 是否使用 GPU 合成
    bool hasTransform() const;           // 是否有 transform
    bool hasFilter() const;              // 是否有滤镜
    bool hasBackdropFilter() const;      // 是否有背景滤镜
    
    // 绘制
    void paint(GraphicsContext&, const LayoutRect& damageRect, PaintBehavior);
    
    // 滚动
    void scrollTo(const ScrollPosition&);
    
    // 裁剪
    void updateClipRects();
    
private:
    // 关联的渲染对象
    RenderLayerModelObject& m_renderer;
    
    // 子图层
    Vector<RenderLayer*> m_children;
    
    // 变换矩阵
    std::unique_ptr<TransformationMatrix> m_transform;
    
    // 合成层
    std::unique_ptr<RenderLayerBacking> m_backing;
};
```

### 5.2 创建合成层的条件

**路径**: `Source/WebCore/rendering/RenderLayerCompositor.cpp`

```cpp
CompositingReasons RenderLayerCompositor::reasonsForCompositing(const RenderLayer& layer)
{
    CompositingReasons reasons = CompositingReason::None;
    
    const RenderStyle& style = layer.renderer().style();
    
    // 1. 3D 变换
    if (style.transform().has3DOperation())
        reasons |= CompositingReason::Transform3D;
    
    // 2. will-change
    if (style.willChange().contains(CSSPropertyTransform))
        reasons |= CompositingReason::WillChangeTransform;
    if (style.willChange().contains(CSSPropertyOpacity))
        reasons |= CompositingReason::WillChangeOpacity;
    
    // 3. 透明度动画
    if (style.hasOpacity() && style.hasAnimations())
        reasons |= CompositingReason::OpacityAnimation;
    
    // 4. 固定定位
    if (style.position() == PositionType::Fixed)
        reasons |= CompositingReason::FixedPosition;
    
    // 5. 滚动溢出
    if (layer.hasOverflowScrolling())
        reasons |= CompositingReason::OverflowScrolling;
    
    // 6. 视频/Canvas
    if (layer.renderer().isVideo() || layer.renderer().isCanvas())
        reasons |= CompositingReason::Media;
    
    return reasons;
}
```

### 5.3 绘制流程

**路径**: `Source/WebCore/rendering/RenderLayer.cpp`

```cpp
void RenderLayer::paint(GraphicsContext& context, const LayoutRect& damageRect, PaintBehavior behavior)
{
    // 绘制顺序很重要：
    
    // 1. 绘制背景
    paintBackgroundForFragments(context, damageRect);
    
    // 2. 绘制负 z-index 的子元素
    for (auto* child : m_negativeZOrderList)
        child->paint(context, damageRect, behavior);
    
    // 3. 绘制自身的前景
    paintForegroundForFragments(context, damageRect);
    
    // 4. 绘制正常流中的子元素
    for (auto* child : m_normalFlowList)
        child->paint(context, damageRect, behavior);
    
    // 5. 绘制正 z-index 的子元素
    for (auto* child : m_positiveZOrderList)
        child->paint(context, damageRect, behavior);
    
    // 6. 绘制轮廓
    paintOutlineForFragments(context, damageRect);
}
```

### 5.4 绘制阶段详解

**路径**: `Source/WebCore/rendering/PaintPhase.h`

```cpp
enum class PaintPhase {
    // 1. 块级元素背景
    BlockBackground,
    
    // 2. 子块级元素背景
    ChildBlockBackgrounds,
    
    // 3. 浮动元素
    Float,
    
    // 4. 前景（文本、边框等）
    Foreground,
    
    // 5. 轮廓
    Outline,
    
    // 6. 子元素轮廓
    ChildOutlines,
    
    // 7. 选中状态
    Selection,
    
    // 8. 遮罩
    Mask
};
```

### 5.5 GPU 合成

**路径**: `Source/WebCore/rendering/RenderLayerCompositor.cpp`

```cpp
void RenderLayerCompositor::updateBacking(RenderLayer& layer)
{
    if (layer.needsCompositedScrolling() || 
        requiresCompositing(layer)) {
        
        // 创建或更新 GraphicsLayer
        if (!layer.backing()) {
            layer.ensureBacking();
        }
        
        // 配置图层
        auto& backing = *layer.backing();
        
        // 设置内容
        backing.updateContents();
        
        // 设置变换
        backing.updateTransform();
        
        // 设置滤镜
        backing.updateFilters();
        
        // 连接到父图层
        parentLayer->backing()->graphicsLayer()->addChild(
            backing.graphicsLayer());
    }
}
```

---

## 6. 动画系统

### 6.1 Web Animations API

**路径**: `Source/WebCore/animation/WebAnimation.h`

```cpp
class WebAnimation : public RefCounted<WebAnimation>, public EventTarget {
public:
    // 播放控制
    void play();
    void pause();
    void cancel();
    void finish();
    void reverse();
    
    // 时间控制
    double currentTime() const;
    void setCurrentTime(double);
    
    // 速率
    double playbackRate() const;
    void setPlaybackRate(double);
    
    // 状态
    AnimationPlayState playState() const;
    bool pending() const;
    
private:
    Ref<AnimationEffect> m_effect;
    RefPtr<AnimationTimeline> m_timeline;
    double m_playbackRate { 1.0 };
    Seconds m_currentTime;
};
```

### 6.2 关键帧动画

**路径**: `Source/WebCore/animation/KeyframeEffect.cpp`

```cpp
class KeyframeEffect : public AnimationEffect {
public:
    // 关键帧
    void setKeyframes(const Vector<Keyframe>& keyframes) {
        m_keyframes = keyframes;
        updateInterpolations();
    }
    
    // 更新动画
    void apply() override {
        double progress = iterationProgress();
        
        // 找到当前进度所在的关键帧区间
        size_t startIndex = 0;
        for (size_t i = 0; i < m_keyframes.size() - 1; i++) {
            if (progress >= m_keyframes[i].offset && 
                progress <= m_keyframes[i + 1].offset) {
                startIndex = i;
                break;
            }
        }
        
        const Keyframe& start = m_keyframes[startIndex];
        const Keyframe& end = m_keyframes[startIndex + 1];
        
        // 计算区间内的进度
        double intervalProgress = (progress - start.offset) / 
                                  (end.offset - start.offset);
        
        // 应用缓动函数
        intervalProgress = start.easing.transform(intervalProgress);
        
        // 插值每个属性
        for (auto& property : m_animatedProperties) {
            auto startValue = start.properties.get(property);
            auto endValue = end.properties.get(property);
            auto interpolated = interpolate(startValue, endValue, intervalProgress);
            
            m_target->setAnimatedStyle(property, interpolated);
        }
    }
    
private:
    Vector<Keyframe> m_keyframes;
    Element* m_target;
};
```

### 6.3 CSS 过渡

**路径**: `Source/WebCore/animation/CSSTransition.cpp`

```cpp
// 当样式改变时检查是否需要创建过渡
void checkForTransitions(Element& element, 
                         const RenderStyle& oldStyle, 
                         const RenderStyle& newStyle)
{
    for (auto property : animatableProperties()) {
        // 检查属性值是否改变
        if (oldStyle.propertyValue(property) == newStyle.propertyValue(property))
            continue;
        
        // 检查是否有 transition 配置
        auto transitionDuration = newStyle.transitionDuration(property);
        if (transitionDuration <= 0)
            continue;
        
        // 创建过渡动画
        auto transition = CSSTransition::create(element, property);
        transition->setStartValue(oldStyle.propertyValue(property));
        transition->setEndValue(newStyle.propertyValue(property));
        transition->setDuration(transitionDuration);
        transition->setTimingFunction(newStyle.transitionTimingFunction(property));
        
        element.addAnimation(transition);
    }
}
```

### 6.4 合成动画

**路径**: `Source/WebCore/animation/AcceleratedEffectStackUpdater.cpp`

```cpp
// 某些动画可以在 GPU 上执行，不影响主线程
bool canAccelerateAnimation(const Animation& animation)
{
    for (auto property : animation.animatedProperties()) {
        switch (property) {
        case CSSPropertyTransform:
        case CSSPropertyOpacity:
        case CSSPropertyFilter:
        case CSSPropertyBackdropFilter:
            continue; // 可以加速
        default:
            return false; // 需要主线程
        }
    }
    return true;
}

void pushAcceleratedAnimation(RenderLayer& layer, const Animation& animation)
{
    // 将动画信息传递给 GPU 进程
    auto& backing = layer.backing();
    backing->addAnimation(animation);
    
    // GPU 进程会独立驱动这个动画
    // 主线程只在动画结束时收到通知
}
```

---

## 7. 与 iOS UIKit 的对比

### 7.1 视图系统对比

| WebKit 概念 | UIKit 对应 | 说明 |
|------------|-----------|------|
| `Node` | `UIView` | 视图层次中的节点 |
| `Document` | `UIWindow` | 根容器 |
| `Element` | `UIView` 子类 | 具体的视图类型 |
| `RenderObject` | `CALayer` | 负责渲染 |
| `RenderLayer` | `CALayer` 层级 | 独立的渲染层 |
| `GraphicsLayer` | `CALayer` (GPU) | GPU 合成层 |

### 7.2 布局系统对比

```swift
// WebKit CSS 布局
// .container {
//     display: flex;
//     justify-content: center;
//     align-items: center;
// }

// UIKit Auto Layout 等效
containerView.addSubview(childView)
NSLayoutConstraint.activate([
    childView.centerXAnchor.constraint(equalTo: containerView.centerXAnchor),
    childView.centerYAnchor.constraint(equalTo: containerView.centerYAnchor)
])

// 或使用 UIStackView
let stackView = UIStackView(arrangedSubviews: [childView])
stackView.axis = .horizontal
stackView.alignment = .center
stackView.distribution = .equalCentering
```

### 7.3 动画系统对比

```swift
// WebKit CSS 动画
// @keyframes fadeIn {
//     from { opacity: 0; }
//     to { opacity: 1; }
// }
// .element { animation: fadeIn 0.3s ease-in-out; }

// UIKit Core Animation 等效
let animation = CABasicAnimation(keyPath: "opacity")
animation.fromValue = 0
animation.toValue = 1
animation.duration = 0.3
animation.timingFunction = CAMediaTimingFunction(name: .easeInEaseOut)
view.layer.add(animation, forKey: "fadeIn")

// 或使用 UIView animate
UIView.animate(withDuration: 0.3, 
               delay: 0,
               options: .curveEaseInOut) {
    view.alpha = 1
}
```

### 7.4 事件处理对比

```swift
// WebKit DOM 事件（捕获 → 目标 → 冒泡）
// element.addEventListener('click', handler, { capture: true })

// UIKit Responder Chain（反向：从目标向上）
class CustomView: UIView {
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        // 处理或传递给 next responder
        next?.touchesBegan(touches, with: event)
    }
}

// 手势识别器
let tap = UITapGestureRecognizer(target: self, action: #selector(handleTap))
view.addGestureRecognizer(tap)
```

### 7.5 性能优化对比

| 优化策略 | WebKit | UIKit |
|---------|--------|-------|
| 图层提升 | `will-change: transform` | `shouldRasterize = true` |
| 离屏渲染 | `transform: translateZ(0)` | `CALayer.masksToBounds` |
| 异步绘制 | 合成层 | `drawsAsynchronously = true` |
| 内容缓存 | 绘制指令缓存 | `contents` 缓存 |

---

## 总结

通过这份 WebCore 渲染引擎指南，你应该理解了：

1. **完整的渲染流水线** - 从 HTML 到像素的每一步
2. **DOM 树是如何构建的** - HTML 解析和节点创建
3. **样式是如何计算的** - CSS 选择器匹配和级联
4. **布局是如何工作的** - 盒模型、块级布局、Flex、Grid
5. **绘制和合成机制** - 图层、绘制指令、GPU 合成
6. **动画系统原理** - Web Animations、CSS 过渡、合成动画

这些知识将帮助你：
- 理解网页渲染的性能特点
- 优化 WKWebView 中网页的性能
- 在 iOS 开发中借鉴 WebKit 的设计理念
- 调试复杂的渲染问题

WebCore 是一个极其复杂和精密的系统，希望这份文档能帮助你更好地理解它的工作原理！
