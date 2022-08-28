#pragma once

#include "xsharp_global.h"
#include "ASTNodes.h"
#include "Tokens.h"
#include "XSharpUtils.h"
#include "OperInfo.h"

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
	std::vector<ASTNode*> params(Iterator paramsBegin,Iterator paramsEnd);

	BlockNode* block();
	ASTNode* statement();
	ASTNode* expression(Iterator exprBegin,Iterator exprEnd);
	ASTNode* factor(Iterator& factorBegin);

	Iterator nextSentenceEnd(Iterator begin) const;
	Iterator nextCloseParenthesis(Iterator begin) const;
	std::vector<Iterator> findFunctionComma(Iterator begin, Iterator end) const;

	void forward();
	void backward();
	Iterator current;
	Iterator end;
};