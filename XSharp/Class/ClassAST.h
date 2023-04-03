#pragma once
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

    ~DefinitionsNode();

    std::vector<ASTNode*> decls;
};

class XSharp_EXPORT InitializerNode : public ASTNode
{
   public:
    ~InitializerNode();

    std::vector<VariableNode*> parameters;
    BlockNode* impl;
};

}  // namespace XSharp
