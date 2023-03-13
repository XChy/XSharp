#pragma once
#include <vector>
#include "XSharp/ASTNodes.h"

namespace XSharp {

class XSharp_EXPORT ClassNode : public ASTNode
{
   public:
    XString dump() const;

    ~ClassNode();

    XString name;
    // TODO: Design specific Member/MemberMethod AST
    std::vector<VariableNode*> members;
    std::vector<FunctionNode*> methods;
};

class XSharp_EXPORT DefinitionsNode : public ASTNode
{
   public:
    XString dump() const;

    void addClass(ClassNode* classDeclaration);
    void addFunction(FunctionNode* functionDeclaration);
    void addVariable(VariableNode* variableDeclaration);

    std::vector<ClassNode*> classes() const;
    std::vector<FunctionNode*> functions() const;
    std::vector<VariableNode*> variables() const;

    ~DefinitionsNode();

   private:
    std::vector<ClassNode*> _classes;
    std::vector<FunctionNode*> _functions;
    std::vector<VariableNode*> _variables;
};
}  // namespace XSharp
