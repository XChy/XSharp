#pragma once

#include "xsharp_global.h"
#include "ASTNodes.h"
#include "Tokens.h"
#include "XSharpUtils.h"

class Parser {
public:
	typedef std::vector<Token>::const_iterator Iterator;
	ASTNode* parse(const std::vector<Token>& tokenList);//The astnode pointer is managed by caller
private:
	DefinitionsNode* definitions();
	ClassDeclarationNode* classDeclaration();
	FunctionDeclarationNode* functionDeclaration();
	VariableDeclarationNode* variableDeclaration();

	std::vector<std::pair<XString, XString>> paramsDefinition();

	BlockNode* block();
	ASTNode* statement();
	ASTNode* expression(Iterator exprBegin,Iterator exprEnd);

	Iterator nextSentenceEnd(Iterator begin) const;
	Iterator nextCloseParenthesis(Iterator begin) const;

	void forward();
	void backward();
	Iterator current;
	Iterator end;
};