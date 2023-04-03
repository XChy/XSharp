#include "ClassAST.h"
#include "XSharp/XString.h"
#include "fmt/format.h"

using namespace XSharp;

XString ClassNode::dump() const
{
    std::vector<std::string> fieldDumps;
    for (auto member : members) {
        fieldDumps.push_back(fmt::format("field : {}", member->dump()));
    }
    for (auto method : methods) {
        fieldDumps.push_back(fmt::format("field : {}", method->dump()));
    }

    return fmt::format("Class {}{{\n{}\n}}", name, fmt::join(fieldDumps, "\n"));
}

ClassNode::~ClassNode()
{
    for (auto i : members) delete i;
    for (auto i : methods) delete i;
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

void DefinitionsNode::addVariable(VariableNode* variableDeclaration)
{
    decls.push_back(variableDeclaration);
}

DefinitionsNode::~DefinitionsNode()
{
    for (auto p : decls) delete p;
}

InitializerNode::~InitializerNode()
{
    for (auto p : parameters) delete p;
    delete impl;
}
