# WebKit 进阶指南：iOS 开发实战篇

> 本文档是 WebKit 架构分析的进阶篇，聚焦于 iOS 开发中的实际应用场景，帮助你更好地理解和使用 WKWebView。

## 目录

1. [WKWebView 完整架构](#1-wkwebview-完整架构)
2. [JavaScript 与 Native 交互深度分析](#2-javascript-与-native-交互深度分析)
3. [渲染性能优化原理](#3-渲染性能优化原理)
4. [内存管理机制](#4-内存管理机制)
5. [安全模型](#5-安全模型)
6. [实战：常见问题源码级分析](#6-实战常见问题源码级分析)

---

## 1. WKWebView 完整架构

### 1.1 类层次结构

从源码角度看 WKWebView 的完整类层次：

```
WKWebView (UIView 子类)
    │
    ├── WKScrollView (UIScrollView 子类)
    │       └── WKContentView (UIView 子类)
    │               ├── 手势识别器集合
    │               ├── 输入辅助控件
    │               └── 实际内容渲染
    │
    └── WebPageProxy (C++ 对象)
            │
            ├── WebProcessProxy (管理 Web 进程)
            │
            └── 通过 IPC 连接到 WebProcess
                    └── WebPage (C++ 对象)
                            └── LocalFrame / Document / ...
```

### 1.2 WKScrollView 的特殊性

**路径**: `Source/WebKit/UIProcess/ios/WKScrollView.h`

```objc
// WKScrollView 不是普通的 UIScrollView
// 它有很多定制行为来支持 Web 内容
@interface WKScrollView : UIScrollView

// 减速行为定制（网页滚动的"弹性"感觉）
@property (nonatomic) BOOL _allowsDoubleTapGestures;

// 捏合缩放
@property (nonatomic) CGFloat minimumZoomScale;
@property (nonatomic) CGFloat maximumZoomScale;

// 橡皮筋效果控制
@property (nonatomic) BOOL _alwaysBounceHorizontal;
@property (nonatomic) BOOL _alwaysBounceVertical;

@end
```

#### iOS 开发者注意

```swift
// ⚠️ 直接访问 WKWebView.scrollView 的一些陷阱

let webView = WKWebView()

// ✅ 可以安全修改
webView.scrollView.bounces = false
webView.scrollView.showsVerticalScrollIndicator = false

// ⚠️ 需要谨慎，可能影响缩放行为
webView.scrollView.minimumZoomScale = 1.0
webView.scrollView.maximumZoomScale = 1.0

// ❌ 不要这样做，会破坏 WKWebView 内部逻辑
webView.scrollView.delegate = self  // 覆盖了内部 delegate
```

### 1.3 WebPageProxy - 核心代理对象

**路径**: `Source/WebKit/UIProcess/WebPageProxy.h`

WebPageProxy 是 UIProcess 中代表 WebProcess 里 WebPage 的对象。

```cpp
class WebPageProxy : public API::Object, 
                     public IPC::MessageReceiver,
                     public IPC::MessageSender {
public:
    // 导航相关
    void loadRequest(const WebCore::ResourceRequest&);
    void loadHTMLString(const String& html, const String& baseURL);
    void goBack();
    void goForward();
    void reload();
    void stopLoading();
    
    // JavaScript 执行
    void evaluateJavaScript(const String& script, 
                           CompletionHandler<void(WebCore::ValueOrException&&)>&&);
    
    // 状态获取
    const String& title() const;
    const URL& url() const;
    double estimatedProgress() const;
    bool isLoading() const;
    
    // 内容管理
    void takeSnapshot(const IntRect&, 
                     CompletionHandler<void(std::optional<ShareableBitmap::Handle>&&)>&&);
    
    // 进程管理
    WebProcessProxy& process() const;
    void terminateProcess();
    
private:
    // IPC 消息处理
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) override;
    
    // 与 WebProcess 的连接
    Ref<WebProcessProxy> m_process;
    
    // 状态
    String m_title;
    URL m_url;
    double m_estimatedProgress;
};
```

### 1.4 WebPage - 真正的网页对象

**路径**: `Source/WebKit/WebProcess/WebPage/WebPage.h`

WebPage 在 WebProcess 中，真正持有 DOM 和渲染树。

```cpp
class WebPage : public API::Object,
                public IPC::MessageReceiver,
                public IPC::MessageSender {
public:
    // 核心 WebCore 对象
    WebCore::Page& corePage() const { return *m_page; }
    WebCore::LocalFrame& mainFrame() const;
    WebCore::Document* mainFrameDocument() const;
    
    // 导航处理
    void loadRequest(const WebCore::ResourceRequest&);
    void loadHTMLString(const String& html, const URL& baseURL);
    
    // 事件处理
    void mouseEvent(const WebMouseEvent&);
    void keyEvent(const WebKeyboardEvent&);
    void touchEvent(const WebTouchEvent&);
    
    // 渲染
    void drawRect(WebCore::GraphicsContext&, const WebCore::IntRect&);
    
    // JavaScript
    void runJavaScript(const String& script, 
                      WebCore::RunJavaScriptParameters&&,
                      CompletionHandler<void(WebCore::ValueOrException&&)>&&);

private:
    // WebCore 的 Page 对象
    std::unique_ptr<WebCore::Page> m_page;
    
    // Frame 管理
    HashMap<WebCore::FrameIdentifier, RefPtr<WebFrame>> m_frameMap;
};
```

---

## 2. JavaScript 与 Native 交互深度分析

### 2.1 evaluateJavaScript 的完整流程

当你调用 `webView.evaluateJavaScript("...")` 时：

```
你的 Swift 代码
       │
       ▼
┌─────────────────────────────────┐
│  WKWebView.evaluateJavaScript   │  UIProcess
│  转换为 Objective-C 调用          │
└─────────────────────────────────┘
       │
       ▼
┌─────────────────────────────────┐
│  WebPageProxy::evaluateJavaScript│
│  序列化参数，准备 IPC 消息         │
└─────────────────────────────────┘
       │
       │ IPC::Connection::send()
       ▼
═══════════════════════════════════  进程边界
       │
       │ Mach ports / Unix sockets
       ▼
┌─────────────────────────────────┐
│  WebPage::runJavaScript          │  WebProcess
│  接收并解析 IPC 消息               │
└─────────────────────────────────┘
       │
       ▼
┌─────────────────────────────────┐
│  LocalFrame::script()            │
│  获取 JSC 执行上下文               │
└─────────────────────────────────┘
       │
       ▼
┌─────────────────────────────────┐
│  JSC::evaluate()                 │
│  实际执行 JavaScript 代码          │
└─────────────────────────────────┘
       │
       │ 执行结果
       ▼
═══════════════════════════════════  进程边界（返回）
       │
       ▼
┌─────────────────────────────────┐
│  CompletionHandler 回调           │
│  结果反序列化，回调到主线程         │
└─────────────────────────────────┘
       │
       ▼
你的 completionHandler 闭包
```

### 2.2 IPC 序列化成本

**路径**: `Source/WebKit/Shared/WebCoreArgumentCoders.serialization.in`

```cpp
// JavaScript 返回值需要序列化传递
// 源码中定义了各种类型的序列化方式

// 简单类型直接传
// 复杂对象需要序列化
struct SerializedScriptValue {
    Vector<uint8_t> data;  // 序列化后的二进制数据
};
```

#### iOS 开发最佳实践

```swift
// ❌ 低效：每次调用都跨进程
for i in 0..<1000 {
    webView.evaluateJavaScript("updateItem(\(i))")
}

// ✅ 高效：批量处理
let script = (0..<1000).map { "updateItem(\($0))" }.joined(separator: ";")
webView.evaluateJavaScript(script)

// ✅ 更高效：传递 JSON 数据一次性处理
let items = (0..<1000).map { $0 }
let json = try! JSONEncoder().encode(items)
let jsonString = String(data: json, encoding: .utf8)!
webView.evaluateJavaScript("updateItems(\(jsonString))")
```

### 2.3 WKScriptMessageHandler 内部原理

**路径**: `Source/WebKit/WebProcess/UserContent/WebUserContentController.cpp`

当 JS 调用 `window.webkit.messageHandlers.xxx.postMessage()` 时：

```cpp
// 1. JS 端调用 postMessage
// 这是注入到页面的 JS 对象

// 2. 触发 native 绑定
// Source/WebCore/bindings/js/JSDOMWindowCustom.cpp
void JSDOMWindow::postMessage(/* ... */)
{
    // 序列化消息
    auto serializedValue = SerializedScriptValue::create(/* ... */);
    
    // 发送 IPC 消息
    sendMessage(messageName, WTFMove(serializedValue));
}

// 3. UIProcess 收到消息
// Source/WebKit/UIProcess/WebPageProxy.cpp
void WebPageProxy::scriptMessageReceived(const FrameInfoData& frameInfo,
                                        const String& name,
                                        const UserData& messageBody)
{
    // 找到对应的 handler
    auto* handler = m_scriptMessageHandlers.get(name);
    
    // 调用 Objective-C 代理方法
    [handler->delegate() userContentController:controller 
                      didReceiveScriptMessage:message];
}
```

### 2.4 UserScript 注入时机

**路径**: `Source/WebKit/WebProcess/UserContent/WebUserContentController.cpp`

```cpp
enum class WKUserScriptInjectionTime {
    WKUserScriptInjectionTimeAtDocumentStart,  // 文档开始构建时
    WKUserScriptInjectionTimeAtDocumentEnd     // DOM 构建完成后
};
```

#### 注入时机详解

```
网络响应到达
       │
       ▼
┌─────────────────────────────────┐
│   HTML 解析开始                   │
│   创建空的 Document 对象           │
└─────────────────────────────────┘
       │
       │ ← AtDocumentStart 脚本在这里执行
       │   此时 DOM 几乎是空的
       ▼
┌─────────────────────────────────┐
│   解析 <head> 内容                │
│   加载外部 CSS/JS                 │
└─────────────────────────────────┘
       │
       ▼
┌─────────────────────────────────┐
│   解析 <body> 内容                │
│   构建 DOM 树                     │
└─────────────────────────────────┘
       │
       │ ← AtDocumentEnd 脚本在这里执行
       │   此时 DOM 完整，但图片/样式可能未加载完
       ▼
┌─────────────────────────────────┐
│   DOMContentLoaded 事件           │
└─────────────────────────────────┘
       │
       ▼
┌─────────────────────────────────┐
│   子资源加载完成                   │
│   load 事件                       │
└─────────────────────────────────┘
```

#### iOS 开发选择指南

```swift
// AtDocumentStart 适用场景：
// - 需要在页面 JS 执行前修改环境
// - 拦截或重写页面函数
// - 注入 polyfill

let earlyScript = WKUserScript(
    source: """
    // 拦截 console.log
    const originalLog = console.log;
    console.log = function(...args) {
        window.webkit.messageHandlers.console.postMessage(args.join(' '));
        originalLog.apply(console, args);
    };
    """,
    injectionTime: .atDocumentStart,
    forMainFrameOnly: false
)

// AtDocumentEnd 适用场景：
// - 需要访问 DOM 元素
// - 添加事件监听器
// - 修改页面内容

let lateScript = WKUserScript(
    source: """
    // 给所有链接添加点击监听
    document.querySelectorAll('a').forEach(a => {
        a.addEventListener('click', e => {
            window.webkit.messageHandlers.linkClick.postMessage(a.href);
        });
    });
    """,
    injectionTime: .atDocumentEnd,
    forMainFrameOnly: true
)
```

---

## 3. 渲染性能优化原理

### 3.1 渲染流水线瓶颈

WebKit 的渲染流水线每一步都可能成为瓶颈：

```
┌─────────────┐     ┌─────────────┐     ┌─────────────┐
│   Style     │ →→→ │   Layout    │ →→→ │   Paint     │
│ Recalculate │     │  (Reflow)   │     │  (Repaint)  │
└─────────────┘     └─────────────┘     └─────────────┘
     │                    │                    │
     │                    │                    │
  改变样式             改变几何属性           改变外观
   属性会               会触发              不触发布局
  触发此阶段            此阶段               只触发此阶段
```

### 3.2 什么操作会触发重排（Reflow）？

**路径**: `Source/WebCore/rendering/RenderObject.cpp`

```cpp
void RenderObject::setNeedsLayout(MarkingBehavior markParents)
{
    // 标记需要重新布局
    m_needsLayout = true;
    
    // 向上传播，标记父节点
    if (markParents == MarkContainingBlockChain) {
        if (auto* container = containingBlock())
            container->setChildNeedsLayout();
    }
}
```

#### 触发重排的常见操作

| CSS 属性变化 | 是否触发重排 | 原因 |
|-------------|-------------|------|
| `width`, `height` | ✅ 是 | 影响盒子尺寸 |
| `margin`, `padding` | ✅ 是 | 影响盒子模型 |
| `position` | ✅ 是 | 影响布局方式 |
| `display` | ✅ 是 | 影响布局方式 |
| `font-size` | ✅ 是 | 影响文本布局 |
| `color` | ❌ 否 | 只影响颜色 |
| `background` | ❌ 否 | 只影响背景 |
| `transform` | ❌ 否 | 在合成层处理 |
| `opacity` | ❌ 否 | 在合成层处理 |

#### JavaScript 读取操作也会触发重排！

```javascript
// ❌ 这些读取操作会强制同步布局
element.offsetTop
element.offsetLeft
element.offsetWidth
element.offsetHeight
element.scrollTop
element.clientTop
element.getComputedStyle()
element.getBoundingClientRect()

// ❌ 糟糕的模式：读写交替
for (let el of elements) {
    el.style.height = el.offsetHeight + 10 + 'px';  // 每次循环都重排
}

// ✅ 优化后：批量读，批量写
const heights = elements.map(el => el.offsetHeight);  // 批量读
elements.forEach((el, i) => {
    el.style.height = heights[i] + 10 + 'px';  // 批量写
});
```

### 3.3 合成层优化

**路径**: `Source/WebCore/rendering/RenderLayerCompositor.cpp`

```cpp
// 判断是否需要创建合成层
bool RenderLayerCompositor::requiresCompositingForTransform(RenderLayerModelObject& renderer)
{
    const RenderStyle& style = renderer.style();
    
    // 3D 变换需要合成层
    if (style.transform().has3DOperation())
        return true;
    
    // will-change: transform 也需要
    if (style.willChange().contains(CSSPropertyTransform))
        return true;
    
    return false;
}
```

#### iOS 开发中的应用

```css
/* 创建合成层的 CSS */
.optimized-animation {
    /* 方法 1: 3D 变换 */
    transform: translateZ(0);
    
    /* 方法 2: will-change (更语义化) */
    will-change: transform, opacity;
}
```

```swift
// 在 iOS 中，可以通过 WKWebView 注入性能优化脚本
let optimizeScript = WKUserScript(
    source: """
    // 给动画元素添加合成层提示
    document.querySelectorAll('.animated').forEach(el => {
        el.style.willChange = 'transform, opacity';
    });
    """,
    injectionTime: .atDocumentEnd,
    forMainFrameOnly: true
)
```

### 3.4 帧率和渲染时机

**路径**: `Source/WebCore/page/RenderingUpdateScheduler.cpp`

```cpp
void RenderingUpdateScheduler::scheduleRenderingUpdate()
{
    // 与显示器刷新同步
    // iOS 设备通常是 60Hz (16.67ms) 或 120Hz (8.33ms)
    if (!m_renderingUpdateTaskScheduled) {
        m_renderingUpdateTaskScheduled = true;
        
        // 使用 CADisplayLink 或类似机制
        scheduleAnimation();
    }
}
```

#### 性能监控

```swift
// 监控 WKWebView 的性能
extension WKWebView {
    func injectPerformanceMonitor() {
        let script = """
        (function() {
            let frameCount = 0;
            let lastTime = performance.now();
            
            function checkFPS() {
                frameCount++;
                const currentTime = performance.now();
                if (currentTime - lastTime >= 1000) {
                    window.webkit.messageHandlers.fps.postMessage(frameCount);
                    frameCount = 0;
                    lastTime = currentTime;
                }
                requestAnimationFrame(checkFPS);
            }
            
            requestAnimationFrame(checkFPS);
        })();
        """
        
        evaluateJavaScript(script)
    }
}
```

---

## 4. 内存管理机制

### 4.1 WebKit 的内存架构

```
┌─────────────────────────────────────────────────────┐
│                    你的 App 进程                      │
├─────────────────────────────────────────────────────┤
│  WKWebView (UIView)                                  │
│  • 占用较少内存                                       │
│  • 主要是 UI 相关数据                                 │
└─────────────────────────────────────────────────────┘
                          │
═══════════════════════════════════════════════════════
                          │
┌─────────────────────────────────────────────────────┐
│                    WebProcess                        │
├─────────────────────────────────────────────────────┤
│  DOM 树     → 可能很大 (复杂页面数十 MB)               │
│  渲染树     → 通常小于 DOM 树                         │
│  JS 堆      → 取决于 JS 代码                         │
│  资源缓存   → 图片、字体等                            │
└─────────────────────────────────────────────────────┘
```

### 4.2 DOM 节点内存

**路径**: `Source/WebCore/dom/Node.cpp`

```cpp
// Node 对象的大小是精心优化的
// 使用位域来节省空间
class Node {
    // 紧凑存储
    uint32_t m_nodeFlags;  // 多个标志位打包
    
    // 使用智能指针
    RefPtr<Document> m_document;  // 文档引用
    
    // 树结构指针
    ContainerNode* m_parentNode;
    Node* m_nextSibling;
    Node* m_previousSibling;
};

// 实际内存 footprint 的检查
static_assert(sizeof(Node) == /* 预期大小 */, "Node should stay small");
```

### 4.3 JavaScript 内存与垃圾回收

**路径**: `Source/JavaScriptCore/heap/Heap.cpp`

```cpp
void Heap::collect(CollectionScope collectionScope)
{
    // GC 类型
    // - Eden collection: 只收集新分配的对象（快）
    // - Full collection: 全堆扫描（慢）
    
    switch (collectionScope) {
    case CollectionScope::Eden:
        collectEden();
        break;
    case CollectionScope::Full:
        collectFull();
        break;
    }
}
```

#### iOS 上的内存压力处理

**路径**: `Source/WebKit/UIProcess/WebProcessPool.cpp`

```cpp
void WebProcessPool::handleMemoryPressureWarning(Critical critical)
{
    // 系统内存压力时的响应
    if (critical == Critical::Yes) {
        // 终止后台 Web 进程
        terminateAllProcesses();
    } else {
        // 清理缓存
        m_memoryCache->prune();
        
        // 触发 JS GC
        for (auto& process : m_processes)
            process->gc();
    }
}
```

#### iOS 开发中监控内存

```swift
// 监听 WebProcess 终止
class WebViewController: UIViewController, WKNavigationDelegate {
    var webView: WKWebView!
    
    // WebProcess 因内存压力被终止时调用
    func webViewWebContentProcessDidTerminate(_ webView: WKWebView) {
        print("⚠️ WebProcess 被终止，可能是内存压力")
        
        // 重新加载
        webView.reload()
    }
}

// 主动释放内存
extension WKWebView {
    func releaseMemory() {
        // 清理 JS 内存
        evaluateJavaScript("""
            // 清理全局变量
            for (var prop in window) {
                if (window.hasOwnProperty(prop) && 
                    !['location', 'document', 'window'].includes(prop)) {
                    try { window[prop] = null; } catch(e) {}
                }
            }
        """)
        
        // 清理后退前进缓存
        // 注意：这是私有 API，仅供参考
    }
}
```

### 4.4 JS 与 Native 交互中的内存陷阱

```swift
// ❌ 内存泄漏示例
class BadViewController: UIViewController, WKScriptMessageHandler {
    var webView: WKWebView!
    
    func setupWebView() {
        let config = WKWebViewConfiguration()
        // self 被 userContentController 强引用
        // userContentController 被 configuration 强引用
        // configuration 被 webView 强引用
        // webView 被 self 强引用
        // 形成循环引用！
        config.userContentController.add(self, name: "handler")
        
        webView = WKWebView(frame: .zero, configuration: config)
    }
    
    func userContentController(_ controller: WKUserContentController,
                               didReceive message: WKScriptMessage) {
        // ...
    }
}

// ✅ 正确做法：使用弱引用包装器
class WeakScriptMessageHandler: NSObject, WKScriptMessageHandler {
    weak var delegate: WKScriptMessageHandler?
    
    init(_ delegate: WKScriptMessageHandler) {
        self.delegate = delegate
    }
    
    func userContentController(_ controller: WKUserContentController,
                               didReceive message: WKScriptMessage) {
        delegate?.userContentController(controller, didReceive: message)
    }
}

class GoodViewController: UIViewController, WKScriptMessageHandler {
    var webView: WKWebView!
    
    func setupWebView() {
        let config = WKWebViewConfiguration()
        // 使用弱引用包装器，打破循环
        config.userContentController.add(
            WeakScriptMessageHandler(self), name: "handler")
        
        webView = WKWebView(frame: .zero, configuration: config)
    }
    
    deinit {
        // 清理 message handler
        webView.configuration.userContentController
            .removeScriptMessageHandler(forName: "handler")
    }
    
    func userContentController(_ controller: WKUserContentController,
                               didReceive message: WKScriptMessage) {
        // ...
    }
}
```

---

## 5. 安全模型

### 5.1 沙盒隔离

**路径**: `Source/WebKit/WebProcess/com.apple.WebProcess.sb.in`

```
WebProcess 运行在严格的沙盒中：

┌─────────────────────────────────────────────────────┐
│                     沙盒限制                          │
├─────────────────────────────────────────────────────┤
│  ✗ 不能访问文件系统（除了特定缓存目录）                  │
│  ✗ 不能使用网络（通过 NetworkProcess 代理）             │
│  ✗ 不能访问用户数据（联系人、相册等）                    │
│  ✗ 不能执行任意系统调用                                │
├─────────────────────────────────────────────────────┤
│  ✓ 可以访问 GPU（通过限制的方式）                       │
│  ✓ 可以分配内存                                       │
│  ✓ 可以通过 IPC 与 UIProcess 通信                     │
└─────────────────────────────────────────────────────┘
```

### 5.2 同源策略 (Same-Origin Policy)

**路径**: `Source/WebCore/page/SecurityOrigin.h`

```cpp
class SecurityOrigin {
public:
    // 判断是否同源
    bool canAccess(const SecurityOrigin& other) const {
        // 协议、主机、端口都必须相同
        return m_protocol == other.m_protocol
            && m_host == other.m_host
            && m_port == other.m_port;
    }
    
    // 例外情况
    bool canRequest(const URL& url) const;
    
private:
    String m_protocol;  // "https"
    String m_host;      // "example.com"
    std::optional<uint16_t> m_port;  // 443
};
```

### 5.3 Content Security Policy (CSP)

**路径**: `Source/WebCore/page/csp/`

```cpp
// CSP 检查示例
bool ContentSecurityPolicy::allowInlineScript() const
{
    // 检查是否允许内联脚本
    return m_policy.allows(ContentSecurityPolicyDirective::Script, 
                           "'unsafe-inline'");
}

bool ContentSecurityPolicy::allowScriptFromSource(const URL& url) const
{
    // 检查脚本源是否在白名单中
    return m_policy.allowsSource(ContentSecurityPolicyDirective::Script, url);
}
```

#### iOS 开发中处理 CSP

```swift
// 有些网页的 CSP 可能阻止你注入的脚本执行
// 解决方案：使用 WKUserScript 在文档开始前注入

let bypassCSP = WKUserScript(
    source: """
    // 这个脚本在 CSP 解析之前执行
    // 所以不受 CSP 限制
    """,
    injectionTime: .atDocumentStart,
    forMainFrameOnly: false
)

// 或者使用 WKContentRuleList 修改响应头
```

### 5.4 CORS (跨域资源共享)

**路径**: `Source/WebCore/loader/CrossOriginAccessControl.cpp`

```cpp
bool passesAccessControlCheck(const ResourceResponse& response,
                              StoredCredentialsPolicy credentialsPolicy,
                              const SecurityOrigin& securityOrigin)
{
    // 检查 Access-Control-Allow-Origin 头
    String accessControlOrigin = 
        response.httpHeaderField(HTTPHeaderName::AccessControlAllowOrigin);
    
    if (accessControlOrigin == "*" && credentialsPolicy == StoredCredentialsPolicy::DoNotUse)
        return true;
    
    if (accessControlOrigin == securityOrigin.toString())
        return true;
    
    return false;
}
```

---

## 6. 实战：常见问题源码级分析

### 6.1 白屏问题

**症状**: WKWebView 突然变白，无内容显示

**原因分析**:

```cpp
// Source/WebKit/UIProcess/WebPageProxy.cpp
void WebPageProxy::webProcessTerminated(ProcessTerminationReason reason)
{
    // WebProcess 崩溃或被系统终止
    switch (reason) {
    case ProcessTerminationReason::ExceededMemoryLimit:
        // 内存超限
        break;
    case ProcessTerminationReason::Crash:
        // 代码崩溃
        break;
    case ProcessTerminationReason::RequestedByClient:
        // 主动终止
        break;
    }
    
    // 通知代理
    m_navigationClient->webViewWebContentProcessDidTerminate(*this);
}
```

**解决方案**:

```swift
class RobustWebViewController: UIViewController, WKNavigationDelegate {
    var webView: WKWebView!
    var lastURL: URL?
    
    func webViewWebContentProcessDidTerminate(_ webView: WKWebView) {
        // 记录崩溃
        print("WebProcess 终止")
        
        // 方案 1: 直接 reload
        webView.reload()
        
        // 方案 2: 重新加载上次 URL（更稳健）
        if let url = lastURL {
            webView.load(URLRequest(url: url))
        }
    }
    
    func webView(_ webView: WKWebView, 
                 didFinish navigation: WKNavigation!) {
        // 保存当前 URL
        lastURL = webView.url
    }
}
```

### 6.2 JavaScript 执行延迟

**症状**: `evaluateJavaScript` 回调延迟或不执行

**原因分析**:

```cpp
// Source/WebKit/WebProcess/WebPage/WebPage.cpp
void WebPage::runJavaScript(const String& script,
                           WebCore::RunJavaScriptParameters&&,
                           CompletionHandler<void(...)>&& completionHandler)
{
    // 如果页面正在导航，JS 可能不会执行
    if (m_isClosed || !m_mainFrame) {
        completionHandler(/* error */);
        return;
    }
    
    // 如果是 about:blank 或没有安全源
    auto* document = m_mainFrame->document();
    if (!document || document->url().isAboutBlank()) {
        // 可能有限制
    }
    
    // 实际执行
    // ...
}
```

**解决方案**:

```swift
class SafeJSEvaluator {
    weak var webView: WKWebView?
    
    func evaluateJavaScript(_ script: String, 
                           completion: @escaping (Any?, Error?) -> Void) {
        guard let webView = webView else {
            completion(nil, NSError(domain: "WebView", code: -1))
            return
        }
        
        // 等待页面加载完成
        guard !webView.isLoading else {
            // 延迟执行
            DispatchQueue.main.asyncAfter(deadline: .now() + 0.1) {
                self.evaluateJavaScript(script, completion: completion)
            }
            return
        }
        
        // 确保不是 about:blank
        guard let url = webView.url, !url.absoluteString.hasPrefix("about:") else {
            completion(nil, NSError(domain: "WebView", code: -2))
            return
        }
        
        webView.evaluateJavaScript(script, completionHandler: completion)
    }
}
```

### 6.3 Cookie 同步问题

**症状**: 原生 App 设置的 Cookie 在 WKWebView 中无法获取

**原因分析**:

```cpp
// WKWebView 使用独立的 Cookie 存储
// Source/WebKit/UIProcess/WebsiteData/WebsiteDataStore.cpp

// Cookie 存储在 WKWebsiteDataStore 中
// 与 HTTPCookieStorage.shared 是分开的！
```

**解决方案**:

```swift
// 同步 Cookie 到 WKWebView
func syncCookies(to webView: WKWebView) async {
    guard let cookies = HTTPCookieStorage.shared.cookies else { return }
    
    let cookieStore = webView.configuration.websiteDataStore.httpCookieStore
    
    for cookie in cookies {
        await cookieStore.setCookie(cookie)
    }
}

// 在加载前同步
func loadURL(_ url: URL) {
    Task {
        await syncCookies(to: webView)
        webView.load(URLRequest(url: url))
    }
}

// 从 WKWebView 同步到 App
func syncCookiesFromWebView() {
    webView.configuration.websiteDataStore.httpCookieStore.getAllCookies { cookies in
        for cookie in cookies {
            HTTPCookieStorage.shared.setCookie(cookie)
        }
    }
}
```

### 6.4 滚动性能问题

**症状**: 滚动卡顿，不流畅

**原因分析**:

```cpp
// Source/WebCore/page/scrolling/ScrollingTree.cpp
void ScrollingTree::handleWheelEvent(const PlatformWheelEvent& event)
{
    // 滚动处理在 Scrolling Thread
    // 如果需要等待主线程（如 JS 事件监听），会卡顿
    
    if (m_eventTrackingRegions.contains(event.position())) {
        // 需要同步到主线程，可能造成延迟
        synchronouslyWaitForMainThread();
    }
}
```

**解决方案**:

```swift
// 优化滚动性能的 JS
let optimizeScrollScript = WKUserScript(
    source: """
    // 使用 passive 事件监听，避免阻塞滚动
    document.addEventListener('touchstart', handler, { passive: true });
    document.addEventListener('touchmove', handler, { passive: true });
    
    // 或者使用 CSS
    // touch-action: manipulation; 
    """,
    injectionTime: .atDocumentEnd,
    forMainFrameOnly: true
)
```

---

## 总结

通过这份深入分析，你应该能够：

1. **理解 WKWebView 的完整架构** - 知道 UI 层、代理层、渲染层的关系
2. **优化 JS 交互性能** - 减少 IPC 开销，批量处理
3. **处理内存问题** - 避免循环引用，正确响应内存压力
4. **理解安全模型** - 了解沙盒限制和同源策略
5. **调试常见问题** - 从源码层面理解问题根源

WebKit 的源码是学习大型系统设计的绝佳材料。希望这份文档能帮助你在 iOS 开发中更好地使用 WKWebView！
