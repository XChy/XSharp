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
	root->setType(type());

	if (current->type == Identifier) {
		root->setName(current->value);
		forward();
	}
	else {
		throw XSharpError("No name after a typename");
	}

	if (current->type == SentenceEnd) {
		root->setInitValue(nullptr);
		forward();
	}
	else if (current->value == "=") {
		forward();
		auto nextEnd = nextSentenceEnd(current);
		root->setInitValue(expression(current, nextEnd));
		current = nextEnd + 1;
	}
	else {
		throw XSharpError("variable defintion error");
	}
	return root;
}

std::vector<std::pair<TypeInfo, XString>> Parser::paramsDefinition()
{
	std::vector<std::pair<TypeInfo, XString>> paramsDef;
	if (current->type == OpenParenthesis) {
		forward();
		while (current->type != CloseParenthesis) {
			if (current == end) {
				throw XSharpError("No ')' matched");
			}
			else {
				std::pair<TypeInfo, XString> paramDef;
				paramDef.first = type();

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

std::vector<ASTNode*> Parser::paramsList(Iterator paramsBegin, Iterator paramsEnd)
{
	if (paramsBegin == paramsEnd)return std::vector<ASTNode*>();

	auto commas = findFunctionCommas(paramsBegin, paramsEnd);

	if (commas.size() == 0) {
		return { expression(paramsBegin,paramsEnd) };
	}
	else {
		std::vector<ASTNode*> _params;
		_params.reserve(commas.size() + 1);
		_params.push_back(expression(paramsBegin, commas[0]));

		for (int i = 0; i < commas.size() - 1; ++i) {
			_params.push_back(expression(commas[i] + 1, commas[i + 1]));
		}

		_params.push_back(expression(commas[commas.size() - 1] + 1, paramsEnd));
		return _params;
	}
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
	ASTNode* stmt = nullptr;
	auto end = nextSentenceEnd(current);
	switch (current->type)
	{
	case Keyword:
		if (current->value == "const") {
			stmt = variableDeclaration();
		}
		break;
	case Identifier:
		if ((current + 1)->type == OpenBracket && (current + 2)->type == CloseBracket) {
			stmt = variableDeclaration();
		}
		else if ((current + 1)->type == Identifier) {
			stmt = variableDeclaration();
		}
		break;
	default:
		ASTNode* expr = expression(current, end);//current is ";",need to forward to next statement
	}
	current = end + 1;
	return stmt;
}

ASTNode* Parser::expression(Iterator exprBegin, Iterator exprEnd)
{
	if (exprBegin == exprEnd) { return nullptr; }

	BinaryOperatorNode* root = nullptr;

	ASTNode* factor1 = nullptr;
	BinaryOperatorNode* oper2 = nullptr;
	ASTNode* factor3 = nullptr;

	factor1 = operand(exprBegin);

	if (exprBegin == exprEnd)return factor1;

	if (exprBegin->type == Operator) {
		root = new BinaryOperatorNode;
		root->setOperatorStr(exprBegin->value);
		root->setLeft(factor1);
	}
	else {
		throw XSharpError("No operator matched after operand");
	}

	if (++exprBegin == exprEnd)throw XSharpError("No operand after operator");

	root->setRight(operand(exprBegin));

	while (exprBegin != exprEnd) {
		oper2 = new BinaryOperatorNode;
		oper2->setOperatorStr(exprBegin->value);
		if (++exprBegin == exprEnd)throw XSharpError("No operand after operator");

		factor3 = operand(exprBegin);

		if (priority(oper2) > priority(root)) {
			oper2->setLeft(root);
			oper2->setRight(factor3);
			root = oper2;
		}
		else if (priority(oper2) == priority(root)) {
			if (assoc(oper2) == LeftToRight) {
				oper2->setLeft(root);
				oper2->setRight(factor3);
				root = oper2;
			}
			else if (assoc(oper2) == RightToLeft) {
				oper2->setLeft(root->right());
				oper2->setRight(factor3);
				root->setRight(oper2);
			}
		}
		else {
			BinaryOperatorNode* node = root;
			while (true) {
				if (!node->right()->is<BinaryOperatorNode>()) {
					oper2->setLeft(node->right());
					oper2->setRight(factor3);
					node->setRight(oper2);
					break;
				}

				BinaryOperatorNode* currentNode = (BinaryOperatorNode*)node->right();

				if (priority(oper2) > priority(currentNode)) {
					oper2->setLeft(currentNode);
					oper2->setRight(factor3);
					node->setRight(oper2);
					break;
				}
				else if (priority(oper2) == priority(currentNode)) {
					if (assoc(oper2) == LeftToRight) {
						oper2->setLeft(currentNode);
						oper2->setRight(factor3);
						node->setRight(oper2);
					}
					else if (assoc(oper2) == RightToLeft) {
						oper2->setLeft(currentNode->right());
						oper2->setRight(factor3);
						currentNode->setRight(oper2);
					}
					break;
				}
				node = currentNode;
			}
		}
	}
	return root;
}

ASTNode* Parser::operand(Iterator& factorBegin)
{
	UnaryOperatorNode* before = nullptr;
	UnaryOperatorNode* after = nullptr;
	ASTNode* operand = nullptr;
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
		operand = new BoxNode(expression(factorBegin + 1, nextPar));
		factorBegin = nextPar;
	}
	else if (factorBegin->type == Identifier) {
		operand = new VariableNode(factorBegin->value);
	}
	else {
		delete before;
		throw XSharpError("Not a operand");
	}

	factorBegin++;

	while (true)
	{
		if (factorBegin->type == Dot) {
			factorBegin++;
			if (factorBegin->type == Identifier) {
				MemberNode* member = new MemberNode(factorBegin->value);
				member->setObject(operand);
				operand = member;
			}
			else {
				throw XSharpError("No member matched with '.'");
			}

		}
		else if (factorBegin->type == OpenParenthesis) {
			factorBegin++;
			auto nextPar = nextCloseParenthesis(factorBegin);
			FunctionCallNode* funcCall = new FunctionCallNode;
			funcCall->setParams(paramsList(factorBegin, nextPar));
			funcCall->setFunction(operand);
			operand = funcCall;
			factorBegin = nextPar;
		}
		else if (factorBegin->type == OpenBracket) {
			factorBegin++;
			auto nextBra = nextCloseBracket(factorBegin);
			IndexNode* index = new IndexNode;
			index->setOperand(operand);
			index->setIndexExpr(expression(factorBegin, nextBra));
			operand = index;
			factorBegin = nextBra;
		}
		else {
			break;
		}
		factorBegin++;
	}

	if (factorBegin->type == Operator) {
		if ((factorBegin + 1)->type != Integer && (factorBegin + 1)->type != DecimalFraction
			&& (factorBegin + 1)->type != Boolean && (factorBegin + 1)->type != String
			&& (factorBegin + 1)->type != OpenParenthesis && (factorBegin + 1)->type != Identifier) {
			after = new UnaryOperatorNode;
			after->setOperatorStr(factorBegin->value);
			factorBegin++;
		}
	}

	if (!(before || after)) {
		return operand;
	}
	else if (before && !after) {
		before->setOperand(operand);
		return before;
	}
	else if (!before && after) {
		after->setOperand(operand);
		return after;
	}
	else {
		if (priority(before) < priority(after)) {
			before->setOperand(operand);
			after->setOperand(before);
			return after;
		}
		else {
			after->setOperand(operand);
			before->setOperand(after);
			return before;
		}
	}
}

TypeInfo Parser::type()
{
	TypeInfo info;
	if (current->type == Keyword && current->value == "const") {
		info.isConst = true;
		forward();
	}

	if (current->type == Identifier) {
		info.typeName = current->value;
		forward();

		int arrayDimension = 0;
		while (current->type == OpenBracket)
		{
			forward();
			if (current->type == CloseBracket) {
				arrayDimension++;
				forward();
			}
		}
		info.arrayDimension = arrayDimension;
	}
	else {
		throw XSharpError("No typename matched");
	}
	return info;
}

Parser::Iterator Parser::nextSentenceEnd(Iterator begin) const
{
	int numUnmatchedBrace = 0;
	for (; begin != end; ++begin) {
		if (begin->type == OpenBrace) {
			++numUnmatchedBrace;
		}
		else if (begin->type == CloseBrace) {
			--numUnmatchedBrace;
		}
		if (begin->type == SentenceEnd && !numUnmatchedBrace) {
			return begin;
		}
	}
	throw XSharpError("';' is missing");
}

Parser::Iterator Parser::nextCloseParenthesis(Iterator begin) const
{
	int numUnmatchedParenthesis = 1;
	for (auto it = begin; it != end; ++it) {
		if (it->type == OpenParenthesis) {
			++numUnmatchedParenthesis;
		}
		else if (it->type == CloseParenthesis) {
			--numUnmatchedParenthesis;
		}
		if (!numUnmatchedParenthesis) {
			return it;
		}
	}

	throw XSharpError("')' is missing");
}

Parser::Iterator Parser::nextCloseBracket(Iterator begin) const
{
	int numUnmatchedBracket = 1;
	for (auto it = begin; it != end; ++it) {
		if (it->type == OpenBracket) {
			++numUnmatchedBracket;
		}
		else if (it->type == CloseBracket) {
			--numUnmatchedBracket;
		}
		if (!numUnmatchedBracket) {
			return it;
		}
	}

	throw XSharpError("']' is missing");
}

std::vector<Parser::Iterator> Parser::findFunctionCommas(Iterator begin, Iterator end)
{
	std::vector<Parser::Iterator> result;
	int numOpenParentheses = 0;
	int numOpenBrackets = 0;
	int numOpenBraces = 0;

	for (auto it = begin; it != end; ++it) {
		if (it->type == OpenParenthesis) {
			++numOpenParentheses;
		}
		else if (it->type == CloseParenthesis) {
			--numOpenParentheses;
		}
		if (it->type == OpenBrace) {
			++numOpenBraces;
		}
		else if (it->type == CloseBrace) {
			--numOpenBraces;
		}
		if (it->type == OpenBracket) {
			++numOpenBrackets;
		}
		else if (it->type == CloseBracket) {
			--numOpenBrackets;
		}
		if (it->type == Comma) {
			if (!numOpenParentheses && !numOpenBraces && !numOpenBrackets) {
				result.push_back(it);
			}
		}
	}
	return result;
}

int Parser::priority(BinaryOperatorNode* oper)
{
	return XSharp::binaryOperInfo[oper->operatorStr()].priority;
}

int Parser::priority(UnaryOperatorNode* oper)
{
	return XSharp::unaryOperInfo[oper->operatorStr()].priority;;
}

Assoc Parser::assoc(BinaryOperatorNode* oper)
{
	return XSharp::binaryOperInfo[oper->operatorStr()].assoc;
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
