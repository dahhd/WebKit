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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "ASTStringDumper.h"

#include "AST.h"
#include <wtf/DataLog.h>
#include <wtf/EnumTraits.h>
#include <wtf/SetForScope.h>

namespace WGSL::AST {

struct Indent {
    Indent(StringDumper& dumper)
        : m_scope(dumper.m_indent, dumper.m_indent + "    ")
    { }
    SetForScope<String> m_scope;
};

static Indent bumpIndent(StringDumper& dumper)
{
    return Indent(dumper);
}

template<typename T, typename J>
void StringDumper::visitVector(T& nodes, J joiner)
{
    if (nodes.isEmpty())
        return;

    visit(nodes[0]);
    for (size_t n = 1; n < nodes.size(); ++n) {
        m_out.print(joiner);
        visit(nodes[n]);
    }
}

String StringDumper::toString()
{
    return m_out.toString();
}

// Shader Module
void StringDumper::visit(ShaderModule& shaderModule)
{
    for (auto& directive : shaderModule.directives())
        visit(directive);
    if (!shaderModule.directives().isEmpty())
        m_out.print("\n\n");

    for (auto& structure : shaderModule.structs())
        visit(structure);
    if (!shaderModule.structs().isEmpty())
        m_out.printf("\n\n");

    for (auto& variable : shaderModule.globalVars())
        visit(variable);
    if (!shaderModule.globalVars().isEmpty())
        m_out.printf("\n\n");

    for (auto& function : shaderModule.functions())
        visit(function);
    if (!shaderModule.functions().isEmpty())
        m_out.printf("\n\n");
}

void StringDumper::visit(GlobalDirective& directive)
{
    m_out.print(m_indent, "enable ", directive.name(), ";");
}

// Attribute
void StringDumper::visit(BindingAttribute& binding)
{
    m_out.print("@binding(", binding.binding(), ")");
}

void StringDumper::visit(BuiltinAttribute& builtin)
{
    m_out.print("@builtin(", builtin.name(), ")");
}

void StringDumper::visit(GroupAttribute& group)
{
    m_out.print("@group(", group.group(), ")");
}

void StringDumper::visit(LocationAttribute& location)
{
    m_out.print("@location(", location.location(), ")");
}

void StringDumper::visit(StageAttribute& stage)
{
    switch (stage.stage()) {
    case StageAttribute::Stage::Compute:
        m_out.print("@compute");
        break;
    case StageAttribute::Stage::Fragment:
        m_out.print("@fragment");
        break;
    case StageAttribute::Stage::Vertex:
        m_out.print("@vertex");
        break;
    }
}

// Declaration
void StringDumper::visit(FunctionDecl& function)
{
    m_out.print(m_indent);
    if (!function.attributes().isEmpty()) {
        visitVector(function.attributes(), " ");
        m_out.print("\n", m_indent);
    }
    m_out.print("fn ", function.name(), "(");
    if (!function.parameters().isEmpty()) {
        m_out.print("\n");
        {
            auto indent = bumpIndent(*this);
            visitVector(function.parameters(), "\n");
        }
        m_out.print("\n", m_indent);
    }
    m_out.print(")");
    if (function.maybeReturnType()) {
        m_out.print(" -> ");
        visitVector(function.returnAttributes(), " ");
        m_out.print(" ");
        visit(*function.maybeReturnType());
    }
    m_out.print("\n", m_indent);
    visit(function.body());
}

void StringDumper::visit(StructDecl& structure)
{
    m_out.print(m_indent);
    if (!structure.attributes().isEmpty()) {
        visitVector(structure.attributes(), " ");
        m_out.print("\n", m_indent);
    }
    m_out.print("struct ", structure.name(), " {");
    if (!structure.members().isEmpty()) {
        m_out.print("\n");
        {
            auto indent = bumpIndent(*this);
            visitVector(structure.members(), ",\n");
        }
        m_out.print("\n", m_indent);
    }
    m_out.print("}\n");
}

void StringDumper::visit(VariableDecl& variable)
{
    if (!variable.attributes().isEmpty()) {
        visitVector(variable.attributes(), " ");
        m_out.print(" ");
    }
    m_out.print("var");
    if (variable.maybeQualifier())
        visit(*variable.maybeQualifier());
    m_out.print(" ", variable.name());
    if (variable.maybeTypeDecl()) {
        m_out.print(": ");
        visit(*variable.maybeTypeDecl());
    }
    if (variable.maybeInitializer()) {
        m_out.print(" = ");
        visit(*variable.maybeInitializer());
    }
    m_out.print(";");
}

// Expression
void StringDumper::visit(AbstractFloatLiteral& literal)
{
    m_out.print(literal.value());
}

void StringDumper::visit(AbstractIntLiteral& literal)
{
    m_out.print(literal.value());
}

void StringDumper::visit(ArrayAccess& arrayAccess)
{
    visit(arrayAccess.base());
    m_out.print("[");
    visit(arrayAccess.index());
    m_out.print("]");
}

void StringDumper::visit(BoolLiteral& literal)
{
    m_out.print(literal.value() ? "true": "false");
}

void StringDumper::visit(CallableExpression& expression)
{
    visit(expression.target());
    m_out.print("(");
    if (!expression.arguments().isEmpty()) {
        auto indent = bumpIndent(*this);
        visitVector(expression.arguments(), ", ");
    }
    m_out.print(")");
}

void StringDumper::visit(Float32Literal& literal)
{
    m_out.print(literal.value(), "f");
}

void StringDumper::visit(IdentifierExpression& identifier)
{
    m_out.print(identifier.identifier());
}

void StringDumper::visit(Int32Literal& literal)
{
    m_out.print(literal.value(), "i");
}

void StringDumper::visit(StructureAccess& structureAccess)
{
    visit(structureAccess.base());
    m_out.print(".", structureAccess.fieldName());
}

void StringDumper::visit(Uint32Literal& literal)
{
    m_out.print(literal.value(), "u");
}

void StringDumper::visit(UnaryExpression& expression)
{
    constexpr ASCIILiteral unaryOperator[] = { "-"_s };
    auto op = WTF::enumToUnderlyingType(expression.operation());
    m_out.print(unaryOperator[op]);
    visit(expression.expression());
}

void StringDumper::visit(BinaryExpression& expression)
{
    constexpr ASCIILiteral binaryOperator[] = { "+"_s, "*"_s };
    auto op = WTF::enumToUnderlyingType(expression.operation());
    visit(expression.lhs());
    m_out.print(" ", binaryOperator[op], " ");
    visit(expression.rhs());
}

void StringDumper::visit(PointerDereference& pointerDereference)
{
    m_out.print("(*");
    visit(pointerDereference.target());
    m_out.print(")");
}

// Statement
void StringDumper::visit(AssignmentStatement& statement)
{
    m_out.print(m_indent);
    if (statement.maybeLhs())
        visit(*statement.maybeLhs());
    else
        m_out.print("_");
    m_out.print(" = ");
    visit(statement.rhs());
    m_out.print(";");
}

void StringDumper::visit(CompoundStatement& block)
{
    m_out.print(m_indent, "{");
    if (!block.statements().isEmpty()) {
        {
            auto indent = bumpIndent(*this);
            m_out.print("\n");
            visitVector(block.statements(), "\n");
        }
        m_out.print("\n", m_indent);
    }
    m_out.print("}\n");
}

void StringDumper::visit(ReturnStatement& statement)
{
    m_out.print(m_indent, "return");
    if (statement.maybeExpression()) {
        m_out.print(" ");
        visit(*statement.maybeExpression());
    }
    m_out.print(";");
}

void StringDumper::visit(VariableStatement& statement)
{
    m_out.print(m_indent);
    visit(statement.declaration());
}

// Types
void StringDumper::visit(ArrayType& type)
{
    m_out.print("array");
    if (type.maybeElementType()) {
        m_out.print("<");
        visit(*type.maybeElementType());
        if (type.maybeElementCount()) {
            m_out.print(", ");
            visit(*type.maybeElementCount());
        }
        m_out.print(">");
    }
}

void StringDumper::visit(NamedType& type)
{
    m_out.print(type.name());
}

void StringDumper::visit(ParameterizedType& type)
{
    constexpr ASCIILiteral base[] = {
        "Vec2"_s,
        "Vec3"_s,
        "Vec4"_s,
        "Mat2x2"_s,
        "Mat2x3"_s,
        "Mat2x4"_s,
        "Mat3x2"_s,
        "Mat3x3"_s,
        "Mat3x4"_s,
        "Mat4x2"_s,
        "Mat4x3"_s,
        "Mat4x4"_s
    };
    auto b = WTF::enumToUnderlyingType(type.base());
    m_out.print(base[b], "<");
    visit(type.elementType());
    m_out.print(">");
}

void StringDumper::visit(StructType& type)
{
    m_out.print(type.structDecl().name());
}

void StringDumper::visit(ReferenceType& type)
{
    visit(type.type());
    m_out.print("&");
}

void StringDumper::visit(Parameter& parameter)
{
    m_out.print(m_indent);
    if (!parameter.attributes().isEmpty()) {
        visitVector(parameter.attributes(), " ");
        m_out.print(" ");
    }
    m_out.print(parameter.name(), ": ");
    visit(parameter.type());
}

void StringDumper::visit(StructMember& member)
{
    m_out.print(m_indent);
    if (!member.attributes().isEmpty()) {
        visitVector(member.attributes(), " ");
        m_out.print(" ");
    }
    m_out.print(member.name(), ": ");
    visit(member.type());
}

void StringDumper::visit(VariableQualifier& qualifier)
{
    constexpr ASCIILiteral accessMode[]= { "read"_s, "write"_s, "read_write"_s };
    constexpr ASCIILiteral storageClass[] = { "function"_s, "private"_s, "workgroup"_s, "uniform"_s, "storage"_s };
    auto sc = WTF::enumToUnderlyingType(qualifier.storageClass());
    auto am = WTF::enumToUnderlyingType(qualifier.accessMode());
    m_out.print("<", storageClass[sc], ",", accessMode[am], ">");
}

template<typename T>
void dumpNode(PrintStream& out, T& node)
{
    StringDumper dumper;
    dumper.visit(node);
    out.print(dumper.toString());
}

void dumpAST(ShaderModule& shaderModule)
{
    dataLogLn(ShaderModuleDumper(shaderModule));
}

} // namespace WGSL::AST
