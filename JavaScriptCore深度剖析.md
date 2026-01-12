# JavaScriptCore 深度剖析

> 本文档深入分析 WebKit 中的 JavaScript 引擎 - JavaScriptCore (JSC)，帮助 iOS 开发者理解 JS 执行的底层原理。

## 目录

1. [JavaScriptCore 概述](#1-javascriptcore-概述)
2. [解析与字节码生成](#2-解析与字节码生成)
3. [执行引擎分层架构](#3-执行引擎分层架构)
4. [内存管理与垃圾回收](#4-内存管理与垃圾回收)
5. [对象模型与属性访问](#5-对象模型与属性访问)
6. [iOS 中的 JavaScriptCore 实战](#6-ios-中的-javascriptcore-实战)

---

## 1. JavaScriptCore 概述

### 1.1 JSC 在 WebKit 中的位置

```
WebKit 整体架构
═══════════════════════════════════════════════════════

┌─────────────────────────────────────────────────────┐
│                    WebCore                           │
│  (HTML/CSS/DOM/渲染)                                 │
│                                                     │
│  ┌───────────────────────────────────────────────┐  │
│  │              Bindings (绑定层)                  │  │
│  │  将 DOM 对象暴露给 JavaScript                   │  │
│  └───────────────────────────────────────────────┘  │
│                        │                            │
└────────────────────────│────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────┐
│               JavaScriptCore (JSC)                   │
├─────────────────────────────────────────────────────┤
│  • JavaScript 解析器 (Parser)                       │
│  • 字节码编译器 (Bytecode Compiler)                  │
│  • 解释器 (LLInt)                                   │
│  • JIT 编译器 (Baseline, DFG, FTL)                  │
│  • 垃圾回收器 (Garbage Collector)                    │
│  • 运行时库 (Runtime)                               │
└─────────────────────────────────────────────────────┘
```

### 1.2 核心目录结构

```
Source/JavaScriptCore/
├── API/            # 公开 C/ObjC API (iOS 可用)
├── parser/         # JS 解析器
├── bytecompiler/   # 字节码生成器
├── bytecode/       # 字节码定义和操作
├── interpreter/    # LLInt 解释器
├── llint/          # Low Level Interpreter 实现
├── jit/            # Baseline JIT
├── dfg/            # Data Flow Graph 优化编译器
├── ftl/            # Faster Than Light 高级优化
├── b3/             # B3 中间表示
├── runtime/        # 运行时对象和函数
├── heap/           # 垃圾回收
└── wasm/           # WebAssembly 支持
```

### 1.3 执行流水线

```
JavaScript 源代码: "function add(a, b) { return a + b; }"
                                │
                                ▼
┌─────────────────────────────────────────────────────┐
│                 1. 词法分析 (Lexer)                   │
│  源代码 → Token 流                                   │
│  "function" → TOKEN_FUNCTION                        │
│  "add" → TOKEN_IDENT                                │
└─────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────┐
│                 2. 语法分析 (Parser)                  │
│  Token 流 → 抽象语法树 (AST)                         │
│  FunctionDeclaration                                │
│    ├── name: "add"                                  │
│    ├── params: ["a", "b"]                           │
│    └── body: ReturnStatement(BinaryExpression)      │
└─────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────┐
│              3. 字节码生成 (BytecodeGenerator)        │
│  AST → 字节码                                        │
│  op_enter                                           │
│  op_get_argument, r0, 0 (a)                         │
│  op_get_argument, r1, 1 (b)                         │
│  op_add, r2, r0, r1                                 │
│  op_ret, r2                                         │
└─────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────┐
│              4. 执行 (多层优化)                       │
│                                                     │
│  首次执行 → LLInt (解释执行)                          │
│       │                                             │
│  热点检测 → Baseline JIT (机器码)                    │
│       │                                             │
│  更热点   → DFG JIT (类型推测优化)                    │
│       │                                             │
│  最热点   → FTL (LLVM 后端优化)                       │
└─────────────────────────────────────────────────────┘
```

---

## 2. 解析与字节码生成

### 2.1 Lexer - 词法分析器

**路径**: `Source/JavaScriptCore/parser/Lexer.h`

```cpp
template <typename T>
class Lexer {
public:
    // 获取下一个 Token
    JSToken lex(JSToken* token) {
        // 跳过空白
        skipWhitespace();
        
        // 识别 Token 类型
        switch (m_current) {
        case '(':
            return OPENPAREN;
        case ')':
            return CLOSEPAREN;
        case '{':
            return OPENBRACE;
        case '}':
            return CLOSEBRACE;
        // ... 更多符号
        
        default:
            // 标识符或关键字
            if (isIdentifierStart(m_current))
                return lexIdentifier();
            // 数字
            if (isDigit(m_current))
                return lexNumber();
            // 字符串
            if (m_current == '"' || m_current == '\'')
                return lexString();
        }
    }
    
private:
    const T* m_code;      // 源代码
    const T* m_current;   // 当前位置
};
```

### 2.2 Parser - 语法分析器

**路径**: `Source/JavaScriptCore/parser/Parser.h`

```cpp
template <class LexerType>
class Parser {
public:
    // 解析程序
    std::unique_ptr<ProgramNode> parse(ParserError& error) {
        // 解析语句列表
        auto* sourceElements = parseSourceElements();
        return std::make_unique<ProgramNode>(sourceElements);
    }
    
private:
    // 解析函数声明
    FunctionNode* parseFunctionDeclaration() {
        // function name(params) { body }
        consume(FUNCTION);
        auto name = parseFunctionName();
        auto params = parseFormalParameters();
        auto body = parseFunctionBody();
        return new FunctionNode(name, params, body);
    }
    
    // 解析表达式
    ExpressionNode* parseExpression() {
        // 处理运算符优先级
        return parseAssignmentExpression();
    }
    
    // 解析二元表达式 (加减乘除等)
    ExpressionNode* parseBinaryExpression() {
        auto* left = parseUnaryExpression();
        
        while (isBinaryOperator(m_token)) {
            auto op = m_token;
            next();
            auto* right = parseUnaryExpression();
            left = new BinaryOpNode(op, left, right);
        }
        
        return left;
    }
};
```

### 2.3 BytecodeGenerator - 字节码生成

**路径**: `Source/JavaScriptCore/bytecompiler/BytecodeGenerator.h`

```cpp
class BytecodeGenerator {
public:
    // 为函数生成字节码
    void generate(FunctionNode* function) {
        // 序言
        emitOpcode(op_enter);
        
        // 处理参数
        for (auto& param : function->parameters())
            emitArgument(param);
        
        // 处理函数体
        emitNode(function->body());
        
        // 尾声
        emitOpcode(op_end);
    }
    
private:
    // 生成表达式
    RegisterID* emitNode(ExpressionNode* node) {
        switch (node->type()) {
        case BinaryOp:
            return emitBinaryOp(static_cast<BinaryOpNode*>(node));
        case Call:
            return emitCall(static_cast<CallNode*>(node));
        // ...
        }
    }
    
    // 生成二元运算
    RegisterID* emitBinaryOp(BinaryOpNode* node) {
        auto* left = emitNode(node->left());
        auto* right = emitNode(node->right());
        auto* result = newTemporary();
        
        switch (node->op()) {
        case OpPlus:
            emitOpcode(op_add, result, left, right);
            break;
        case OpMinus:
            emitOpcode(op_sub, result, left, right);
            break;
        // ...
        }
        
        return result;
    }
};
```

### 2.4 字节码格式

**路径**: `Source/JavaScriptCore/bytecode/BytecodeList.rb`

```ruby
# 字节码定义（使用 Ruby DSL）
op :add,
    args: {
        dst: VirtualRegister,
        lhs: VirtualRegister,
        rhs: VirtualRegister
    },
    metadata: { arithProfile: ArithProfile }

op :get_by_id,
    args: {
        dst: VirtualRegister,
        base: VirtualRegister,
        property: Identifier
    },
    metadata: { 
        modeMetadata: GetByIdModeMetadata,
        profile: ValueProfile
    }

op :call,
    args: {
        dst: VirtualRegister,
        callee: VirtualRegister,
        argc: unsigned,
        argv: VirtualRegister
    }
```

---

## 3. 执行引擎分层架构

### 3.1 LLInt - 低级解释器

**路径**: `Source/JavaScriptCore/llint/`

LLInt 是 JSC 的第一层执行方式，特点是启动快但执行慢。

```cpp
// LLInt 是用汇编宏定义的
// Source/JavaScriptCore/llint/LowLevelInterpreter.asm

// 加法指令实现（伪代码）
macro llintOp_add(dst, src1, src2)
    // 加载操作数
    loadConstantOrVariable(src1, temp1)
    loadConstantOrVariable(src2, temp2)
    
    // 快速路径：两个都是整数
    if (isInt32(temp1) && isInt32(temp2)) {
        result = temp1 + temp2
        if (!overflow) {
            store(dst, result)
            dispatch(next_instruction)
        }
    }
    
    // 慢速路径：调用 C++ 函数
    callSlowPath(operationAdd, dst, temp1, temp2)
end
```

### 3.2 Baseline JIT

**路径**: `Source/JavaScriptCore/jit/`

当函数被多次调用时，触发 Baseline JIT 编译。

```cpp
// Source/JavaScriptCore/jit/JIT.cpp
void JIT::compileOpAdd(Instruction* instruction)
{
    // 加载操作数到寄存器
    emitGetVirtualRegister(instruction->src1(), regT0);
    emitGetVirtualRegister(instruction->src2(), regT1);
    
    // 检查是否都是整数（快速路径）
    m_assembler.test32(regT0, tagMask);
    auto notInt1 = m_assembler.branchNotEqual();
    m_assembler.test32(regT1, tagMask);
    auto notInt2 = m_assembler.branchNotEqual();
    
    // 整数加法
    m_assembler.add32(regT1, regT0);
    auto overflow = m_assembler.branchOverflow();
    
    // 存储结果
    emitPutVirtualRegister(instruction->dst(), regT0);
    
    // 慢速路径
    notInt1.link(this);
    notInt2.link(this);
    overflow.link(this);
    callOperation(operationAdd, instruction->dst(), regT0, regT1);
}
```

### 3.3 DFG JIT - 推测优化

**路径**: `Source/JavaScriptCore/dfg/`

DFG (Data Flow Graph) 进行类型推测和优化。

```cpp
// Source/JavaScriptCore/dfg/DFGSpeculativeJIT.cpp

// DFG 会分析历史执行数据，推测类型
void SpeculativeJIT::compileAdd(Node* node)
{
    // 获取类型推测结果
    SpeculatedType leftType = m_state.forNode(node->child1()).type();
    SpeculatedType rightType = m_state.forNode(node->child2()).type();
    
    // 如果都推测为整数
    if (leftType == SpecInt32Only && rightType == SpecInt32Only) {
        // 生成优化的整数加法
        GPRReg left = operand(node->child1());
        GPRReg right = operand(node->child2());
        GPRReg result = allocate();
        
        m_jit.add32(left, right, result);
        
        // 检查溢出（如果溢出，deoptimize）
        speculationCheck(Overflow, result);
        
        return;
    }
    
    // 否则回退到通用加法
    callOperation(operationAdd, /* ... */);
}
```

### 3.4 FTL - 最高级优化

**路径**: `Source/JavaScriptCore/ftl/`

FTL (Faster Than Light) 使用 B3 后端进行最激进的优化。

```cpp
// Source/JavaScriptCore/ftl/FTLLowerDFGToB3.cpp

// FTL 将 DFG 图转换为 B3 中间表示
LValue FTLCompiler::compileAdd(Node* node)
{
    LValue left = lowInt32(node->child1());
    LValue right = lowInt32(node->child2());
    
    // 使用 B3 生成优化的机器码
    LValue result = m_out.add(left, right);
    
    // 溢出检查
    LValue overflow = m_out.addWithOverflow32(left, right);
    speculate(Overflow, overflow);
    
    return result;
}
```

### 3.5 热点检测与编译触发

**路径**: `Source/JavaScriptCore/bytecode/ExecutionCounter.h`

```cpp
class ExecutionCounter {
public:
    // 检查是否应该触发 JIT 编译
    bool checkIfThresholdCrossedAndSet(CodeBlock* codeBlock)
    {
        // 执行计数器
        m_counter += 1;
        
        // 检查阈值
        if (m_counter >= Options::thresholdForJIT()) {
            // 触发 Baseline JIT
            triggerBaselineCompilation(codeBlock);
            return true;
        }
        
        if (m_counter >= Options::thresholdForDFG()) {
            // 触发 DFG 编译
            triggerDFGCompilation(codeBlock);
            return true;
        }
        
        return false;
    }
};
```

### 3.6 OSR (On-Stack Replacement)

当正在执行的代码需要切换到优化版本时：

```cpp
// 从 LLInt 升级到 Baseline JIT
void performOSREntryToBaseline(CodeBlock* codeBlock, unsigned bytecodeIndex)
{
    // 1. 保存当前执行状态
    saveCallFrame();
    
    // 2. 编译 Baseline 代码
    compileBaseline(codeBlock);
    
    // 3. 找到对应的机器码位置
    void* entryPoint = codeBlock->jitCodeFor(bytecodeIndex);
    
    // 4. 跳转到优化代码继续执行
    jumpToJITCode(entryPoint);
}

// 从优化代码回退到解释器（当推测失败时）
void performOSRExit(CodeBlock* codeBlock, ExitKind exitKind)
{
    // 1. 记录退出原因
    recordExitReason(codeBlock, exitKind);
    
    // 2. 恢复解释器状态
    restoreInterpreterState();
    
    // 3. 重新开始解释执行
    enterLLInt(codeBlock);
}
```

---

## 4. 内存管理与垃圾回收

### 4.1 JSC 堆架构

**路径**: `Source/JavaScriptCore/heap/Heap.h`

```cpp
class Heap {
public:
    // 内存分配
    void* allocate(size_t);
    
    // 垃圾回收
    void collect(CollectionScope);
    
    // 写屏障（记住代际引用）
    void writeBarrier(JSCell* from, JSCell* to);
    
private:
    // 不同大小对象的分配器
    MarkedSpace m_markedSpace;
    
    // 大对象分配器
    LargeAllocation m_largeAllocator;
    
    // GC 线程
    std::unique_ptr<GCThread> m_gcThread;
    
    // 标记位图
    BlockDirectory m_blockDirectory;
};
```

### 4.2 对象布局

```cpp
// JSC 中所有堆对象都继承自 JSCell
class JSCell {
    // 前 8 字节是结构信息
    StructureID m_structureID;    // 4 bytes - 对象"形状"
    IndexingType m_indexingType;  // 1 byte
    JSType m_type;                // 1 byte
    TypeInfo m_typeInfo;          // 2 bytes
    
    // 之后是对象特定数据
};

// 例如 JSObject 的布局
class JSObject : public JSCell {
    // Butterfly 指针 - 指向属性存储区
    Butterfly* m_butterfly;
};

// Butterfly 结构（存储对象属性和数组元素）
//
//  ┌─────────────────────────────────────────┐
//  │        Named Properties (向左增长)       │
//  │  prop3 | prop2 | prop1                  │
//  └─────────────────────────────────────────┘
//                    ↑
//               Butterfly 指针
//                    ↓
//  ┌─────────────────────────────────────────┐
//  │  publicLength  |  vectorLength          │
//  │  array[0] | array[1] | array[2] ...     │
//  │        Array Elements (向右增长)         │
//  └─────────────────────────────────────────┘
```

### 4.3 标记-清除算法

**路径**: `Source/JavaScriptCore/heap/SlotVisitor.cpp`

```cpp
// 标记阶段
void SlotVisitor::visit(JSCell* cell)
{
    // 检查是否已标记
    if (isMarked(cell))
        return;
    
    // 标记当前对象
    setMarked(cell);
    
    // 遍历引用的对象
    cell->visitChildren(*this);
}

// 清除阶段
void Heap::sweep()
{
    // 遍历所有内存块
    for (auto& block : m_markedSpace.blocks()) {
        // 检查每个单元格
        for (auto& cell : block.cells()) {
            if (!isMarked(cell)) {
                // 未标记的对象是垃圾
                free(cell);
            }
        }
        // 清除标记位
        block.clearMarkBits();
    }
}
```

### 4.4 增量 GC 与并发 GC

```cpp
// JSC 支持增量和并发 GC 以减少停顿

// 增量标记：将标记工作分成多个小步骤
void Heap::incrementalMark(Seconds deadline)
{
    while (hasMoreWork() && MonotonicTime::now() < deadline) {
        // 标记一小批对象
        markSomeCells();
    }
}

// 并发标记：在后台线程进行标记
void GCThread::run()
{
    while (true) {
        // 等待 GC 开始信号
        waitForGCStart();
        
        // 在后台进行标记
        concurrentMark();
        
        // 通知主线程标记完成
        signalMarkingComplete();
    }
}
```

### 4.5 写屏障

```cpp
// 当修改对象引用时，需要通知 GC
void writeBarrier(JSCell* from, JSCell* to)
{
    // 如果 from 在老年代，to 在新生代
    // 需要记录这个引用，否则 GC 会错误地回收 to
    if (isOldGeneration(from) && isYoungGeneration(to)) {
        m_rememberedSet.add(from);
    }
}

// 使用写屏障的例子
void JSObject::putDirect(PropertyName propertyName, JSValue value)
{
    // 设置属性值
    m_butterfly->setProperty(offset, value);
    
    // 写屏障
    if (value.isCell())
        heap()->writeBarrier(this, value.asCell());
}
```

---

## 5. 对象模型与属性访问

### 5.1 Structure - 隐藏类

**路径**: `Source/JavaScriptCore/runtime/Structure.h`

JSC 使用 "Structure"（类似 V8 的 Hidden Class）来优化属性访问。

```cpp
class Structure {
public:
    // 属性查找
    PropertyOffset get(PropertyName name) const {
        // 在结构中查找属性偏移
        auto* entry = m_propertyTable.get(name);
        return entry ? entry->offset : invalidOffset;
    }
    
    // 添加属性时的转换
    Structure* addProperty(PropertyName name, unsigned attributes) {
        // 检查是否已有转换
        if (auto* cached = m_transitionTable.get(name))
            return cached;
        
        // 创建新结构
        auto* newStructure = Structure::create(this, name, attributes);
        
        // 缓存转换
        m_transitionTable.add(name, newStructure);
        
        return newStructure;
    }
    
private:
    StructureID m_id;
    Structure* m_previous;                      // 转换链
    PropertyTable m_propertyTable;              // 属性表
    StructureTransitionTable m_transitionTable; // 转换缓存
};
```

### 5.2 属性访问优化 - Inline Cache

**路径**: `Source/JavaScriptCore/bytecode/StructureStubInfo.h`

```cpp
// 当执行 obj.prop 时，JSC 使用内联缓存加速
class StructureStubInfo {
public:
    // 单态缓存（最常见情况）
    Structure* m_cachedStructure;
    PropertyOffset m_cachedOffset;
    
    // 检查缓存
    bool checkStructure(JSCell* cell) {
        return cell->structure() == m_cachedStructure;
    }
    
    JSValue getProperty(JSObject* obj) {
        if (checkStructure(obj)) {
            // 缓存命中，直接用偏移读取
            return obj->getDirect(m_cachedOffset);
        }
        // 缓存未命中，走慢速路径
        return slowGetProperty(obj);
    }
};
```

### 5.3 实际的属性读取流程

```javascript
// JavaScript 代码
function getX(obj) {
    return obj.x;
}
```

```cpp
// 第一次执行 - 未缓存
JSValue firstCall(JSObject* obj) {
    // 1. 获取对象的 Structure
    Structure* structure = obj->structure();
    
    // 2. 在 Structure 中查找属性 "x"
    PropertyOffset offset = structure->get("x");
    
    // 3. 如果找到，读取值
    if (offset != invalidOffset) {
        JSValue value = obj->getDirect(offset);
        
        // 4. 记录到内联缓存
        stubInfo.m_cachedStructure = structure;
        stubInfo.m_cachedOffset = offset;
        
        return value;
    }
    
    // 5. 原型链查找...
}

// 后续执行 - 使用缓存
JSValue cachedCall(JSObject* obj) {
    // 直接检查结构
    if (obj->structureID() == stubInfo.m_cachedStructure->id()) {
        // 缓存命中！直接读取
        return obj->getDirect(stubInfo.m_cachedOffset);
    }
    // 缓存未命中，更新缓存或转为多态
}
```

### 5.4 多态内联缓存

当同一属性访问遇到不同结构的对象时：

```cpp
class PolymorphicAccess {
    // 最多 4 种不同结构的缓存
    struct CacheEntry {
        StructureID structureID;
        PropertyOffset offset;
    };
    
    CacheEntry m_entries[4];
    unsigned m_count;
    
    JSValue get(JSObject* obj) {
        StructureID id = obj->structureID();
        
        // 检查所有缓存项
        for (unsigned i = 0; i < m_count; i++) {
            if (m_entries[i].structureID == id) {
                return obj->getDirect(m_entries[i].offset);
            }
        }
        
        // 如果超过 4 种结构，转为 megamorphic
        if (m_count >= 4) {
            return megamorphicGet(obj);
        }
        
        // 添加新缓存项
        return addCacheEntry(obj);
    }
};
```

---

## 6. iOS 中的 JavaScriptCore 实战

### 6.1 基本使用

```swift
import JavaScriptCore

// 创建 JS 上下文
let context = JSContext()!

// 执行 JS 代码
context.evaluateScript("var x = 1 + 2")
let x = context.objectForKeyedSubscript("x")
print(x?.toInt32() ?? 0)  // 3

// 定义 JS 函数
context.evaluateScript("""
    function fibonacci(n) {
        if (n <= 1) return n;
        return fibonacci(n - 1) + fibonacci(n - 2);
    }
""")

// 调用 JS 函数
let fibonacci = context.objectForKeyedSubscript("fibonacci")
let result = fibonacci?.call(withArguments: [10])
print(result?.toInt32() ?? 0)  // 55
```

### 6.2 Swift 与 JS 互操作

```swift
// 暴露 Swift 函数给 JS
let logFunc: @convention(block) (String) -> Void = { message in
    print("[JS Log]: \(message)")
}
context.setObject(logFunc, forKeyedSubscript: "nativeLog" as NSString)

// JS 中调用
context.evaluateScript("nativeLog('Hello from JavaScript!')")

// 暴露 Swift 对象给 JS
@objc protocol PersonJSExport: JSExport {
    var name: String { get set }
    var age: Int { get set }
    func greet() -> String
}

class Person: NSObject, PersonJSExport {
    dynamic var name: String = ""
    dynamic var age: Int = 0
    
    func greet() -> String {
        return "Hello, I'm \(name), \(age) years old"
    }
}

let person = Person()
person.name = "Alice"
person.age = 30

context.setObject(person, forKeyedSubscript: "person" as NSString)

let greeting = context.evaluateScript("person.greet()")
print(greeting?.toString() ?? "")  // "Hello, I'm Alice, 30 years old"

// JS 可以修改 Swift 对象
context.evaluateScript("person.name = 'Bob'")
print(person.name)  // "Bob"
```

### 6.3 异常处理

```swift
// 设置异常处理器
context.exceptionHandler = { context, exception in
    print("JS Error: \(exception?.toString() ?? "unknown")")
}

// 这会触发异常
context.evaluateScript("throw new Error('Something went wrong')")

// 或者检查返回值
let result = context.evaluateScript("undefinedFunction()")
if context.exception != nil {
    print("Error occurred: \(context.exception!)")
    context.exception = nil  // 清除异常
}
```

### 6.4 内存管理

```swift
// ⚠️ 重要：避免循环引用

// ❌ 这会导致内存泄漏
class BadExample {
    var context: JSContext!
    
    func setup() {
        context = JSContext()
        
        // self 被 block 捕获，形成循环：
        // self -> context -> block -> self
        let callback: @convention(block) () -> Void = {
            self.doSomething()
        }
        context.setObject(callback, forKeyedSubscript: "callback" as NSString)
    }
    
    func doSomething() { }
}

// ✅ 正确做法：使用 weak self
class GoodExample {
    var context: JSContext!
    
    func setup() {
        context = JSContext()
        
        let callback: @convention(block) () -> Void = { [weak self] in
            self?.doSomething()
        }
        context.setObject(callback, forKeyedSubscript: "callback" as NSString)
    }
    
    func doSomething() { }
}

// ✅ 使用 JSManagedValue 管理 JS 对象生命周期
class ManagedExample {
    var context: JSContext!
    var managedCallback: JSManagedValue?
    
    func setup() {
        context = JSContext()
        
        // 创建 JS 函数
        let jsFunc = context.evaluateScript("(function() { return 42; })")
        
        // 使用 JSManagedValue 包装，防止被 GC 回收
        managedCallback = JSManagedValue(value: jsFunc, andOwner: self)
        context.virtualMachine.addManagedReference(managedCallback, withOwner: self)
    }
    
    func callCallback() -> Int32 {
        return managedCallback?.value?.call(withArguments: [])?.toInt32() ?? 0
    }
    
    deinit {
        if let managed = managedCallback {
            context.virtualMachine.removeManagedReference(managed, withOwner: self)
        }
    }
}
```

### 6.5 性能优化

```swift
// 1. 缓存频繁使用的 JS 函数引用
class OptimizedExample {
    var context: JSContext!
    var processFunc: JSValue?  // 缓存函数引用
    
    func setup() {
        context = JSContext()
        context.evaluateScript("""
            function process(data) {
                return data.map(x => x * 2);
            }
        """)
        
        // 一次查找，多次使用
        processFunc = context.objectForKeyedSubscript("process")
    }
    
    func process(data: [Int]) -> [Int]? {
        let jsArray = JSValue(object: data, in: context)
        let result = processFunc?.call(withArguments: [jsArray!])
        return result?.toArray() as? [Int]
    }
}

// 2. 批量处理数据，减少跨边界调用
func processBatch(items: [String]) {
    // ❌ 低效：每次调用都跨语言边界
    for item in items {
        context.evaluateScript("process('\(item)')")
    }
    
    // ✅ 高效：一次性传递所有数据
    let json = try! JSONSerialization.data(withJSONObject: items)
    let jsonString = String(data: json, encoding: .utf8)!
    context.evaluateScript("processAll(\(jsonString))")
}

// 3. 使用 Typed Arrays 传递二进制数据
func processImage(data: Data) {
    // 直接传递 ArrayBuffer，避免数据复制
    let uint8Array = JSValue(object: [UInt8](data), in: context)
    context.setObject(uint8Array, forKeyedSubscript: "imageData" as NSString)
    context.evaluateScript("processImageData(imageData)")
}
```

### 6.6 调试技巧

```swift
// 1. 启用调试日志
context.evaluateScript("""
    console.log = function(...args) {
        _nativeLog(args.map(String).join(' '));
    };
    console.error = function(...args) {
        _nativeError(args.map(String).join(' '));
    };
""")

let logFunc: @convention(block) (String) -> Void = { message in
    print("📝 [JS]: \(message)")
}
let errorFunc: @convention(block) (String) -> Void = { message in
    print("❌ [JS Error]: \(message)")
}
context.setObject(logFunc, forKeyedSubscript: "_nativeLog" as NSString)
context.setObject(errorFunc, forKeyedSubscript: "_nativeError" as NSString)

// 2. 性能分析
let startTime = CFAbsoluteTimeGetCurrent()
context.evaluateScript(script)
let elapsed = CFAbsoluteTimeGetCurrent() - startTime
print("JS execution time: \(elapsed * 1000)ms")

// 3. 内存监控
func checkJSMemory() {
    let vm = context.virtualMachine!
    // 注意：这是私有 API，仅供调试
    if let heap = vm.value(forKey: "heap") as? NSObject {
        print("JS Heap: \(heap)")
    }
}
```

---

## 总结

通过这份 JavaScriptCore 深度剖析，你应该理解了：

1. **JS 代码如何被解析和编译** - 从源码到字节码的完整流程
2. **多层执行引擎如何协作** - LLInt、Baseline、DFG、FTL 的分工
3. **内存是如何管理的** - GC 算法、写屏障、增量/并发收集
4. **属性访问如何优化** - Structure、内联缓存、多态处理
5. **如何在 iOS 中高效使用 JSC** - 最佳实践和性能优化

这些知识将帮助你：
- 写出更高性能的 JS 代码
- 正确处理 Swift/JS 交互
- 避免内存泄漏
- 调试复杂问题

JavaScriptCore 是一个极其精密的系统，希望这份文档能帮助你更好地理解和使用它！
