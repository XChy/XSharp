#pragma once

#include "XSharp/ASTNodes.h"
#include "XSharp/xsharp_global.h"

namespace XSharp {

// TODO: implement the AST below

class XSharp_EXPORT IfNode : public ASTNode
{
   public:
    IfNode() : condition(nullptr), block(nullptr), elseAst(nullptr) {}
    IfNode(ASTNode* cond, ASTNode* codeblock)
        : condition(cond), block(codeblock), elseAst(nullptr)
    {
    }
    XString dump() const;
    ~IfNode();

    // Children

    // <if> ( <condition> ) <statements> [else...]
    ASTNode* condition;
    ASTNode* block;

    // <if> or <else> following <if>
    ASTNode* elseAst;
};

class XSharp_EXPORT ForNode : public ASTNode
{
   public:
    XString dump() const;
    ~ForNode();

   private:
};

class XSharp_EXPORT WhileNode : public ASTNode
{
   public:
    WhileNode() : condition(nullptr), block(nullptr){};
    WhileNode(ASTNode* condition, ASTNode* block)
        : condition(condition), block(block){};
    XString dump() const;
    ~WhileNode();

    ASTNode* condition;
    ASTNode* block;
};

class XSharp_EXPORT ContinueNode : public ASTNode
{
   public:
    XString dump() const;
    ~ContinueNode();

   private:
};

class XSharp_EXPORT BreakNode : public ASTNode
{
   public:
    XString dump() const;
    ~BreakNode();

   private:
};

class XSharp_EXPORT ReturnNode : public ASTNode
{
   public:
    ReturnNode(ASTNode* expr);

    XString dump() const;

    void setReturnValue(ASTNode* retVal);
    ASTNode* returnValue();

    ~ReturnNode();

   private:
    ASTNode* retVal;
};
};  // namespace XSharp
