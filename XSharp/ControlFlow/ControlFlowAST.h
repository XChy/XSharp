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
    ASTNode* block();
    void setBlock(ASTNode* block);

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
    void setCondition(ASTNode* cond);

    ASTNode* block();
    void setBlock(ASTNode* block);

    ASTNode* lastIf();
    void setLastIf(ASTNode* lastIf);

   private:
    ASTNode* _lastIf;
    ASTNode* _condition;
    ASTNode* _block;
};

class XSharp_EXPORT ElseNode
{
   public:
    XString dump() const;
    ~ElseNode();

    ASTNode* block();
    void setBlock(ASTNode* block);

    ASTNode* lastIf();
    void setLastIf(ASTNode* lastIf);

   private:
    ASTNode* _lastIf;
    ASTNode* _block;
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

   private:
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
