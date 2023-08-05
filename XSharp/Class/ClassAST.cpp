#include "ClassAST.h"
#include <cmath>
#include "XSharp/XString.h"
#include "fmt/core.h"
#include "fmt/format.h"

using namespace XSharp;

XString ClassNode::dump() const
{
    std::vector<std::string> fieldDumps;

    for (auto constructor : constructors) {
        fieldDumps.push_back(fmt::format("field : {}", constructor->dump()));
    }

    for (auto member : members) {
        fieldDumps.push_back(fmt::format("field : {}", member->dump()));
    }

    for (auto method : methods) {
        fieldDumps.push_back(fmt::format("field : {}", method->dump()));
    }

    std::string extendsDump;
    if (superClass.size() > 0) {
        extendsDump = fmt::format("extends {}", superClass);
    }

    return fmt::format("Class {} {}{{\n{}\n}}", name, extendsDump,
                       fmt::join(fieldDumps, "\n"));
}

ClassNode::~ClassNode()
{
    for (auto i : members) delete i;
    for (auto i : methods) delete i;
    for (auto i : constructors) delete i;
}

XString MemberMethodNode::dump() const
{
    return fmt::format("{}::{}", selfClass->name, FunctionNode::dump());
}

XString DefinitionsNode::dump() const
{
    XString result;
    for (auto decl : decls) {
        result.append(decl->dump());
        result.append('\n');
    }
    return result;
}

void DefinitionsNode::addClass(ClassNode* classDeclaration)
{
    decls.push_back(classDeclaration);
}

void DefinitionsNode::addFunction(FunctionNode* functionDeclaration)
{
    decls.push_back(functionDeclaration);
}

void DefinitionsNode::addVariable(VarDeclNode* variableDeclaration)
{
    decls.push_back(variableDeclaration);
}

DefinitionsNode::~DefinitionsNode()
{
    for (auto p : decls) delete p;
}

XString ConstructorNode::dump() const
{
    std::vector<std::string> paramDumps;

    for (auto param : parameters)
        paramDumps.push_back(param->dump().toStdString());

    return fmt::format("constructor({}){{{}}}", fmt::join(paramDumps, ","),
                       impl->dump());
}

ConstructorNode::~ConstructorNode()
{
    for (auto p : parameters) delete p;
    delete impl;
}
