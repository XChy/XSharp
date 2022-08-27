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
	ASTNode* root;
	if (factorBegin->type == Operator) {
		UnaryOperatorNode* root = new UnaryOperatorNode;
		root->setOperatorStr(factorBegin->value);
		factorBegin++;
		if (factorBegin == end) {
			throw XSharpError("No factor matched after a unary operator");
		}
		root->setValue(factor(factorBegin));
		return root;
	}
	else
	{
		return factor(factorBegin);
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
