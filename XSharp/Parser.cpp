#include "Parser.h"

ASTNode* Parser::parse(const std::vector<Token>& tokenList)
{
	current = tokenList.begin();
	end = tokenList.end();
	return definitions();
}

DefinitionsNode* Parser::definitions()
{
	DefinitionsNode* root = new DefinitionsNode;
	while (current != end) {
		if (current->type == Keyword) {
			if (current->value == "class") {
				root->addClass(classDeclaration());
			}
		}
		else if (current->type == Identifier)
		{
			forward();
			if (current->type == Identifier) {//Function or variable
				forward();
				if (current->type == OpenParenthesis) {//define function
					backward();
					backward();
					root->addFunction(functionDeclaration());
				}
				else if ((current->type == SentenceEnd)
					|| (current->type == Operator && current->value == "=")) {//define variable
					backward();
					backward();
					root->addVariable(variableDeclaration());
				}
				else {

				}
			}
			else {
				throw XSharpError("Illegal identifer");
			}
		}
	}
	return root;
}

ClassDeclarationNode* Parser::classDeclaration()
{
	return nullptr;
}

FunctionDeclarationNode* Parser::functionDeclaration()
{
	FunctionDeclarationNode* root = new FunctionDeclarationNode;

	root->setReturnType(current->value);
	forward();

	root->setName(current->value);
	forward();

	root->setParams(paramsDefinition());

	root->setImpl(block());

	return root;
}

VariableDeclarationNode* Parser::variableDeclaration()
{
	VariableDeclarationNode* root = new VariableDeclarationNode;
	root->setType(current->value);
	forward();

	root->setName(current->value);
	forward();

	if (current->type == SentenceEnd) {
		root->setInitValue(nullptr);
		forward();
	}
	else if (current->value == "=") {
		forward();
		root->setInitValue(expression(current, nextSentenceEnd(current)+1));
	}
	else {
		throw XSharpError("variable defintion error");
	}
	return root;
}

std::vector<std::pair<XString, XString>> Parser::paramsDefinition()
{
	std::vector<std::pair<XString, XString>> paramsDef;
	if (current->type == OpenParenthesis) {
		forward();
		while (current->type != CloseParenthesis) {
			if (current == end) {
				throw XSharpError("No ')' matched");
			}
			else {
				std::pair<XString, XString> paramDef;
				if (current->type == Identifier) {
					paramDef.first = current->value;
					forward();
				}
				else {
					throw XSharpError("No typename matched");
				}

				if (current->type == Identifier) {
					paramDef.second = current->value;
					forward();
				}
				else {
					throw XSharpError("No paramname matched");
				}

				if (current->type == Comma) {
					forward();
					if (current->type == CloseParenthesis) {
						throw XSharpError("A param is missing between ',' and ')'");
					}
				}

				paramsDef.push_back(paramDef);
			}
		}
		forward();
	}
	return paramsDef;
}

std::vector<ASTNode*> Parser::params(Iterator paramsBegin, Iterator paramsEnd)
{
	return std::vector<ASTNode*>();
}

std::vector<Parser::Iterator> findFunctionComma(Parser::Iterator begin, Parser::Iterator end) const
{
	std::vector<Parser::Iterator> result;
	int numOpenParentheses = 0;
	for (auto it = begin; it != end; ++it) {
		if (it->type == OpenParenthesis) {
			++numOpenParentheses;
		}
		else if (it->type== CloseParenthesis) {
			--numOpenParentheses;
		}
		if (it->type == Comma) {
			if (!numOpenParentheses) {
				result.push_back(it);
			}
		}
	}
	return result;
}

BlockNode* Parser::block()
{
	BlockNode* root = new BlockNode;
	if (current->type == OpenBrace) {
		forward();
		while (current->type != CloseBrace)
		{
			if (current->type == OpenBrace) {
				root->addContent(block());
			}
			else if (current == end) {
				throw XSharpError("No '}' matched");
			}
			else {
				ASTNode* stmt = statement();
				if (stmt) {
					root->addContent(stmt);
				}
			}

		}
		forward();
	}
	else {
		throw XSharpError("No '{' matched");
	}
	return root;
}

ASTNode* Parser::statement()
{
	switch (current->type)
	{
	case Keyword:

		break;
	default:
		ASTNode* expr = expression(current, nextSentenceEnd(current));//current is ";",need to forward to next statement
		forward();
		return expr;
	}
}

