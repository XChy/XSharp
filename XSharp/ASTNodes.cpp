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

XString FunctionDeclarationNode::dump() const
{
	return XString();
}

XString VariableDeclarationNode::dump() const
{
	return XString();
}

XString BlockNode::dump() const
{
	return XString();
}
