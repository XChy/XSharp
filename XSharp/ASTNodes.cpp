#include "ASTNodes.h"

XString IntegerNode::dump()
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

XString DecimalFractionNode::dump()
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

XString BooleanNode::dump()
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

XString StringNode::dump()
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
