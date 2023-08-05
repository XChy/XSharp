#pragma once

#include "xsharp_global.h"
#include "XString.h"

namespace XSharp {

class TypeNode;

class XSharp_EXPORT ASTNode
{
   public:
    virtual XString dump() const = 0;

    template <typename T>
    T* to()
    {
        return dynamic_cast<T*>(this);
    }

    template <typename T>
    bool is()
    {
        return bool(dynamic_cast<T*>(this));
    }

    virtual ~ASTNode() = default;  // The children are managed by the parent
};

class XSharp_EXPORT IntegerNode : public ASTNode
{
   public:
    IntegerNode(int64_t value = 0);
    XString dump() const;

    void setValue(int64_t value);
    int64_t value() const;

   private:
    int64_t _value;
};

class XSharp_EXPORT FPNode : public ASTNode
{
   public:
    FPNode(double value = 0);
    XString dump() const;

    void setValue(double value);
    double value() const;

   private:
    double _value;
};

class XSharp_EXPORT BooleanNode : public ASTNode
{
   public:
    BooleanNode(bool value = 0);
    XString dump() const;

    void setValue(bool value);
    bool value() const;

   private:
    bool _value;
};

class XSharp_EXPORT CharNode : public ASTNode
{
   public:
    CharNode(const XChar& value);
    XString dump() const;

    XChar value;
};

class XSharp_EXPORT StringNode : public ASTNode
{
   public:
    StringNode(const XString& value);
    XString dump() const;

    void setValue(XString value);
    XString value() const;

   private:
    XString _value;
};

class XSharp_EXPORT BinaryOperatorNode : public ASTNode
{
   public:
    BinaryOperatorNode();
    XString dump() const;

    void setLhs(ASTNode* left);
    ASTNode* lhs();

    void setRhs(ASTNode* right);
    ASTNode* rhs();

    void setParent(BinaryOperatorNode* parent);
    BinaryOperatorNode* parent();

    void setOpStr(const XString& operatorStr);
    XString opStr() const;

    ~BinaryOperatorNode();

   private:
    ASTNode* _lhs;
    ASTNode* _rhs;
    BinaryOperatorNode* _parent;
    XString _opStr;
};

class XSharp_EXPORT UnaryOpNode : public ASTNode
{
   public:
    XString dump() const;

    void setOperand(ASTNode* operand);
    ASTNode* operand();

    void setOpStr(const XString& operatorStr);
    XString opStr() const;

    ~UnaryOpNode();

   private:
    ASTNode* _operand;
    XString _opStr;
};

class XSharp_EXPORT BlockNode : public ASTNode
{
   public:
    XString dump() const;

    void addContent(ASTNode* content);

    void setContents(std::vector<ASTNode*> contents);
    std::vector<ASTNode*> contents() const;

    ~BlockNode();

   private:
    std::vector<ASTNode*> _contents;
};

class XSharp_EXPORT VarDeclNode : public ASTNode
{
   public:
    VarDeclNode();

    XString dump() const;

    void setType(TypeNode* type);
    TypeNode* type() const;

    void setName(const XString& name);
    XString name() const;

    void setInit(ASTNode* init);
    ASTNode* init() const;

    ~VarDeclNode();

   private:
    TypeNode* _type;
    XString _name;
    ASTNode* _init;
};

class XSharp_EXPORT FunctionNode : public ASTNode
{
   public:
    FunctionNode();

    XString dump() const;

    void setName(const XString& name);
    XString name() const;

    void setReturnType(TypeNode* returnType);
    TypeNode* returnType() const;

    void setParams(std::vector<VarDeclNode*> params);
    void addParam(VarDeclNode* param);
    std::vector<VarDeclNode*> params();

    BlockNode* impl() const;
    void setImpl(BlockNode* impl);

    ~FunctionNode();

   private:
    XString _name;
    TypeNode* _returnType;
    std::vector<VarDeclNode*> _params;  // <type name,param name>
    BlockNode* _impl;
};

class XSharp_EXPORT CallNode : public ASTNode
{
   public:
    XString dump() const;

    void setCallee(ASTNode* func);
    ASTNode* callee();

    void setArgs(std::vector<ASTNode*> args);
    void addArg(ASTNode* arg);
    std::vector<ASTNode*> args() const;

    ~CallNode();

   private:
    ASTNode* _callee;
    std::vector<ASTNode*> _args;
};

class XSharp_EXPORT VarExprNode : public ASTNode
{
   public:
    VarExprNode(const XString name);

    XString dump() const;

    void setName(const XString& name);
    XString name() const;

   private:
    XString _name;
};

class XSharp_EXPORT MemberExprNode : public ASTNode
{
   public:
    MemberExprNode(const XString name);

    XString dump() const;

    void setMemberName(const XString& name);
    XString memberName() const;

    void setObject(ASTNode* object);
    ASTNode* object();

    ~MemberExprNode();

   private:
    XString _memberName;
    ASTNode* _object;
};

class XSharp_EXPORT IndexNode : public ASTNode
{
   public:
    ~IndexNode();
    XString dump() const;

    ASTNode* setIndexed(ASTNode* indexed);
    ASTNode* operand();

    void setIndex(ASTNode* indexExpr);
    ASTNode* index();

   private:
    ASTNode* _indexed;
    ASTNode* _index;
};

// Control Flow's ASTNode

}  // namespace XSharp
