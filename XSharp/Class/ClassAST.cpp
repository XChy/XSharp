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
    return fmt::format("Class {}{{ \n{}\n }}", name,
                       fmt::join(fieldDumps, "\n"));
}

ClassNode::~ClassNode()
{
    for (auto i : members) delete i;
    for (auto i : methods) delete i;
}
XString DefinitionsNode::dump() const
{
    XString result;
    for (auto i : _classes) {
        result.append(i->dump());
        result.append('\n');
    }
    for (auto i : _functions) {
        result.append(i->dump());
        result.append('\n');
    }
    for (auto i : _variables) {
        result.append(i->dump());
        result.append('\n');
    }
    return result;
}

void DefinitionsNode::addClass(ClassNode* classDeclaration)
{
    _classes.push_back(classDeclaration);
}

void DefinitionsNode::addFunction(FunctionNode* functionDeclaration)
{
    _functions.push_back(functionDeclaration);
}

void DefinitionsNode::addVariable(VariableNode* variableDeclaration)
{
    _variables.push_back(variableDeclaration);
}

std::vector<ClassNode*> DefinitionsNode::classes() const { return _classes; }

std::vector<FunctionNode*> DefinitionsNode::functions() const
{
    return _functions;
}

std::vector<VariableNode*> DefinitionsNode::variables() const
{
    return _variables;
}

DefinitionsNode::~DefinitionsNode()
{
    for (auto p : _variables) delete p;
    for (auto p : _functions) delete p;
    for (auto p : _classes) delete p;
}
