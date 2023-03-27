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

class XSharp_EXPORT DecimalFractionNode : public ASTNode
{
   public:
    DecimalFractionNode(double value = 0);
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

class XSharp_EXPORT BoxNode : public ASTNode
{  // Applied to hide the binary operator node's information
   public:
    BoxNode(ASTNode* child);
    XString dump() const;

    void setChild(ASTNode* child);
    ASTNode* child();

    ~BoxNode();

   private:
    ASTNode* _child;
};

class XSharp_EXPORT BinaryOperatorNode : public ASTNode
{
   public:
    BinaryOperatorNode();
    XString dump() const;

    void setLeft(ASTNode* left);
    ASTNode* left();

    void setRight(ASTNode* right);
    ASTNode* right();

    void setParent(BinaryOperatorNode* parent);
    BinaryOperatorNode* parent();

    void setOperatorStr(const XString& operatorStr);
    XString operatorStr() const;

    ~BinaryOperatorNode();

   private:
    ASTNode* _left;
    ASTNode* _right;
    BinaryOperatorNode* _parent;
    XString _operatorStr;
};

class XSharp_EXPORT UnaryOperatorNode : public ASTNode
{
   public:
    XString dump() const;

    void setOperand(ASTNode* operand);
    ASTNode* operand();

    void setOperatorStr(const XString& operatorStr);
    XString operatorStr() const;

    // TypeNode* exprType();
    ~UnaryOperatorNode();

   private:
    ASTNode* _operand;
    XString _operatorStr;
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

class XSharp_EXPORT VariableNode : public ASTNode
{
   public:
    VariableNode();

    XString dump() const;

    void setType(TypeNode* type);
    TypeNode* type() const;

    void setName(const XString& name);
    XString name() const;

    void setInitValue(ASTNode* initValue);
    ASTNode* initValue() const;

    ~VariableNode();

   private:
    TypeNode* _type;
    XString _name;
    ASTNode* _initValue;
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

    void setParams(std::vector<VariableNode*> params);
    void addParam(VariableNode* param);
    std::vector<VariableNode*> params();

    BlockNode* impl() const;
    void setImpl(BlockNode* impl);

    ~FunctionNode();

   private:
    XString _name;
    TypeNode* _returnType;
    std::vector<VariableNode*> _params;  // <type name,param name>
    BlockNode* _impl;
};

class XSharp_EXPORT FunctionCallNode : public ASTNode
{
   public:
    XString dump() const;

    void setFunction(ASTNode* func);
    ASTNode* function();

    void setArgs(std::vector<ASTNode*> args);
    void addArg(ASTNode* arg);
    std::vector<ASTNode*> params() const;

    ~FunctionCallNode();

   private:
    ASTNode* _function;
    std::vector<ASTNode*> _args;
};

class XSharp_EXPORT VariableExprNode : public ASTNode
{
   public:
    VariableExprNode(const XString name);

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
    XString dump() const;

    ASTNode* setOperand(ASTNode* operand);
    ASTNode* operand();

    void setIndexExpr(ASTNode* indexExpr);
    ASTNode* indexExpr();

   private:
    ASTNode* _operand;
    ASTNode* _indexExpr;
};

// Control Flow's ASTNode

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
}  // namespace XSharp
