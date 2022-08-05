#include "ASTNodes.h"

XString IntegerNode::dump() const
{
	return XString::fromInterger(_value);
}

void IntegerNode::setValue(int64_t value)
{
	_value = value;
}

int64_t IntegerNode::value() const
{
	return _value;
}

XString DecimalFractionNode::dump() const
{
	return XString::number(_value);
}

void DecimalFractionNode::setValue(double value)
{
	_value = value;
}

double DecimalFractionNode::value() const
{
	return _value;
}

XString BooleanNode::dump() const
{
	if (_value)
		return "true";
	else
		return "false";
}

void BooleanNode::setValue(bool value)
{
	_value = value;
}

bool BooleanNode::value() const
{
	return _value;
}

XString StringNode::dump() const
{
	return _value;
}

void StringNode::setValue(XString value)
{
	_value = value;
}

XString StringNode::value() const
{
	return _value;
}

XString AddNode::dump() const
{
	return "Add { left:"+_left->dump()+"\n"
		"right:"+_right->dump()+"}";
}

void AddNode::setLeft(ASTNode* left)
{
	_left = left;
}

ASTNode* AddNode::left()
{
	return _left;
}

void AddNode::setRight(ASTNode* right)
{
	_right = right;
}

ASTNode* AddNode::right()
{
	return _right;
}

AddNode::~AddNode()
{
	delete _left;
	delete _right;
}

XString SubNode::dump() const
{
	return "Sub { left:" + _left->dump() + "\n"
		"right:" + _right->dump() + "}";
}

void SubNode::setLeft(ASTNode* left)
{
	_left = left;
}

ASTNode* SubNode::left()
{
	return _left;
}

void SubNode::setRight(ASTNode* right)
{
	_right = right;
}

ASTNode* SubNode::right()
{
	return _right;
}

SubNode::~SubNode()
{
	delete _left;
	delete _right;
}

XString MulNode::dump() const
{
	return "Mul { left:" + _left->dump() + "\n"
		"right:" + _right->dump() + "}";
}

void MulNode::setLeft(ASTNode* left)
{
	_left = left;
}

ASTNode* MulNode::left()
{
	return _left;
}

void MulNode::setRight(ASTNode* right)
{
	_right = right;
}

ASTNode* MulNode::right()
{
	return _right;
}

MulNode::~MulNode()
{
	delete _left;
	delete _right;
}

XString DivNode::dump() const
{
	return "Mul { left:" + _left->dump() + "\n"
		"right:" + _right->dump() + "}";
}

void DivNode::setLeft(ASTNode* left)
{
	_left = left;
}

ASTNode* DivNode::left()
{
	return _left;
}

void DivNode::setRight(ASTNode* right)
{
	_right = right;
}

ASTNode* DivNode::right()
{
	return _right;
}

DivNode::~DivNode()
{
	delete _left;
	delete _right;
}

XString ClassDeclarationNode::dump() const
{
	return "Class{name:"+_name+"\n " + "}";
}

XString DefinitionsNode::dump() const
{
	return XString();
}

DefinitionsNode::~DefinitionsNode()
{
	for (auto p : _variableDeclarations)delete p;
	for (auto p : _functionDeclarations)delete p;
	for (auto p : _classDeclarations)delete p;
}

XString FunctionDeclarationNode::dump() const
{
	return XString();
}

void FunctionDeclarationNode::setName(const XString& name)
{
	_name = name;
}

XString FunctionDeclarationNode::name() const
{
	return _name;
}

void FunctionDeclarationNode::setReturnType(XString returnType)
{
	_returnType = returnType;
}

XString FunctionDeclarationNode::returnType() const
{
	return _returnType;
}

void FunctionDeclarationNode::setParams(std::vector<std::pair<XString, XString>> params)
{
	_params = params;
}

std::vector<std::pair<XString, XString>> FunctionDeclarationNode::params() const
{
	return _params;
}

void FunctionDeclarationNode::setImpl(BlockNode* impl)
{
	_impl = impl;
}

BlockNode* FunctionDeclarationNode::impl() const
{
	return _impl;
}

XString VariableDeclarationNode::dump() const
{
	return XString();
}

XString BlockNode::dump() const
{
	return XString();
}

BlockNode::~BlockNode()
{
	for (auto p : _statements)delete p;
}
