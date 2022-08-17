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
	else if(current->value=="=") {
		forward();
		root->setInitValue(expression());
	}
	else {
		throw XSharpError("variable defintion error");
	}
	return root;
}

std::vector<ASTNode*> Parser::paramsDefinition()
{
	if (current->type == OpenParenthesis) {
		forward();
		while (true){

		}
	}
	return std::vector<ASTNode*>();
}

BlockNode* Parser::block()
{
	BlockNode* root = new BlockNode;
	if (current->type == OpenBrace) {
		forward();
		while (current->type!=CloseBrace)
		{
			if (current->type == OpenBrace) {
				//addblock();
			}
			else if(current==end){
				throw XSharpError("No '}' matched");
			}
			else {
				//addstatement();
			}

		}
		forward();
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
		break;
	}
	return nullptr;
}

ASTNode* Parser::expression()
{
	return nullptr;
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
