#pragma once

#include "xsharp_global.h"
#include "ASTNodes.h"
#include "Tokens.h"
#include "XSharpUtils.h"
#include "OperInfo.h"

class XSharp_EXPORT Parser {
public:
	typedef std::vector<Token>::const_iterator Iterator;
	ASTNode* parse(const std::vector<Token>& tokenList);//The astnode pointer is managed by caller
private:
	DefinitionsNode* definitions();
	ClassDeclarationNode* classDeclaration();
	FunctionDeclarationNode* functionDeclaration();
	VariableDeclarationNode* variableDeclaration();

	std::vector<std::pair<TypeInfo, XString>> paramsDefinition();
	std::vector<ASTNode*> paramsList(Iterator paramsBegin, Iterator paramsEnd);

	BlockNode* block();
	ASTNode* statement();
	ASTNode* expression(Iterator exprBegin, Iterator exprEnd);
	ASTNode* operand(Iterator& factorBegin);

	TypeInfo type();

	Iterator nextSentenceEnd(Iterator begin) const;
	Iterator nextCloseParenthesis(Iterator begin) const;
	Iterator nextCloseBracket(Iterator begin) const;
	static std::vector<Iterator> findFunctionCommas(Iterator begin, Iterator end);
	static int priority(BinaryOperatorNode* oper);
	static int priority(UnaryOperatorNode* oper);
	static Assoc assoc(BinaryOperatorNode* oper);

	void forward();
	void backward();
	Iterator current;
	Iterator end;
};