ASTNode* Parser::expression(Iterator exprBegin, Iterator exprEnd)
{
	if (exprBegin == exprEnd) { return nullptr; }

	ASTNode* root = nullptr;

	ASTNode* factor1 = factor(exprBegin);
	BinaryOperatorNode* oper1 = nullptr;
	ASTNode* factor2 = nullptr;
	BinaryOperatorNode* oper2=nullptr;
	ASTNode* factor3 = nullptr;

	if (exprBegin == exprEnd) {
		return factor1;
	}

	while (exprBegin!=exprEnd)
	{
		
	}

	current = exprEnd;
	return nullptr;
}

ASTNode* Parser::factor(Iterator& factorBegin)
{
	UnaryOperatorNode* before = nullptr;
	UnaryOperatorNode* after = nullptr;
	ASTNode* operand=nullptr;
	if (factorBegin->type == Operator) {
		before = new UnaryOperatorNode;
		before->setOperatorStr(factorBegin->value);

		factorBegin++;
	}

	if (factorBegin->type == Integer) {
		operand = new IntegerNode(factorBegin->value.toInteger<int64_t>());
	}
	else if (factorBegin->type == DecimalFraction) {
		operand = new DecimalFractionNode(factorBegin->value.toDouble());
	}
	else if (factorBegin->type == Boolean) {
		operand = new BooleanNode(factorBegin->value == "true");
	}
	else if (factorBegin->type == String) {
		operand = new StringNode(factorBegin->value);
	}
	else if (factorBegin->type == OpenParenthesis) {
		auto nextPar = nextCloseParenthesis(factorBegin + 1);
		operand = expression(factorBegin + 1, nextPar);
		factorBegin = nextPar;
	}
	else if (factorBegin->type == Identifier) {
		if ((factorBegin + 1)->type == OpenParenthesis) {
			auto functionEnd = nextCloseParenthesis(factorBegin + 2);
			FunctionCallNode* temp = new FunctionCallNode;
			temp->setName(factorBegin->value);
			temp->setParams(params(factorBegin, functionEnd));
			operand = temp;
			factorBegin = functionEnd;
		}
		else {
			operand = new VariableNode(factorBegin->value);
		}
	}
	else {
		delete before;
		throw XSharpError("Not a factor");
	}

	factorBegin++;

	if (factorBegin->type == Operator) {
		if ((factorBegin + 1)->type != Integer && (factorBegin + 1)->type != DecimalFraction
			&& (factorBegin + 1)->type != Boolean && (factorBegin + 1)->type != String
			&& (factorBegin + 1)->type != OpenParenthesis) {
			after = new UnaryOperatorNode;
			after->setOperatorStr(factorBegin->value);
			factorBegin++;
		}
	}

	if (!(before || after)) {
		return operand;
	}
	else if(before && !after) {
		before->setValue(operand);
		return before;
	}
	else if (!before && after) {
		after->setValue(operand);
		return after;
	}else {
		using XSharp::UnaryOperInfo;
		if (UnaryOperInfo[before->operatorStr()].priority < UnaryOperInfo[before->operatorStr()].priority) {
			before->setValue(operand);
			after->setValue(before);
			return after;
		}
		else {
			after->setValue(operand);
			before->setValue(after);
			return before;
		}
	}
}

Parser::Iterator Parser::nextSentenceEnd(Iterator factorBegin) const
{
	for(; factorBegin != end; ++factorBegin){
		if (factorBegin->type == SentenceEnd) {
			return factorBegin;
		}
	}
	throw XSharpError("';' is missing");
}

Parser::Iterator Parser::nextCloseParenthesis(Iterator begin) const
{
	int numOpenParenthesis = 1;
	for (auto it = begin; it != end; ++it) {
		if (it->type == OpenParenthesis) {
			++numOpenParenthesis;
		}
		else if (it->type == CloseParenthesis) {
			--numOpenParenthesis;
		}
		if (!numOpenParenthesis) {
			return it;
		}
	}

	throw XSharpError("')' is missing");
}

void Parser::forward()
{
	if (current != end)
		current++;
	else
		throw XSharpError("Reach the end without completing parsing");
}

void Parser::backward()
{
	current--;
}
