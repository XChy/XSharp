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

class XSharp_EXPORT ElseIfNode : public ASTNode
{
   public:
    XString dump() const;
    ~ElseIfNode();

    ASTNode* condition();
    void setCondition(ASTNode* cond);

    BlockNode* block();
    void setBlock(BlockNode* block);

    ASTNode* lastIf();
    void setLastIf(ASTNode* lastIf);

   private:
    ASTNode* _lastIf;
    ASTNode* _condition;
    BlockNode* _block;
};

class XSharp_EXPORT ElseNode : public ASTNode
{
   public:
    XString dump() const;
    ~ElseNode();

    BlockNode* block();
    void setBlock(BlockNode* block);

    ASTNode* lastIf();
    void setLastIf(ASTNode* lastIf);

   private:
    ASTNode* _lastIf;
    BlockNode* _block;
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
    XString dump() const;
    ~WhileNode();

    ASTNode* condition();
    void setCondition(ASTNode* cond);

    ASTNode* block();
    void setBlock(BlockNode* block);

   private:
    ASTNode* _condition;
    BlockNode* _block;
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

};  // namespace XSharp
