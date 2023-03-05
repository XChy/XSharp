#pragma once

#include "XSharp/ASTNodes.h"
#include "XSharp/xsharp_global.h"

namespace XSharp {

// TODO: implement the AST below

class XSharp_EXPORT IfNode
{
   public:
    XString dump() const;
    ~IfNode();

    ASTNode* condition();
    void setCondition(ASTNode* cond);
    BlockNode* block();
    void setBlock(BlockNode* block);

   private:
    ASTNode* _condition;
    BlockNode* _block;
};

class XSharp_EXPORT ElseIfNode
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

class XSharp_EXPORT ElseNode
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

class XSharp_EXPORT ForNode
{
   public:
    XString dump() const;
    ~ForNode();

   private:
};

class XSharp_EXPORT WhileNode
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

class XSharp_EXPORT ContinueNode
{
   public:
    XString dump() const;
    ~ContinueNode();

   private:
};

class XSharp_EXPORT BreakNode
{
   public:
    XString dump() const;
    ~BreakNode();

   private:
};

};  // namespace XSharp
