#include "ASTNodes.h"
#include <cmath>
#include <vector>
#include "XSharp/Type.h"
#include "XSharp/XString.h"

IntegerNode::IntegerNode(int64_t value) : _value(value) {}

XString IntegerNode::dump() const { return XString::fromInterger(_value); }

void IntegerNode::setValue(int64_t value) { _value = value; }

int64_t IntegerNode::value() const { return _value; }

DecimalFractionNode::DecimalFractionNode(double value) : _value(value) {}

XString DecimalFractionNode::dump() const { return XString::number(_value); }

void DecimalFractionNode::setValue(double value) { _value = value; }

double DecimalFractionNode::value() const { return _value; }

BooleanNode::BooleanNode(bool value) : _value(value) {}

XString BooleanNode::dump() const
{
    if (_value)
        return "true";
    else
        return "false";
}

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
    return _operatorStr + "{ left:" + _left->dump() +
           "\n"
           "right:" +
           _right->dump() + "}";
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

XString ClassDeclarationNode::dump() const
{
    return "Class{name:" + _name + "\n " + "}";
}

XString DefinitionsNode::dump() const
{
    XString result;
    for (auto i : _classDeclarations) {
        result.append(i->dump());
    }
    for (auto i : _functionDeclarations) {
        result.append(i->dump());
    }
    for (auto i : _variableDeclarations) {
        result.append(i->dump());
    }
    return result;
}

void DefinitionsNode::addClass(ClassDeclarationNode* classDeclaration)
{
    _classDeclarations.push_back(classDeclaration);
}

void DefinitionsNode::addFunction(FunctionDeclarationNode* functionDeclaration)
{
    _functionDeclarations.push_back(functionDeclaration);
}

void DefinitionsNode::addVariable(VariableDeclarationNode* variableDeclaration)
{
    _variableDeclarations.push_back(variableDeclaration);
}

std::vector<ClassDeclarationNode*> DefinitionsNode::classDeclarations() const
{
    return _classDeclarations;
}

std::vector<FunctionDeclarationNode*> DefinitionsNode::functionDeclarations()
    const
{
    return _functionDeclarations;
}

std::vector<VariableDeclarationNode*> DefinitionsNode::variableDeclarations()
    const
{
    return _variableDeclarations;
}

DefinitionsNode::~DefinitionsNode()
{
    for (auto p : _variableDeclarations) delete p;
    for (auto p : _functionDeclarations) delete p;
    for (auto p : _classDeclarations) delete p;
}

FunctionDeclarationNode::FunctionDeclarationNode() : _impl(nullptr) {}

XString FunctionDeclarationNode::dump() const
{
    XString paramsDump;
    for (VariableDeclarationNode* param : _params) {
        paramsDump.append(param->dump()).append("\n");
    }

    XString implDump = "no";
    if (_impl) {
        implDump = _impl->dump();
    }
    return "Function{name:" + _name + "\nreturnType:" + _returnType.typeName() +
           "\nparams:{" + paramsDump + "}\nblock:{" + implDump + "}\n}\n";
}

void FunctionDeclarationNode::setName(const XString& name) { _name = name; }

XString FunctionDeclarationNode::name() const { return _name; }

void FunctionDeclarationNode::setReturnType(const TypeNode& returnType)
{
    _returnType = returnType;
}

TypeNode FunctionDeclarationNode::returnType() const { return _returnType; }

void FunctionDeclarationNode::setParams(
    std::vector<VariableDeclarationNode*> params)
{
    _params = params;
}

void FunctionDeclarationNode::addParam(VariableDeclarationNode* param)
{
    _params.push_back(param);
}

std::vector<VariableDeclarationNode*> FunctionDeclarationNode::params()
{
    return _params;
}

BlockNode* FunctionDeclarationNode::impl() const { return _impl; }

void FunctionDeclarationNode::setImpl(BlockNode* impl) { _impl = impl; }

FunctionDeclarationNode::~FunctionDeclarationNode()
{
    for (auto param : params()) {
        delete param;
    }
    delete _impl;
}

VariableDeclarationNode::VariableDeclarationNode() : _initValue(nullptr)
{
    _type.isConst = false;
}

XString VariableDeclarationNode::dump() const
{
    XString initDump = "No";
    if (_initValue) {
        initDump = _initValue->dump();
    }
    return "Variable{name:" + _name + "\ntype:" + _type.typeName() +
           "\ninitValue:" + initDump + "}\n";
}

void VariableDeclarationNode::setType(const TypeNode& type) { _type = type; }

TypeNode VariableDeclarationNode::type() const { return _type; }

void VariableDeclarationNode::setName(const XString& name) { _name = name; }

XString VariableDeclarationNode::name() const { return _name; }

void VariableDeclarationNode::setInitValue(ASTNode* initValue)
{
    _initValue = initValue;
}

ASTNode* VariableDeclarationNode::initValue() const { return _initValue; }

VariableDeclarationNode::~VariableDeclarationNode() { delete _initValue; }

bool VariableDeclarationNode::isConst() const { return _type.isConst; }

void VariableDeclarationNode::setIsConst(bool newIsConst)
{
    _type.isConst = newIsConst;
}

XString BlockNode::dump() const
{
    XString result;
    for (auto i : _contents) {
        if (i) {
            result.append(i->dump()).append("\n");
        }
    }
    return result;
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
    XString paramsDump;
    for (auto param : _params) {
        paramsDump.append(param->dump()).append(',');
    }

    return "FunctionCall{function:" + _function->dump() + "\nparams:{" +
           paramsDump + "}\n}";
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
    return _operatorStr + "{ operand:" + _operand->dump() + "\n}";
}

void UnaryOperatorNode::setOperand(ASTNode* operand) { _operand = operand; }

ASTNode* UnaryOperatorNode::operand() { return _operand; }

void UnaryOperatorNode::setOperatorStr(const XString& operatorStr)
{
    _operatorStr = operatorStr;
}

XString UnaryOperatorNode::operatorStr() const { return _operatorStr; }

UnaryOperatorNode::~UnaryOperatorNode() { delete _operand; }

VariableNode::VariableNode(const XString name) : _name(name) {}

XString VariableNode::dump() const { return "Variable{name:" + _name + "}"; }

void VariableNode::setName(const XString& name) { _name = name; }

XString VariableNode::name() const { return _name; }

BoxNode::BoxNode(ASTNode* child) : _child(child) {}

XString BoxNode::dump() const { return _child->dump(); }

void BoxNode::setChild(ASTNode* child) { _child = child; }

ASTNode* BoxNode::child() { return _child; }

BoxNode::~BoxNode() { delete _child; }

MemberNode::MemberNode(const XString name) : _name(name), _object(nullptr) {}

XString MemberNode::dump() const { return _object->dump() + "." + _name; }

void MemberNode::setName(const XString& name) { _name = name; }

XString MemberNode::name() const { return _name; }

void MemberNode::setObject(ASTNode* object) { _object = object; }

ASTNode* MemberNode::object() { return _object; }

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
    return "Return " + (retVal ? retVal->dump() : "") + "\n";
}

void ReturnNode::setReturnValue(ASTNode* retVal) { this->retVal = retVal; }
ASTNode* ReturnNode::returnValue() { return retVal; }

ReturnNode::~ReturnNode() { delete retVal; }
