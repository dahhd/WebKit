# WebKit 内核架构深度分析

> 本文档基于 WebKit 源码仓库进行深度分析，专为 iOS 开发工程师编写，帮助你深入理解 WebKit 内核的底层原理。

## 目录

1. [概述：什么是 WebKit](#1-概述什么是-webkit)
2. [核心模块架构](#2-核心模块架构)
3. [DOM 树与节点系统](#3-dom-树与节点系统)
4. [渲染流水线](#4-渲染流水线)
5. [JavaScript 引擎 (JavaScriptCore)](#5-javascript-引擎-javascriptcore)
6. [多进程架构](#6-多进程架构)
7. [WKWebView 与 iOS 开发](#7-wkwebview-与-ios-开发)
8. [网络与资源加载](#8-网络与资源加载)
9. [事件处理系统](#9-事件处理系统)
10. [CSS 样式系统](#10-css-样式系统)

---

## 1. 概述：什么是 WebKit

### 1.1 WebKit 的身份

WebKit 是一个**开源的浏览器引擎**，由苹果公司主导开发和维护。在 iOS 设备上，**所有浏览器都必须使用 WebKit** —— 包括 Safari、Chrome、Firefox 的 iOS 版本。这意味着作为 iOS 开发者，理解 WebKit 对于处理任何 Web 内容至关重要。

### 1.2 源码目录结构总览

```
WebKit/
├── Source/                    # 核心源代码目录
│   ├── WebCore/              # 渲染引擎核心（最重要）
│   ├── JavaScriptCore/       # JavaScript 引擎
│   ├── WebKit/               # 跨进程通信和 API 层
│   ├── WTF/                  # Web Template Framework - 基础工具库
│   ├── bmalloc/              # 内存分配器
│   └── WebKitLegacy/         # 旧版 API (UIWebView)
├── Configurations/           # 构建配置
└── WebKitLibraries/          # 第三方库
```

### 1.3 iOS 开发视角

从 iOS 开发的角度，你可以这样理解：

| WebKit 组件 | iOS 开发类比 |
|------------|-------------|
| `WKWebView` | 类似于 `UIImageView`，是用户界面层 |
| `WebCore` | 类似于 `UIKit` 框架，负责渲染和布局 |
| `JavaScriptCore` | 类似于 Swift Runtime，负责执行代码 |
| `WTF` | 类似于 Foundation 框架，提供基础工具 |

---

## 2. 核心模块架构

### 2.1 WTF (Web Template Framework)

**路径**: `Source/WTF/wtf/`

WTF 是 WebKit 的"基础设施层"，提供了类似于 C++ 标准库但更高效的实现。

#### 核心组件

```
wtf/
├── Vector.h           # 类似 std::vector，但性能更好
├── HashMap.h          # 哈希表实现
├── RefPtr.h           # 智能指针，类似 Swift 的 ARC
├── RefCounted.h       # 引用计数基类
├── String.h           # 字符串类
├── URL.h              # URL 解析和处理
├── Threading.h        # 线程管理
├── RunLoop.h          # 运行循环，类似 CFRunLoop
├── MainThread.h       # 主线程工具
└── WorkQueue.h        # 工作队列，类似 DispatchQueue
```

#### iOS 开发类比

```cpp
// WTF::RefPtr 类似 Swift 的强引用
// Source/WTF/wtf/RefPtr.h
template<typename T> class RefPtr {
    T* m_ptr;
    // 自动管理引用计数，类似 Swift ARC
};

// iOS 中的类比：
// let view: UIView = UIView()  // Swift ARC 自动管理
```

```cpp
// WTF::Vector 类似 Swift Array
// Source/WTF/wtf/Vector.h
template<typename T> class Vector {
    // 动态数组，自动扩容
};

// iOS 类比：
// var array: [Int] = []
```

### 2.2 WebCore - 渲染引擎核心

**路径**: `Source/WebCore/`

WebCore 是 WebKit 的"心脏"，负责：
- 解析 HTML/CSS
- 构建 DOM 树
- 计算样式和布局
- 执行渲染

#### 核心子目录

```
WebCore/
├── dom/              # DOM 节点实现
├── html/             # HTML 元素实现
├── css/              # CSS 解析和样式
├── rendering/        # 渲染树和绘制
├── layout/           # 布局计算
├── page/             # Page、Frame、Window 等概念
├── loader/           # 资源加载
├── platform/         # 平台抽象层
├── bindings/         # JS 绑定
├── animation/        # 动画系统
└── style/            # 样式计算
```

### 2.3 JavaScriptCore - JS 引擎

**路径**: `Source/JavaScriptCore/`

JavaScriptCore 是苹果的 JavaScript 引擎，在 iOS 上可以直接使用（通过 JavaScriptCore.framework）。

```
JavaScriptCore/
├── API/              # 公开 API（iOS 可用）
├── runtime/          # JS 运行时（对象、函数等）
├── parser/           # 代码解析
├── bytecode/         # 字节码
├── interpreter/      # 解释器
├── jit/              # JIT 编译器
├── dfg/              # DFG 优化编译器
├── ftl/              # FTL 高级优化编译器
└── heap/             # 垃圾回收
```

### 2.4 WebKit (WebKit2)

**路径**: `Source/WebKit/`

这是现代 WebKit 架构，实现了多进程模型：

```
WebKit/
├── UIProcess/        # UI 进程（你的 App 进程）
├── WebProcess/       # Web 内容进程（渲染进程）
├── NetworkProcess/   # 网络进程
├── GPUProcess/       # GPU 进程
├── Shared/           # 跨进程共享代码
└── Platform/         # 平台相关实现
```

---

## 3. DOM 树与节点系统

### 3.1 Node - 所有节点的基类

**路径**: `Source/WebCore/dom/Node.h`

```cpp
// Node 是 DOM 树中所有节点的基类
class Node : public EventTarget {
public:
    enum NodeType {
        ELEMENT_NODE = 1,           // 元素节点，如 <div>
        ATTRIBUTE_NODE = 2,         // 属性节点（已废弃）
        TEXT_NODE = 3,              // 文本节点
        COMMENT_NODE = 8,           // 注释节点
        DOCUMENT_NODE = 9,          // Document 节点
        DOCUMENT_FRAGMENT_NODE = 11 // DocumentFragment
    };
    
    // 树结构导航
    ContainerNode* parentNode() const;
    Node* firstChild() const;
    Node* lastChild() const;
    Node* nextSibling() const;
    Node* previousSibling() const;
    
    // 所属 Document
    Document& document() const;
    
    // 渲染对象
    RenderObject* renderer() const;
};
```

#### iOS 类比

```swift
// Node 类似于 UIView 在视图层次中的角色
class UIView {
    var superview: UIView?        // 类似 parentNode()
    var subviews: [UIView]        // 类似 childNodes
    // UIView 也是树形结构
}
```

### 3.2 继承层次

```
EventTarget           # 事件目标基类
    └── Node          # 所有 DOM 节点的基类
        ├── Document      # 整个文档
        ├── Element       # 元素节点
        │   ├── HTMLElement   # HTML 元素
        │   │   ├── HTMLDivElement
        │   │   ├── HTMLInputElement
        │   │   └── ...
        │   └── SVGElement    # SVG 元素
        ├── CharacterData     # 字符数据
        │   ├── Text          # 文本节点
        │   └── Comment       # 注释节点
        └── DocumentFragment  # 文档片段
```

### 3.3 Document - 文档对象

**路径**: `Source/WebCore/dom/Document.h`

Document 是整个网页的根节点，管理所有的 DOM 节点。

```cpp
class Document : public ContainerNode, public TreeScope {
public:
    // 创建元素
    Ref<Element> createElement(const AtomString& tagName);
    
    // 查找元素
    Element* getElementById(const AtomString& id);
    
    // 访问特殊元素
    HTMLBodyElement* body() const;
    HTMLHeadElement* head() const;
    
    // 样式表管理
    StyleResolver& styleResolver();
    
    // 页面生命周期
    void prepareForDestruction();
};
```

#### iOS 类比

```swift
// Document 类似于 UIWindow 或整个视图控制器
class UIViewController {
    var view: UIView!  // 类似 document.body
    // 管理整个视图层次
}
```

### 3.4 Element - 元素节点

**路径**: `Source/WebCore/dom/Element.h`

```cpp
class Element : public ContainerNode {
public:
    // 属性操作
    const AtomString& getAttribute(const QualifiedName&) const;
    void setAttribute(const QualifiedName&, const AtomString& value);
    
    // 类名操作
    DOMTokenList& classList();
    
    // 样式
    CSSStyleDeclaration& style();
    
    // 布局信息
    IntRect boundingClientRect();
    
    // Shadow DOM
    ShadowRoot* shadowRoot() const;
    ShadowRoot& attachShadow(ShadowRootMode);
};
```

### 3.5 实际代码示例：节点创建流程

当浏览器解析 HTML 时：

```html
<div id="container" class="box">Hello</div>
```

WebKit 内部的处理流程（简化版）：

```cpp
// 1. 创建 HTMLDivElement
// Source/WebCore/html/HTMLDivElement.cpp
Ref<HTMLDivElement> HTMLDivElement::create(Document& document)
{
    return adoptRef(*new HTMLDivElement(HTMLNames::divTag, document));
}

// 2. 设置属性
element->setAttribute(HTMLNames::idAttr, "container"_s);
element->setAttribute(HTMLNames::classAttr, "box"_s);

// 3. 添加文本子节点
auto textNode = document.createTextNode("Hello"_s);
element->appendChild(textNode);
```

---

## 4. 渲染流水线

### 4.1 渲染流水线概述

WebKit 的渲染流程（Rendering Pipeline）可以分为以下阶段：

```
HTML/CSS/JS 输入
      ↓
┌─────────────────┐
│   DOM 树构建     │  ← 解析 HTML，创建 Node 对象
└─────────────────┘
      ↓
┌─────────────────┐
│  样式计算 Style  │  ← 计算每个元素的最终样式
└─────────────────┘
      ↓
┌─────────────────┐
│   布局 Layout    │  ← 计算每个元素的位置和大小
└─────────────────┘
      ↓
┌─────────────────┐
│  分层 Layering   │  ← 将内容分到不同的图层
└─────────────────┘
      ↓
┌─────────────────┐
│   绘制 Paint     │  ← 生成绘制指令
└─────────────────┘
      ↓
┌─────────────────┐
│  合成 Composite  │  ← GPU 合成最终画面
└─────────────────┘
      ↓
    屏幕显示
```

### 4.2 RenderObject - 渲染对象基类

**路径**: `Source/WebCore/rendering/RenderObject.h`

每个需要显示的 DOM 节点都会创建对应的 RenderObject。

```cpp
class RenderObject : public CachedImageClient {
public:
    // 渲染对象类型
    enum class Type : uint8_t {
        BlockFlow,    // 块级流式布局
        Inline,       // 行内元素
        Image,        // 图片
        Text,         // 文本
        Table,        // 表格
        // ...
    };
    
    // 关联的 DOM 节点
    Node* node() const;
    
    // 关联的样式
    const RenderStyle& style() const;
    
    // 绘制
    virtual void paint(PaintInfo&, const LayoutPoint&);
    
    // 布局
    virtual void layout();
};
```

#### iOS 类比

```swift
// RenderObject 类似于 CALayer
// DOM Node : RenderObject ≈ UIView : CALayer
class CALayer {
    func draw(in ctx: CGContext)  // 类似 paint()
    func layoutSublayers()        // 类似 layout()
}
```

### 4.3 RenderTree vs DOM Tree

```
DOM Tree (逻辑结构)              Render Tree (渲染结构)
═══════════════════              ═══════════════════════

Document                         RenderView
    │                                │
    └── <html>                       └── RenderBlock (html)
        │                                    │
        ├── <head>                           │ (head 不渲染)
        │   └── <style>                      │
        │                                    │
        └── <body>                           └── RenderBody
            │                                        │
            ├── <div>                                ├── RenderBlock
            │   └── "Hello"                          │   └── RenderText
            │                                        │
            └── <span style="display:none">          │ (display:none 不创建渲染对象)
                └── "Hidden"                         │
```

**重要区别：**
- 不是所有 DOM 节点都有 RenderObject（如 `display: none`）
- 一个 DOM 节点可能生成多个渲染对象（如列表项的标记）

### 4.4 布局计算 (Layout)

**路径**: `Source/WebCore/rendering/RenderBlockFlow.cpp`

```cpp
void RenderBlockFlow::layoutBlock(bool relayoutChildren)
{
    // 1. 清除之前的布局数据
    clearFloats();
    
    // 2. 计算自身宽度
    computeLogicalWidth();
    
    // 3. 递归布局子元素
    layoutBlockChildren(relayoutChildren);
    
    // 4. 计算自身高度
    computeLogicalHeight();
    
    // 5. 处理浮动和溢出
    handleAfterFloats();
    computeOverflow();
}
```

#### iOS 类比

```swift
// 布局过程类似 UIView 的 layoutSubviews
class UIView {
    override func layoutSubviews() {
        super.layoutSubviews()
        // 计算并设置子视图的 frame
        for subview in subviews {
            subview.frame = calculateFrame(for: subview)
        }
    }
}

// 或者 Auto Layout 的约束求解过程
```

### 4.5 RenderLayer - 图层系统

**路径**: `Source/WebCore/rendering/RenderLayer.h`

RenderLayer 负责分层渲染，这是实现 CSS 3D 变换、透明度等效果的基础。

```cpp
class RenderLayer {
public:
    // 是否创建合成层
    bool isComposited() const;
    
    // 变换
    TransformationMatrix* transform() const;
    
    // 滚动
    void scrollTo(const ScrollPosition&);
    
    // 裁剪
    void updateClipRects();
    
    // 绘制
    void paintLayer(PaintInfo&);
};
```

#### 什么情况下会创建独立图层？

```css
/* 这些 CSS 会导致创建新的 RenderLayer */
.layer {
    transform: translate3d(0, 0, 0);  /* 3D 变换 */
    opacity: 0.9;                      /* 不完全透明 */
    position: fixed;                   /* 固定定位 */
    overflow: scroll;                  /* 滚动 */
    will-change: transform;            /* 预告变化 */
}
```

#### iOS 类比

```swift
// RenderLayer 非常类似 CALayer
// 特别是涉及到合成和硬件加速时

// iOS 中强制创建独立图层：
view.layer.shouldRasterize = true
// 或使用 3D 变换：
view.layer.transform = CATransform3DMakeTranslation(0, 0, 0)
```

### 4.6 绘制 (Paint)

**路径**: `Source/WebCore/rendering/PaintInfo.h`

```cpp
struct PaintInfo {
    GraphicsContext& context;      // 图形上下文
    const LayoutRect& rect;        // 绘制区域
    PaintPhase phase;              // 绘制阶段
};

// 绘制阶段
enum class PaintPhase {
    BlockBackground,      // 背景
    ChildBlockBackgrounds,
    Float,               // 浮动元素
    Foreground,          // 前景（文本等）
    Outline,             // 轮廓
    ChildOutlines,
    Selection,           // 选中状态
};
```

#### iOS 类比

```swift
// 类似于 UIView 的 draw(_:) 或 CALayer 的 draw(in:)
class UIView {
    override func draw(_ rect: CGRect) {
        let context = UIGraphicsGetCurrentContext()!
        // 绘制背景
        context.setFillColor(backgroundColor?.cgColor ?? .clear)
        context.fill(bounds)
        // 绘制内容
        // ...
    }
}
```

---

## 5. JavaScript 引擎 (JavaScriptCore)

### 5.1 JavaScriptCore 架构

JavaScriptCore (JSC) 是 WebKit 的 JavaScript 引擎，也是 iOS 上可以直接使用的框架。

```
JS 源代码
    ↓
┌─────────────┐
│   Parser    │  ← 词法/语法分析
└─────────────┘
    ↓
┌─────────────┐
│  ByteCode   │  ← 生成字节码
└─────────────┘
    ↓
┌─────────────────────────────────┐
│         执行层 (分层优化)         │
├─────────────┬─────────┬─────────┤
│    LLInt    │   JIT   │  DFG/   │
│  (解释器)    │ (基线)  │  FTL    │
│  最快启动    │  平衡    │ 最优化  │
└─────────────┴─────────┴─────────┘
```

### 5.2 执行层级详解

#### LLInt (Low Level Interpreter)

**路径**: `Source/JavaScriptCore/llint/`

```cpp
// LLInt 是最快启动的执行方式
// 直接解释执行字节码，不需要编译时间

// 字节码示例
// 对于: let a = 1 + 2;
OpAdd(dst: a, src1: 1, src2: 2)
```

#### Baseline JIT

**路径**: `Source/JavaScriptCore/jit/`

```cpp
// 当函数被多次调用时，触发 Baseline JIT
// 生成机器码，但不做优化

// 判断是否需要 JIT
bool shouldJIT(CodeBlock* codeBlock) {
    return codeBlock->llintExecuteCounter() > Options::thresholdForJIT;
}
```

#### DFG (Data Flow Graph) JIT

**路径**: `Source/JavaScriptCore/dfg/`

进行类型推测和优化：

```cpp
// DFG 会分析变量类型，进行优化
// 例如：如果检测到 a 总是 int，就生成 int 专用代码

// 类型推测
SpeculatedType speculatedType = profile->probableType();
if (speculatedType == SpecInt32) {
    // 生成 int32 优化代码
}
```

#### FTL (Faster Than Light)

**路径**: `Source/JavaScriptCore/ftl/`

最高级别的优化，使用 LLVM 后端（在某些平台上）。

### 5.3 iOS 中使用 JavaScriptCore

```swift
import JavaScriptCore

// 创建 JS 上下文
let context = JSContext()!

// 执行 JS 代码
let result = context.evaluateScript("1 + 2")
print(result?.toInt32() ?? 0)  // 输出: 3

// 暴露 Swift 函数给 JS
let logFunc: @convention(block) (String) -> Void = { message in
    print("JS says: \(message)")
}
context.setObject(logFunc, forKeyedSubscript: "nativeLog" as NSString)
context.evaluateScript("nativeLog('Hello from JavaScript!')")

// 从 JS 获取对象
context.evaluateScript("var person = { name: 'John', age: 30 }")
let person = context.objectForKeyedSubscript("person")
let name = person?.objectForKeyedSubscript("name")?.toString()
```

### 5.4 JSC 内部对象模型

**路径**: `Source/JavaScriptCore/runtime/`

```cpp
// JSValue - 所有 JS 值的基类
// Source/JavaScriptCore/runtime/JSCJSValue.h
class JSValue {
    // 使用"标记指针"技术，高效存储不同类型
    // 小整数直接存在指针里，不需要堆分配
};

// JSObject - 所有 JS 对象的基类  
// Source/JavaScriptCore/runtime/JSObject.h
class JSObject : public JSCell {
public:
    // 属性存储
    Structure* structure() const;  // 对象的"形状"
    Butterfly* butterfly();        // 属性存储区
    
    // 属性操作
    bool getOwnPropertySlot(PropertyName, PropertySlot&);
    bool put(PropertyName, JSValue);
};
```

### 5.5 垃圾回收 (Garbage Collection)

**路径**: `Source/JavaScriptCore/heap/`

JSC 使用**标记-清除** (Mark-Sweep) 算法：

```cpp
// Source/JavaScriptCore/heap/Heap.h
class Heap {
public:
    // 分配内存
    void* allocate(size_t);
    
    // 触发 GC
    void collect(CollectionScope);
    
    // GC 阶段
    void markRoots();       // 标记阶段：从根对象开始遍历
    void sweep();           // 清除阶段：回收未标记对象
};
```

#### iOS 类比

```swift
// JSC 的 GC 类似于 Swift 的 ARC，但机制不同：
// - ARC: 引用计数，立即释放
// - GC: 标记清除，批量释放

// 在 iOS 中使用 JSManagedValue 防止循环引用
let managedValue = JSManagedValue(value: jsValue, andOwner: self)
context.virtualMachine.addManagedReference(managedValue, withOwner: self)
```

---

## 6. 多进程架构

### 6.1 进程模型概述

现代 WebKit (WebKit2) 使用多进程架构，这是 iOS 上 WKWebView 比 UIWebView 更安全、更稳定的原因。

```
┌─────────────────────────────────────────────────────┐
│                    你的 iOS App                      │
│  ┌───────────────────────────────────────────────┐  │
│  │              UI Process (UIProcess)            │  │
│  │  • WKWebView                                   │  │
│  │  • WebPageProxy                                │  │
│  │  • 处理用户输入                                  │  │
│  └───────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────┘
                          │ IPC
                          ▼
┌─────────────────────────────────────────────────────┐
│              Web Content Process (WebProcess)        │
│  ┌───────────────────────────────────────────────┐  │
│  │  • WebPage                                     │  │
│  │  • DOM/渲染/JavaScript                          │  │
│  │  • 沙盒隔离，无法访问文件系统                     │  │
│  └───────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────┘
                          │ IPC
                          ▼
┌─────────────────────────────────────────────────────┐
│              Network Process                         │
│  • 处理所有网络请求                                   │
│  • Cookie 管理                                       │
│  • 缓存管理                                          │
└─────────────────────────────────────────────────────┘
```

### 6.2 UIProcess - UI 进程

**路径**: `Source/WebKit/UIProcess/`

这是你的 App 进程，包含 WKWebView 的公开 API。

```
UIProcess/
├── API/
│   └── Cocoa/
│       ├── WKWebView.h          # WKWebView 接口
│       ├── WKWebView.mm         # WKWebView 实现
│       ├── WKNavigationDelegate.h
│       └── WKUIDelegate.h
├── WebPageProxy.cpp             # 代理 WebProcess 中的 WebPage
├── WebProcessProxy.cpp          # 管理 WebProcess
└── ios/
    └── WKContentView.mm         # iOS 特有的内容视图
```

### 6.3 WebProcess - Web 内容进程

**路径**: `Source/WebKit/WebProcess/`

实际执行网页渲染和 JavaScript 的进程。

```
WebProcess/
├── WebPage/
│   ├── WebPage.cpp              # 代表一个网页
│   └── WebFrame.cpp             # 代表一个 frame
├── WebCoreSupport/
│   └── WebChromeClient.cpp      # WebCore 的回调接口
└── WebProcess.cpp               # 进程主类
```

### 6.4 进程间通信 (IPC)

**路径**: `Source/WebKit/Platform/IPC/`

WebKit 使用自定义的 IPC 系统进行进程间通信。

```cpp
// 消息定义示例
// Source/WebKit/UIProcess/WebPageProxy.messages.in

messages -> WebPageProxy {
    // 页面加载完成
    DidFinishLoadForFrame(WebKit::FrameInfoData frameInfo)
    
    // 标题改变
    TitleChanged(String title)
    
    // 需要显示 alert
    RunJavaScriptAlert(String message)
}
```

#### iOS 类比

```swift
// IPC 类似于 iOS 中的 XPC 或 Distributed Notifications
// 但 WebKit 有自己的高效实现

// 跨进程调用类似于：
let connection = NSXPCConnection(serviceName: "com.apple.WebProcess")
let proxy = connection.remoteObjectProxy as? WebPageProtocol
proxy?.loadURL(url)
```

### 6.5 多进程的好处

| 优势 | 说明 |
|-----|------|
| **安全性** | Web 内容在沙盒中运行，无法直接访问 App 数据 |
| **稳定性** | Web 进程崩溃不会影响主 App |
| **性能** | 多进程可以利用多核 CPU |
| **内存管理** | 可以单独终止占用内存过大的 Web 进程 |

这就是为什么 WKWebView 比 UIWebView 更推荐使用的原因！

---

## 7. WKWebView 与 iOS 开发

### 7.1 WKWebView 类结构

**路径**: `Source/WebKit/UIProcess/API/Cocoa/WKWebView.h`

```objc
@interface WKWebView : UIView

// 配置
@property (nonatomic, readonly, copy) WKWebViewConfiguration *configuration;

// 代理
@property (nullable, weak) id <WKNavigationDelegate> navigationDelegate;
@property (nullable, weak) id <WKUIDelegate> UIDelegate;

// 导航
- (WKNavigation *)loadRequest:(NSURLRequest *)request;
- (WKNavigation *)loadHTMLString:(NSString *)string baseURL:(NSURL *)baseURL;
- (WKNavigation *)goBack;
- (WKNavigation *)goForward;

// 状态
@property (readonly) NSURL *URL;
@property (readonly) NSString *title;
@property (readonly) double estimatedProgress;
@property (readonly, getter=isLoading) BOOL loading;

// JavaScript
- (void)evaluateJavaScript:(NSString *)javaScript 
         completionHandler:(void (^)(id, NSError *))completionHandler;

// 截图
- (void)takeSnapshotWithConfiguration:(WKSnapshotConfiguration *)config
                    completionHandler:(void (^)(UIImage *, NSError *))completionHandler;

@end
```

### 7.2 WKWebView 内部实现

**路径**: `Source/WebKit/UIProcess/API/Cocoa/WKWebView.mm`

```objc
@implementation WKWebView {
    // 核心：WebPageProxy 是与 WebProcess 通信的桥梁
    std::unique_ptr<WebKit::WebPageProxy> _page;
    
    // iOS 特有：内容视图
    RetainPtr<WKContentView> _contentView;
    
    // 滚动视图
    RetainPtr<WKScrollView> _scrollView;
}

- (instancetype)initWithFrame:(CGRect)frame configuration:(WKWebViewConfiguration *)configuration
{
    // 1. 创建 UIView
    if (!(self = [super initWithFrame:frame]))
        return nil;
    
    // 2. 初始化 WebPageProxy（连接到 WebProcess）
    _page = WebPageProxy::create(/* ... */);
    
    // 3. 创建内容视图
    _contentView = adoptNS([[WKContentView alloc] initWithFrame:bounds]);
    
    // 4. 创建滚动视图
    _scrollView = adoptNS([[WKScrollView alloc] initWithFrame:bounds]);
    [_scrollView addSubview:_contentView.get()];
    [self addSubview:_scrollView.get()];
    
    return self;
}

- (WKNavigation *)loadRequest:(NSURLRequest *)request
{
    // 通过 IPC 发送给 WebProcess
    return _page->loadRequest(request);
}

@end
```

### 7.3 WKContentView - iOS 内容视图

**路径**: `Source/WebKit/UIProcess/ios/WKContentView.mm`

```objc
@interface WKContentView : UIView <UIGestureRecognizerDelegate>

// 处理触摸事件
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;

// 手势识别器
@property (nonatomic, readonly) UITapGestureRecognizer *singleTapGestureRecognizer;
@property (nonatomic, readonly) UILongPressGestureRecognizer *longPressGestureRecognizer;

// 输入处理
- (void)_setUpTextSelectionAssistant;
- (void)_showKeyboard;

@end
```

### 7.4 与原生交互

#### Swift 调用 JavaScript

```swift
// 执行 JS 代码
webView.evaluateJavaScript("document.title") { result, error in
    if let title = result as? String {
        print("页面标题: \(title)")
    }
}

// 注入 JS 脚本
let script = WKUserScript(
    source: "console.log('Hello from Swift!')",
    injectionTime: .atDocumentStart,
    forMainFrameOnly: true
)
webView.configuration.userContentController.addUserScript(script)
```

#### JavaScript 调用 Swift

```swift
// 1. 添加消息处理器
webView.configuration.userContentController.add(self, name: "nativeHandler")

// 2. 实现协议
extension ViewController: WKScriptMessageHandler {
    func userContentController(_ controller: WKUserContentController,
                               didReceive message: WKScriptMessage) {
        if message.name == "nativeHandler" {
            print("收到消息: \(message.body)")
        }
    }
}

// 3. JS 端调用
// window.webkit.messageHandlers.nativeHandler.postMessage('Hello from JS!')
```

### 7.5 WKWebView 生命周期

```swift
class WebViewController: UIViewController {
    var webView: WKWebView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        // 配置
        let config = WKWebViewConfiguration()
        
        // 创建（此时会启动 WebProcess）
        webView = WKWebView(frame: view.bounds, configuration: config)
        webView.navigationDelegate = self
        view.addSubview(webView)
        
        // 加载
        let url = URL(string: "https://apple.com")!
        webView.load(URLRequest(url: url))
    }
    
    deinit {
        // WKWebView 释放时，会清理 WebProcess 资源
    }
}

extension WebViewController: WKNavigationDelegate {
    // 开始导航
    func webView(_ webView: WKWebView,
                 didStartProvisionalNavigation navigation: WKNavigation!) {
        print("开始加载")
    }
    
    // 收到响应
    func webView(_ webView: WKWebView,
                 didReceiveServerRedirectForProvisionalNavigation navigation: WKNavigation!) {
        print("重定向")
    }
    
    // 内容开始到达
    func webView(_ webView: WKWebView,
                 didCommit navigation: WKNavigation!) {
        print("开始渲染")
    }
    
    // 加载完成
    func webView(_ webView: WKWebView,
                 didFinish navigation: WKNavigation!) {
        print("加载完成")
    }
    
    // 加载失败
    func webView(_ webView: WKWebView,
                 didFail navigation: WKNavigation!,
                 withError error: Error) {
        print("加载失败: \(error)")
    }
}
```

---

## 8. 网络与资源加载

### 8.1 资源加载架构

**路径**: `Source/WebCore/loader/`

```
网页请求
    ↓
┌─────────────────┐
│  DocumentLoader │  ← 文档加载器（主文档）
└─────────────────┘
    ↓
┌─────────────────┐
│  FrameLoader    │  ← Frame 加载器
└─────────────────┘
    ↓
┌─────────────────┐
│ ResourceLoader  │  ← 资源加载器
└─────────────────┘
    ↓
┌─────────────────┐
│   网络层         │  ← 实际网络请求
└─────────────────┘
```

### 8.2 FrameLoader - 帧加载器

**路径**: `Source/WebCore/loader/FrameLoader.h`

```cpp
class FrameLoader {
public:
    // 加载请求
    void load(FrameLoadRequest&&);
    
    // 加载 HTML 字符串
    void loadHTMLString(const String& html, const URL& baseURL);
    
    // 导航控制
    void reload();
    void stopAllLoaders();
    
    // 状态
    FrameLoadType loadType() const;
    bool isLoading() const;
    
    // 历史管理
    HistoryController& history();
};
```

### 8.3 ResourceLoader - 资源加载器

**路径**: `Source/WebCore/loader/ResourceLoader.h`

```cpp
class ResourceLoader : public ResourceHandleClient {
public:
    // 资源类型
    enum Type {
        MainResource,      // 主文档
        Image,            // 图片
        CSSStyleSheet,    // 样式表
        Script,           // 脚本
        Font,             // 字体
        // ...
    };
    
    // 加载回调
    void didReceiveResponse(const ResourceResponse&);
    void didReceiveData(const SharedBuffer&, long long);
    void didFinishLoading();
    void didFail(const ResourceError&);
};
```

### 8.4 缓存机制

**路径**: `Source/WebCore/loader/cache/`

```cpp
// CachedResource - 缓存的资源
class CachedResource {
public:
    enum Status {
        Unknown,      // 未知
        Pending,      // 加载中
        Cached,       // 已缓存
        LoadError,    // 加载错误
        DecodeError   // 解码错误
    };
    
    // 数据
    SharedBuffer* data() const;
    
    // 验证
    bool mustRevalidateDueToCacheHeaders() const;
};

// MemoryCache - 内存缓存
class MemoryCache {
public:
    CachedResource* resourceForRequest(const ResourceRequest&);
    void add(CachedResource&);
    void remove(CachedResource&);
};
```

#### iOS 类比

```swift
// 类似 URLCache
let cache = URLCache.shared
let cachedResponse = cache.cachedResponse(for: request)
```

### 8.5 网络进程通信

在多进程架构中，实际的网络请求在 NetworkProcess 中执行：

```
WKWebView (UIProcess)
       ↓
    IPC 消息
       ↓
WebProcess (请求发起)
       ↓
    IPC 消息  
       ↓
NetworkProcess (实际网络请求)
       ↓
    URLSession
       ↓
    网络
```

---

## 9. 事件处理系统

### 9.1 事件流概述

DOM 事件遵循 W3C 标准的三个阶段：

```
Document          1. 捕获阶段 (Capture)
    ↓
  <html>          事件从 Document 向下传播
    ↓
  <body>
    ↓
   <div>          2. 目标阶段 (Target)
    ↓             事件到达目标元素
  <button>  ← 点击目标
    ↑
   <div>          3. 冒泡阶段 (Bubble)
    ↑
  <body>          事件从目标向上传播
    ↑
  <html>
    ↑
Document
```

### 9.2 EventDispatcher - 事件分发器

**路径**: `Source/WebCore/dom/EventDispatcher.cpp`

```cpp
void EventDispatcher::dispatchEvent(Node& node, Event& event)
{
    // 1. 构建事件路径
    EventPath eventPath(node, event);
    
    // 2. 捕获阶段
    event.setEventPhase(Event::CAPTURING_PHASE);
    for (size_t i = eventPath.size(); i > 0; --i) {
        eventPath.contextAt(i - 1).handleLocalEvents(event, Capturing);
        if (event.propagationStopped())
            return;
    }
    
    // 3. 目标阶段
    event.setEventPhase(Event::AT_TARGET);
    eventPath.contextAt(0).handleLocalEvents(event, Bubbling);
    
    // 4. 冒泡阶段
    if (event.bubbles()) {
        event.setEventPhase(Event::BUBBLING_PHASE);
        for (size_t i = 1; i < eventPath.size(); ++i) {
            eventPath.contextAt(i).handleLocalEvents(event, Bubbling);
            if (event.propagationStopped())
                return;
        }
    }
    
    // 5. 默认处理
    callDefaultEventHandlers(event, eventPath);
}
```

### 9.3 Event - 事件基类

**路径**: `Source/WebCore/dom/Event.h`

```cpp
class Event : public RefCounted<Event>, public EventTarget {
public:
    // 事件类型
    const AtomString& type() const;
    
    // 目标
    EventTarget* target() const;
    EventTarget* currentTarget() const;
    
    // 传播控制
    void stopPropagation();      // 停止传播
    void stopImmediatePropagation();  // 立即停止
    void preventDefault();       // 阻止默认行为
    
    // 阶段
    unsigned short eventPhase() const;
    
    // 冒泡
    bool bubbles() const;
    bool cancelable() const;
};
```

#### iOS 类比

```swift
// DOM 事件系统类似于 UIKit 的 Responder Chain
// 但方向相反：
// - DOM: 先捕获（向下）再冒泡（向上）
// - UIKit: 事件向上传递（通过 responder chain）

class UIResponder {
    func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?)
    var next: UIResponder? { get }  // 响应链下一个
}
```

### 9.4 触摸事件处理 (iOS)

**路径**: `Source/WebKit/UIProcess/ios/WKContentView.mm`

```objc
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    // 1. 创建 WebTouchEvent
    WebTouchEvent webEvent = [self _touchEventForTouches:touches];
    
    // 2. 通过 IPC 发送给 WebProcess
    _page->handleTouchEvent(webEvent);
}

// WebProcess 中的处理
// Source/WebKit/WebProcess/WebPage/WebPage.cpp
void WebPage::handleTouchEvent(const WebTouchEvent& event)
{
    // 1. 命中测试，找到触摸点下的元素
    HitTestResult result = hitTest(event.position());
    
    // 2. 创建 DOM TouchEvent
    Ref<TouchEvent> touchEvent = TouchEvent::create(/* ... */);
    
    // 3. 分发事件
    result.targetNode()->dispatchEvent(touchEvent);
}
```

### 9.5 事件监听器

**路径**: `Source/WebCore/dom/EventTarget.h`

```cpp
class EventTarget {
public:
    // 添加监听器
    bool addEventListener(const AtomString& type,
                         RefPtr<EventListener>,
                         AddEventListenerOptions);
    
    // 移除监听器
    bool removeEventListener(const AtomString& type,
                            EventListener&,
                            ListenerOptions);
    
    // 分发事件
    bool dispatchEvent(Event&);
};
```

---

## 10. CSS 样式系统

### 10.1 样式系统架构

```
CSS 源文本
    ↓
┌─────────────────┐
│   CSS Parser    │  ← 解析 CSS
└─────────────────┘
    ↓
┌─────────────────┐
│  StyleSheet     │  ← 样式表对象
└─────────────────┘
    ↓
┌─────────────────┐
│  StyleResolver  │  ← 样式解析器
└─────────────────┘
    ↓
┌─────────────────┐
│  RenderStyle    │  ← 计算后的样式
└─────────────────┘
```

### 10.2 StyleResolver - 样式解析器

**路径**: `Source/WebCore/style/StyleResolver.h`

```cpp
class StyleResolver {
public:
    // 为元素解析样式
    RenderStyle* styleForElement(Element&, const RenderStyle* parentStyle);
    
    // 样式匹配
    void matchAllRules(ElementRuleCollector&, bool includeSMILProperties);
    
    // 样式继承
    void applyInheritedOnlyStyleFromParent(RenderStyle&, const RenderStyle* parentStyle);
};
```

### 10.3 样式优先级（级联）

CSS 样式的优先级由高到低：

```
1. !important 声明
2. 行内样式 (style 属性)
3. ID 选择器 (#id)
4. 类选择器/属性选择器/伪类 (.class, [attr], :hover)
5. 元素选择器/伪元素 (div, ::before)
6. 通用选择器 (*)
7. 继承的样式
```

```cpp
// Source/WebCore/css/SelectorChecker.cpp
// 计算选择器特异性
unsigned Selector::specificity() const
{
    unsigned idCount = 0;
    unsigned classCount = 0;
    unsigned elementCount = 0;
    
    for (const Selector* selector = this; selector; selector = selector->next()) {
        switch (selector->match()) {
        case ID:
            idCount++;
            break;
        case Class:
        case Attribute:
        case PseudoClass:
            classCount++;
            break;
        case Tag:
        case PseudoElement:
            elementCount++;
            break;
        }
    }
    
    // 返回特异性值：ID * 100 + Class * 10 + Element
    return idCount * 100 + classCount * 10 + elementCount;
}
```

### 10.4 RenderStyle - 计算后的样式

**路径**: `Source/WebCore/rendering/style/RenderStyle.h`

```cpp
class RenderStyle {
public:
    // 盒模型
    Length width() const;
    Length height() const;
    Length marginTop() const;
    Length paddingLeft() const;
    
    // 定位
    PositionType position() const;  // static, relative, absolute, fixed
    
    // 显示
    DisplayType display() const;  // block, inline, flex, grid
    
    // 背景
    Color backgroundColor() const;
    
    // 文本
    TextAlign textAlign() const;
    Color color() const;
    const Font& font() const;
    
    // 变换
    TransformOperations& transform();
    
    // 动画
    const AnimationList* transitions() const;
    const AnimationList* animations() const;
};
```

### 10.5 样式计算流程

当需要计算一个元素的样式时：

```cpp
// Source/WebCore/style/StyleTreeResolver.cpp
void StyleTreeResolver::resolveElement(Element& element)
{
    // 1. 获取父元素样式（用于继承）
    const RenderStyle* parentStyle = element.parentElement()
        ? element.parentElement()->renderStyle()
        : nullptr;
    
    // 2. 收集所有匹配的 CSS 规则
    ElementRuleCollector collector(element);
    collector.matchAllRules();
    
    // 3. 按优先级排序规则
    collector.sortMatchedRules();
    
    // 4. 应用规则，生成 RenderStyle
    RenderStyle* style = styleResolver.styleForElement(
        element, parentStyle);
    
    // 5. 设置到元素上
    element.setRenderStyle(style);
}
```

---

## 附录 A：调试技巧

### 在 iOS 开发中调试 WebView

```swift
// 1. 启用调试
#if DEBUG
webView.isInspectable = true  // iOS 16.4+
#endif

// 2. 在 Safari 中调试
// Safari -> 开发 -> 你的设备 -> 你的页面

// 3. 监控加载进度
webView.publisher(for: \.estimatedProgress)
    .sink { progress in
        print("加载进度: \(progress * 100)%")
    }
    .store(in: &cancellables)

// 4. 监控 URL 变化
webView.publisher(for: \.url)
    .sink { url in
        print("当前 URL: \(url?.absoluteString ?? "nil")")
    }
    .store(in: &cancellables)
```

### 常见问题排查

| 问题 | 可能原因 | 解决方案 |
|-----|---------|---------|
| 白屏 | 进程崩溃 | 检查内存使用，实现 `webViewWebContentProcessDidTerminate` |
| 加载慢 | 网络问题或资源过多 | 使用 Safari 开发者工具分析 |
| JS 不执行 | 安全策略 | 检查 CSP 头，使用 `WKUserScript` |
| 内存暴涨 | 循环引用或缓存过大 | 检查 JS/Native 交互中的引用 |

---

## 附录 B：关键文件速查

| 功能 | 路径 |
|-----|------|
| DOM Node | `Source/WebCore/dom/Node.h` |
| Document | `Source/WebCore/dom/Document.h` |
| 渲染对象 | `Source/WebCore/rendering/RenderObject.h` |
| 布局 | `Source/WebCore/rendering/RenderBlockFlow.cpp` |
| 样式 | `Source/WebCore/style/StyleResolver.h` |
| JS 引擎 | `Source/JavaScriptCore/runtime/` |
| WKWebView | `Source/WebKit/UIProcess/API/Cocoa/WKWebView.mm` |
| 事件分发 | `Source/WebCore/dom/EventDispatcher.cpp` |
| 网络加载 | `Source/WebCore/loader/FrameLoader.h` |
| IPC | `Source/WebKit/Platform/IPC/` |

---

## 总结

作为 iOS 开发工程师，理解 WebKit 可以帮助你：

1. **优化 WKWebView 性能** - 知道哪些操作会触发重新布局/重绘
2. **正确处理 JS 交互** - 理解跨进程通信的开销
3. **调试 Web 问题** - 知道从哪里找问题根源
4. **做出架构决策** - 理解 WebView 的能力边界

WebKit 的代码质量非常高，是学习大型 C++ 项目架构的绝佳材料。希望这份文档能帮助你在 iOS 开发中更好地使用和调试 WebView！
