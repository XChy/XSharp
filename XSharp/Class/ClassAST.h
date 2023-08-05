#pragma once
#include "XSharp/ASTNodes.h"
#include "XSharp/XString.h"

namespace XSharp {

class MemberMethodNode;
class ConstructorNode;

class XSharp_EXPORT ClassNode : public ASTNode
{
   public:
    XString dump() const;

    ~ClassNode();

    XString name;

    XString superClass;

    std::vector<VarDeclNode*> members;
    std::vector<MemberMethodNode*> methods;
    std::vector<ConstructorNode*> constructors;
};

class XSharp_EXPORT MemberMethodNode : public FunctionNode
{
   public:
    XString dump() const;

    ClassNode* selfClass;
};

class XSharp_EXPORT ConstructorNode : public ASTNode
{
   public:
    XString dump() const;

    ~ConstructorNode();

    ClassNode* selfClass;
    std::vector<VarDeclNode*> parameters;
    BlockNode* impl;
};

class XSharp_EXPORT DefinitionsNode : public ASTNode
{
   public:
    XString dump() const;

    void addClass(ClassNode* classDeclaration);
    void addFunction(FunctionNode* functionDeclaration);
    void addVariable(VarDeclNode* variableDeclaration);

    ~DefinitionsNode();

    std::vector<ASTNode*> decls;
};

}  // namespace XSharp
