#include "Parser.h"

ASTNode* Parser::parse(const std::vector<Token>& tokenList)
{
    current = tokenList.begin();
	return definitions();
}

ASTNode* Parser::definitions()
{
	return nullptr;
}
