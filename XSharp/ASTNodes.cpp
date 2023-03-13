#include "ASTNodes.h"
#include <bits/ranges_algo.h>
#include <math.h>
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
#include "XSharp/Types/TypeSystem.h"
#include "XSharp/XString.h"
#include "fmt/format.h"

using namespace XSharp;

IntegerNode::IntegerNode(int64_t value) : _value(value) {}

XString IntegerNode::dump() const { return XString::fromInterger(_value); }

void IntegerNode::setValue(int64_t value) { _value = value; }

int64_t IntegerNode::value() const { return _value; }

DecimalFractionNode::DecimalFractionNode(double value) : _value(value) {}

XString DecimalFractionNode::dump() const { return XString::number(_value); }

void DecimalFractionNode::setValue(double value) { _value = value; }

double DecimalFractionNode::value() const { return _value; }

BooleanNode::BooleanNode(bool value) : _value(value) {}

XString BooleanNode::dump() const { return fmt::format("{}", _value); }

void BooleanNode::setValue(bool value) { _value = value; }

bool BooleanNode::value() const { return _value; }

StringNode::StringNode(const XString& value) : _value(value) {}

XString StringNode::dump() const { return "\"" + _value + "\""; }

void StringNode::setValue(XString value) { _value = value; }

XString StringNode::value() const { return _value; }

BinaryOperatorNode::BinaryOperatorNode()
    : _parent(nullptr), _left(nullptr), _right(nullptr)
{
}

XString BinaryOperatorNode::dump() const
{
    return fmt::format("[{}] {} [{}]", _left->dump(), _operatorStr,
                       _right->dump());
}

void BinaryOperatorNode::setLeft(ASTNode* left)
{
    _left = left;
    if (left->is<BinaryOperatorNode>()) {
        ((BinaryOperatorNode*)left)->setParent(this);
    }
}

ASTNode* BinaryOperatorNode::left() { return _left; }

void BinaryOperatorNode::setRight(ASTNode* right)
{
    _right = right;
    if (right->is<BinaryOperatorNode>()) {
        ((BinaryOperatorNode*)right)->setParent(this);
    }
}

ASTNode* BinaryOperatorNode::right() { return _right; }

void BinaryOperatorNode::setParent(BinaryOperatorNode* parent)
{
    _parent = parent;
}

BinaryOperatorNode* BinaryOperatorNode::parent() { return _parent; }

void BinaryOperatorNode::setOperatorStr(const XString& operatorStr)
{
    _operatorStr = operatorStr;
}

XString BinaryOperatorNode::operatorStr() const { return _operatorStr; }

BinaryOperatorNode::~BinaryOperatorNode()
{
    delete _left;
    delete _right;
}

FunctionNode::FunctionNode() : _impl(nullptr) {}

XString FunctionNode::dump() const
{
    std::vector<std::string> paramDumps;
    for (VariableNode* param : _params) {
        paramDumps.push_back(param->dump().toStdString());
    }

    XString implDump = "No implementation";
    if (_impl) {
        implDump = _impl->dump();
    }
    return fmt::format("func [{}]({})->[{}] {}", _name,
                       fmt::join(paramDumps, ","), _returnType->typeName(),
                       implDump);
}

void FunctionNode::setName(const XString& name) { _name = name; }

XString FunctionNode::name() const { return _name; }

void FunctionNode::setReturnType(TypeNode* returnType)
{
    _returnType = returnType;
}

TypeNode* FunctionNode::returnType() const { return _returnType; }

void FunctionNode::setParams(std::vector<VariableNode*> params)
{
    _params = params;
}

void FunctionNode::addParam(VariableNode* param) { _params.push_back(param); }

std::vector<VariableNode*> FunctionNode::params() { return _params; }

BlockNode* FunctionNode::impl() const { return _impl; }

void FunctionNode::setImpl(BlockNode* impl) { _impl = impl; }

FunctionNode::~FunctionNode()
{
    for (auto param : params()) {
        delete param;
    }
    delete _impl;
}

VariableNode::VariableNode() : _initValue(nullptr) {}

XString VariableNode::dump() const
{
    if (_initValue) {
        return fmt::format("Var {}:{} = {}", _name, _type->typeName(),
                           _initValue->dump());
    } else {
        return fmt::format("Var {}:{}", _name, _type->typeName());
    }
}

void VariableNode::setType(TypeNode* type) { _type = type; }

TypeNode* VariableNode::type() const { return _type; }

