#include "ASTNodes.h"

IntegerNode::IntegerNode(int64_t value)
:_value(value)
{
}

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

DecimalFractionNode::DecimalFractionNode(double value)
	:_value(value)
{
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

BooleanNode::BooleanNode(bool value)
	:_value(value)
{

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

StringNode::StringNode(const XString& value)
:_value(value)
{
}

XString StringNode::dump() const
{
	return "\"" + _value + "\"";
}

void StringNode::setValue(XString value)
{
	_value = value;
}

XString StringNode::value() const
{
	return _value;
}


XString BinaryOperatorNode::dump() const
{
	return _operatorStr+" { left:" + _left->dump() + "\n"
		"right:" + _right->dump() + "}";
}

void BinaryOperatorNode::setLeft(ASTNode* left)
{
	_left = left;
}

ASTNode* BinaryOperatorNode::left()
{
	return _left;
}

void BinaryOperatorNode::setRight(ASTNode* right)
{
	_right = right;
}

ASTNode* BinaryOperatorNode::right()
{
	return _right;
}

void BinaryOperatorNode::setOperatorStr(const XString& operatorStr)
{
	_operatorStr = operatorStr;
}

XString BinaryOperatorNode::operatorStr() const
{
	return _operatorStr;
}

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

std::vector<FunctionDeclarationNode*> DefinitionsNode::functionDeclarations() const
{
	return _functionDeclarations;
}

std::vector<VariableDeclarationNode*> DefinitionsNode::variableDeclarations() const
{
	return _variableDeclarations;
}

DefinitionsNode::~DefinitionsNode()
{
	for (auto p : _variableDeclarations)delete p;
	for (auto p : _functionDeclarations)delete p;
	for (auto p : _classDeclarations)delete p;
}

FunctionDeclarationNode::FunctionDeclarationNode()
	:_impl(nullptr)
{
}

XString FunctionDeclarationNode::dump() const
{
	XString paramsDump;
	for (std::pair<XString, XString> param : _params) {
		paramsDump.append(param.first).append(' ').append(param.second).append("\n");
	}

	XString implDump = "no";
	if (_impl) {
		implDump = _impl->dump();
	}
	return "Function{name:" + _name + "\nreturnType:" + _returnType + "\nparams:{" + paramsDump + "}\nblock:{" + implDump + "}\n}\n";
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

void FunctionDeclarationNode::addParam(const std::pair<XString, XString>& param)
{
	_params.push_back(param);
}

std::vector<std::pair<XString, XString>> FunctionDeclarationNode::params() const
{
	return _params;
}

BlockNode* FunctionDeclarationNode::impl() const
{
	return _impl;
}

void FunctionDeclarationNode::setImpl(BlockNode* impl)
{
	_impl = impl;
}

FunctionDeclarationNode::~FunctionDeclarationNode()
{
	delete _impl;
}


VariableDeclarationNode::VariableDeclarationNode()
:_initValue(nullptr)
{
}

XString VariableDeclarationNode::dump() const
{
	XString initDump = "No";
	if (_initValue) {
		initDump = _initValue->dump();
	}
	return "Variable{name:" + _name + "\ntype:" + _type + "\ninitValue:" + initDump + "}\n";
}

void VariableDeclarationNode::setType(const XString& type)
{
	_type = type;
}

XString VariableDeclarationNode::type() const
{
	return _type;
}

void VariableDeclarationNode::setName(const XString& name)
{
	_name = name;
}

XString VariableDeclarationNode::name() const
{
	return _name;
}

void VariableDeclarationNode::setInitValue(ASTNode* initValue)
{
	_initValue = initValue;
}

ASTNode* VariableDeclarationNode::initValue() const
{
	return _initValue;
}

VariableDeclarationNode::~VariableDeclarationNode()
{
	delete _initValue;
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

void BlockNode::addContent(ASTNode* content)
{
	_contents.push_back(content);
}

void BlockNode::setContents(std::vector<ASTNode*> contents)
{
	_contents = contents;
}

std::vector<ASTNode*> BlockNode::contents() const
{
	return _contents;
}

BlockNode::~BlockNode()
{
	for (auto p : _contents)delete p;
}

XString FunctionCallNode::dump() const
{
	XString paramsDump;
	for (auto param : _params) {
		paramsDump.append(param->dump()).append(',');
	}
	
	return "FunctionCall{name:" + _name +  "\nparams:{" + paramsDump+ "}\n}";
}

void FunctionCallNode::setName(const XString& name)
{
	_name = name;
}

XString FunctionCallNode::name() const
{
	return _name;
}

void FunctionCallNode::setParams(std::vector<ASTNode*> params)
{
	_params = params;
}

void FunctionCallNode::addParam(ASTNode* param)
{
	_params.push_back(param);
}

std::vector<ASTNode*> FunctionCallNode::params() const
{
	return _params;
}

FunctionCallNode::~FunctionCallNode()
{
	for (auto i : _params)delete i;
}

XString MemberFunctionCallNode::dump() const
{
	XString paramsDump;
	for (auto param : _params) {
		paramsDump.append(param->dump());
	}
	return "FunctionCall{name:" + _name +"\nobject:"+ _object->dump() + "\nparams:{" + paramsDump + "}\n}";
}

void MemberFunctionCallNode::setName(const XString& name)
{
	_name = name;
}

XString MemberFunctionCallNode::name() const
{
	return _name;
}

void MemberFunctionCallNode::setObject(ASTNode* object)
{
	_object = object;
}

ASTNode* MemberFunctionCallNode::object() const
{
	return _object;
}

void MemberFunctionCallNode::setParams(std::vector<ASTNode*> params)
{
	_params = params;
}

void MemberFunctionCallNode::addParam(ASTNode* param)
{
	_params.push_back(param);
}

std::vector<ASTNode*> MemberFunctionCallNode::params() const
{
	return _params;
}

MemberFunctionCallNode::~MemberFunctionCallNode()
{
	for (auto i : _params)delete i;
	delete _object;
}

XString UnaryOperatorNode::dump() const
{
	return _operatorStr + " { value:" + _value->dump() + "\n}";
}

void UnaryOperatorNode::setValue(ASTNode* value)
{
	_value = value;
}

ASTNode* UnaryOperatorNode::value()
{
	return _value;
}

void UnaryOperatorNode::setOperatorStr(const XString& operatorStr)
{
	_operatorStr = operatorStr;
}

XString UnaryOperatorNode::operatorStr() const
{
	return _operatorStr;
}

UnaryOperatorNode::~UnaryOperatorNode()
{
	delete _value;
}

VariableNode::VariableNode(const XString name)
	:_name(name)
{
}

XString VariableNode::dump() const
{
	return "Variable{name:"+_name+"}\n";
}

void VariableNode::setName(const XString& name)
{
	_name = name;
}

XString VariableNode::name() const
{
	return _name;
}
