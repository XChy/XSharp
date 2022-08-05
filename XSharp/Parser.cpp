#include "Parser.h"

ASTNode* Parser::parse(const std::vector<Token>& tokenList)
{
    current = tokenList.begin();
	end = tokenList.end();
	return definitions();
}

ASTNode* Parser::definitions()
{
	DefinitionsNode* root = new DefinitionsNode;
	return root;
}

void Parser::forward()
{
	if (current != end)
		current++;
	else
		throw XSharpError("Reach the end without completing parsing");
}