void VariableNode::setName(const XString& name) { _name = name; }

XString VariableNode::name() const { return _name; }

void VariableNode::setInitValue(ASTNode* initValue) { _initValue = initValue; }

ASTNode* VariableNode::initValue() const { return _initValue; }

VariableNode::~VariableNode() { delete _initValue; }

XString BlockNode::dump() const
{
    std::vector<std::string> dumps;
    for (auto i : _contents) {
        if (i) {
            dumps.push_back(i->dump().toStdString());
        }
    }
    return fmt::format("{{\n{}\n}}", fmt::join(dumps, "\n"));
}

void BlockNode::addContent(ASTNode* content) { _contents.push_back(content); }

void BlockNode::setContents(std::vector<ASTNode*> contents)
{
    _contents = contents;
}

std::vector<ASTNode*> BlockNode::contents() const { return _contents; }

BlockNode::~BlockNode()
{
    for (auto p : _contents) delete p;
}

XString FunctionCallNode::dump() const
{
    std::vector<std::string> paramDumps;
    for (auto param : _params) {
        paramDumps.push_back(param->dump().toStdString());
    }

    return fmt::format("call {}({})", _function->dump(),
                       fmt::join(paramDumps, ","));
}

void FunctionCallNode::setFunction(ASTNode* func) { _function = func; }

ASTNode* FunctionCallNode::function() { return _function; }

void FunctionCallNode::setParams(std::vector<ASTNode*> params)
{
    _params = params;
}

void FunctionCallNode::addParam(ASTNode* param) { _params.push_back(param); }

std::vector<ASTNode*> FunctionCallNode::params() const { return _params; }

FunctionCallNode::~FunctionCallNode()
{
    for (auto i : _params) delete i;
    delete _function;
}

XString UnaryOperatorNode::dump() const
{
    return fmt::format("{}[{}]", _operatorStr, _operand->dump());
}

void UnaryOperatorNode::setOperand(ASTNode* operand) { _operand = operand; }

ASTNode* UnaryOperatorNode::operand() { return _operand; }

void UnaryOperatorNode::setOperatorStr(const XString& operatorStr)
{
    _operatorStr = operatorStr;
}

XString UnaryOperatorNode::operatorStr() const { return _operatorStr; }

UnaryOperatorNode::~UnaryOperatorNode() { delete _operand; }

VariableExprNode::VariableExprNode(const XString name) : _name(name) {}

XString VariableExprNode::dump() const { return fmt::format("<{}>", _name); }

void VariableExprNode::setName(const XString& name) { _name = name; }

XString VariableExprNode::name() const { return _name; }

BoxNode::BoxNode(ASTNode* child) : _child(child) {}

XString BoxNode::dump() const { return _child->dump(); }

void BoxNode::setChild(ASTNode* child) { _child = child; }

ASTNode* BoxNode::child() { return _child; }

BoxNode::~BoxNode() { delete _child; }

MemberExprNode::MemberExprNode(const XString name)
    : _memberName(name), _object(nullptr)
{
}

XString MemberExprNode::dump() const
{
    return fmt::format("{}.{}", _object->dump(), _memberName);
}

void MemberExprNode::setMemberName(const XString& name) { _memberName = name; }

XString MemberExprNode::memberName() const { return _memberName; }

void MemberExprNode::setObject(ASTNode* object) { _object = object; }

ASTNode* MemberExprNode::object() { return _object; }
MemberExprNode::~MemberExprNode() { delete _object; }

XString IndexNode::dump() const
{
    return "IndexOf[" + _operand->dump() + "] At [" + _indexExpr->dump() + "]";
}

ASTNode* IndexNode::setOperand(ASTNode* operand) { return _operand = operand; }

ASTNode* IndexNode::operand() { return _operand; }

void IndexNode::setIndexExpr(ASTNode* indexExpr) { _indexExpr = indexExpr; }

ASTNode* IndexNode::indexExpr() { return _indexExpr; }

ReturnNode::ReturnNode(ASTNode* expr) : retVal(expr) {}

XString ReturnNode::dump() const
{
    if (retVal)
        return fmt::format("return {}", retVal->dump());
    else
        return "";
}

void ReturnNode::setReturnValue(ASTNode* retVal) { this->retVal = retVal; }
ASTNode* ReturnNode::returnValue() { return retVal; }

ReturnNode::~ReturnNode()
{
    if (retVal) delete retVal;
}
