#include "ASTNodes.h"
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

FPNode::FPNode(double value) : _value(value) {}

XString FPNode::dump() const { return XString::number(_value); }

void FPNode::setValue(double value) { _value = value; }

double FPNode::value() const { return _value; }

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
    : _parent(nullptr), _lhs(nullptr), _rhs(nullptr)
{
}

XString BinaryOperatorNode::dump() const
{
    return fmt::format("[{}] {} [{}]", _lhs->dump(), _opStr, _rhs->dump());
}

void BinaryOperatorNode::setLhs(ASTNode* left)
{
    _lhs = left;
    if (left->is<BinaryOperatorNode>()) {
        ((BinaryOperatorNode*)left)->setParent(this);
    }
}

ASTNode* BinaryOperatorNode::lhs() { return _lhs; }

void BinaryOperatorNode::setRhs(ASTNode* right)
{
    _rhs = right;
    if (right->is<BinaryOperatorNode>()) {
        ((BinaryOperatorNode*)right)->setParent(this);
    }
}

ASTNode* BinaryOperatorNode::rhs() { return _rhs; }

void BinaryOperatorNode::setParent(BinaryOperatorNode* parent)
{
    _parent = parent;
}

BinaryOperatorNode* BinaryOperatorNode::parent() { return _parent; }

void BinaryOperatorNode::setOpStr(const XString& operatorStr)
{
    _opStr = operatorStr;
}

XString BinaryOperatorNode::opStr() const { return _opStr; }

BinaryOperatorNode::~BinaryOperatorNode()
{
    delete _lhs;
    delete _rhs;
}

FunctionNode::FunctionNode() : _impl(nullptr) {}

XString FunctionNode::dump() const
{
    std::vector<std::string> paramDumps;
    for (VarDeclNode* param : _params) {
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

void FunctionNode::setParams(std::vector<VarDeclNode*> params)
{
    _params = params;
}

void FunctionNode::addParam(VarDeclNode* param) { _params.push_back(param); }

std::vector<VarDeclNode*> FunctionNode::params() { return _params; }

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

VarDeclNode::VarDeclNode() : _init(nullptr) {}

XString VarDeclNode::dump() const
{
    if (_init) {
        return fmt::format("Var {}:{} = {}", _name, _type->dump(),
                           _init->dump());
    } else {
        return fmt::format("Var {}:{}", _name, _type->dump());
    }
}

void VarDeclNode::setType(TypeNode* type) { _type = type; }

TypeNode* VarDeclNode::type() const { return _type; }

void VarDeclNode::setName(const XString& name) { _name = name; }

XString VarDeclNode::name() const { return _name; }

void VarDeclNode::setInit(ASTNode* initValue) { _init = initValue; }

ASTNode* VarDeclNode::init() const { return _init; }

VarDeclNode::~VarDeclNode()
{
    delete _type;
    delete _init;
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

XString CallNode::dump() const
{
    std::vector<std::string> paramDumps;
    for (auto param : _args) {
        paramDumps.push_back(param->dump().toStdString());
    }

    return fmt::format("call {}({})", _callee->dump(),
                       fmt::join(paramDumps, ","));
}

void CallNode::setCallee(ASTNode* func) { _callee = func; }

ASTNode* CallNode::callee() { return _callee; }

void CallNode::setArgs(std::vector<ASTNode*> params) { _args = params; }

void CallNode::addArg(ASTNode* param) { _args.push_back(param); }

std::vector<ASTNode*> CallNode::args() const { return _args; }

CallNode::~CallNode()
{
    for (auto i : _args) delete i;
    delete _callee;
}

XString UnaryOpNode::dump() const
{
    return fmt::format("{}[{}]", _opStr, _operand->dump());
}

void UnaryOpNode::setOperand(ASTNode* operand) { _operand = operand; }

ASTNode* UnaryOpNode::operand() { return _operand; }

void UnaryOpNode::setOpStr(const XString& operatorStr) { _opStr = operatorStr; }

XString UnaryOpNode::opStr() const { return _opStr; }

UnaryOpNode::~UnaryOpNode() { delete _operand; }

VarExprNode::VarExprNode(const XString name) : _name(name) {}

XString VarExprNode::dump() const { return _name; }

void VarExprNode::setName(const XString& name) { _name = name; }

XString VarExprNode::name() const { return _name; }

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
    delete _indexed;
    delete _index;
}

XString IndexNode::dump() const
{
    return fmt::format("{} in {}", _index->dump(), _indexed->dump());
}

ASTNode* IndexNode::setIndexed(ASTNode* operand) { return _indexed = operand; }

ASTNode* IndexNode::operand() { return _indexed; }

void IndexNode::setIndex(ASTNode* indexExpr) { _index = indexExpr; }

ASTNode* IndexNode::index() { return _index; }
