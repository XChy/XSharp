#include "ASTNodes.h"
#include <bits/ranges_algo.h>
#include <math.h>
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
#include "XSharp/Tokens.h"
#include "XSharp/Types/TypeNodes.h"
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

CharNode::CharNode(const XChar& value) : value(value) {}

XString CharNode::dump() const { return XString("\'") + value + "\'"; }

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
                       fmt::join(paramDumps, ","), _returnType->dump(),
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
    delete _returnType;
}

VariableNode::VariableNode() : _initValue(nullptr) {}

XString VariableNode::dump() const
{
    if (_initValue) {
        return fmt::format("Var {}:{} = {}", _name, _type->dump(),
                           _initValue->dump());
    } else {
        return fmt::format("Var {}:{}", _name, _type->dump());
    }
}

void VariableNode::setType(TypeNode* type) { _type = type; }

TypeNode* VariableNode::type() const { return _type; }

void VariableNode::setName(const XString& name) { _name = name; }

XString VariableNode::name() const { return _name; }

void VariableNode::setInitValue(ASTNode* initValue) { _initValue = initValue; }

ASTNode* VariableNode::initValue() const { return _initValue; }

VariableNode::~VariableNode()
{
    delete _type;
    delete _initValue;
}

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
    for (auto param : _args) {
        paramDumps.push_back(param->dump().toStdString());
    }

    return fmt::format("call {}({})", _callee->dump(),
                       fmt::join(paramDumps, ","));
}

void FunctionCallNode::setCallee(ASTNode* func) { _callee = func; }

ASTNode* FunctionCallNode::callee() { return _callee; }

void FunctionCallNode::setArgs(std::vector<ASTNode*> params) { _args = params; }

void FunctionCallNode::addArg(ASTNode* param) { _args.push_back(param); }

std::vector<ASTNode*> FunctionCallNode::args() const { return _args; }

FunctionCallNode::~FunctionCallNode()
{
    for (auto i : _args) delete i;
    delete _callee;
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

XString VariableExprNode::dump() const { return _name; }

void VariableExprNode::setName(const XString& name) { _name = name; }

XString VariableExprNode::name() const { return _name; }

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

IndexNode::~IndexNode()
{
    delete _operand;
    delete _index;
}

XString IndexNode::dump() const
{
    return fmt::format("{} in {}", _index->dump(), _operand->dump());
}

ASTNode* IndexNode::setOperand(ASTNode* operand) { return _operand = operand; }

ASTNode* IndexNode::operand() { return _operand; }

void IndexNode::setIndex(ASTNode* indexExpr) { _index = indexExpr; }

ASTNode* IndexNode::index() { return _index; }
