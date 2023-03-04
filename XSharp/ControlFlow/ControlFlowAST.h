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
    ASTNode* setCondition(ASTNode* cond);
    ASTNode* block();
    ASTNode* setBlock(ASTNode* block);

   private:
    ASTNode* _condition;
    ASTNode* _block;
};

class XSharp_EXPORT ElseIfNode
{
   public:
    XString dump() const;
    ~ElseIfNode();

    ASTNode* condition();
    ASTNode* setCondition(ASTNode* cond);

    ASTNode* block();
    ASTNode* setBlock(ASTNode* block);

    ASTNode* lastIf();
    ASTNode* setLastIf(ASTNode* lastIf);

   private:
    ASTNode* _lastIf;
    ASTNode* _condition;
    ASTNode* _block;
};

class XSharp_EXPORT ElseNode
{
   public:
    virtual XString dump() const;
    virtual ~ElseNode();

   private:
    ASTNode* _lastIf;
    ASTNode* _block;
};

class XSharp_EXPORT ForNode
{
   public:
    virtual XString dump() const;
    virtual ~ForNode();

   private:
};

class XSharp_EXPORT WhileNode
{
   public:
    virtual XString dump() const;
    virtual ~WhileNode();

   private:
};

class XSharp_EXPORT ContinueNode
{
   public:
    virtual XString dump() const;
    virtual ~ContinueNode();

   private:
};

class XSharp_EXPORT BreakNode
{
   public:
    virtual XString dump() const;
    virtual ~BreakNode();

   private:
};

};  // namespace XSharp
