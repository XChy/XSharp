#pragma once
#include "XSharp/ASTNodes.h"
#include "XSharp/XString.h"

namespace XSharp {

class MemberMethodNode;

class XSharp_EXPORT ClassNode : public ASTNode
{
   public:
    XString dump() const;

    ~ClassNode();

    XString name;
    // TODO: Design specific Member/MemberMethod AST
    std::vector<VariableNode*> members;
    std::vector<MemberMethodNode*> methods;
};

class XSharp_EXPORT MemberMethodNode : public FunctionNode
{
   public:
    XString dump() const;

    ClassNode* selfClass;
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
