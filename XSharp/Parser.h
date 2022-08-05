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
	ASTNode* definitions();
	void forward();
	Iterator current;
	Iterator end;
};