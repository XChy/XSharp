#include "Parser.h"
#include <algorithm>
#include <vector>
#include "XSharp/ASTNodes.h"
#include "XSharp/Tokens.h"
#include "XSharp/Type.h"
#include "XSharp/XSharpUtils.h"
#include "XSharp/XString.h"

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
        } else if (current->type == Identifier) {
            forward();
            // TODO: Identify the variable and function when parsing
            if (current->type == Identifier) {  // Function or variable
                forward();
                if (current->type == OpenParenthesis) {  // define function
                    backward();
                    backward();
                    root->addFunction(functionDeclaration());
                } else if ((current->type == SentenceEnd) ||
                           (current->type == Operator &&
                            current->value == "=")) {  // define variable
                    backward();
                    backward();
                    root->addVariable(variableDeclaration({SentenceEnd}));
                } else {
                }
            } else {
                throw XSharpError("Illegal identifer");
            }
        }
    }
    return root;
}

ClassDeclarationNode* Parser::classDeclaration() { return nullptr; }

FunctionDeclarationNode* Parser::functionDeclaration()
{
    FunctionDeclarationNode* root = new FunctionDeclarationNode;

    root->setReturnType(type());

    root->setName(current->value);
    forward();

    // start with '('
    if (current->type != OpenParenthesis) {
        throw XSharpError("No '(' matched");
    }
    forward();

    root->setParams(paramsDefinition());
    // end with ')', so skip ')'
    forward();

    root->setImpl(block());
    return root;
}

VariableDeclarationNode* Parser::variableDeclaration(
    const std::vector<TokenType>& stopwords)
{
    VariableDeclarationNode* root = new VariableDeclarationNode;
    root->setType(type());

    if (current->type == Identifier) {
        root->setName(current->value);
        forward();
    } else {
        throw XSharpError("No name after a typename");
    }

    if (isStopwords(current, stopwords)) {
        root->setInitValue(nullptr);
        forward();
    } else if (current->type == Operator && current->value == "=") {
        forward();
        root->setInitValue(expression(stopwords));
        forward();
    } else {
        throw XSharpError("variable defintion error");
    }
    return root;
}

std::vector<VariableDeclarationNode*> Parser::paramsDefinition()
{
    std::vector<VariableDeclarationNode*> paramsDef;

    // if no parameter in parentheses, then return empty paramsDef
    if (current->type == CloseParenthesis) return paramsDef;

    while (true) {
        paramsDef.push_back(variableDeclaration({Comma, CloseParenthesis}));
        backward();

        if (current->type == CloseParenthesis)
            break;
        else if (current->type == Comma)
            forward();
        else
            throw XSharpError("')' expected is missing");
    }
    return paramsDef;
}

std::vector<ASTNode*> Parser::paramsList()
{
    std::vector<ASTNode*> results;
    if (current->type == CloseParenthesis) return results;

    while (true) {
        results.push_back(expression({CloseParenthesis, Comma}));
        if (current->type == CloseParenthesis)
            break;
        else if (current->type == Comma)
            current++;
    }
    return results;
}

BlockNode* Parser::block()
{
    BlockNode* root = new BlockNode;
    if (current->type == OpenBrace) {
        forward();
        while (current->type != CloseBrace) {
            if (current->type == OpenBrace) {
                root->addContent(block());
            } else if (current == end) {
                throw XSharpError("No '}' matched");
            } else {
                ASTNode* stmt = statement();
                if (stmt) {
                    root->addContent(stmt);
                }
            }
        }
        forward();
    } else {
        throw XSharpError("No '{' matched");
    }
    return root;
}

ASTNode* Parser::statement()
{
    ASTNode* stmt = nullptr;
    switch (current->type) {
        case Keyword:
            if (current->value == "const") {
                stmt = variableDeclaration({SentenceEnd});
            } else if (current->value == "return") {
                forward();
                stmt = new ReturnNode(expression({SentenceEnd}));
                forward();
            } else if (current->value == "if") {
            }
            break;
        case Identifier:
            if ((current + 1)->type == OpenBracket &&
                (current + 2)->type == CloseBracket) {
                stmt = variableDeclaration({SentenceEnd});
            } else if ((current + 1)->type == Identifier) {
                stmt = variableDeclaration({SentenceEnd});
            } else {
                stmt = expression({SentenceEnd});
                forward();
            }
            break;
        default:
            stmt = expression({SentenceEnd});
            forward();  // current is ";",need to forward
    }
    return stmt;
}

ASTNode* Parser::expression(std::vector<TokenType> stopwords)
{
    if (isStopwords(current, stopwords)) {
        return nullptr;
    }

    BinaryOperatorNode* root = nullptr;

    ASTNode* factor1 = nullptr;
    BinaryOperatorNode* oper2 = nullptr;
    ASTNode* factor3 = nullptr;

    factor1 = operand();

    if (isStopwords(current, stopwords)) return factor1;

    if (current->type == Operator) {
        root = new BinaryOperatorNode;
        root->setOperatorStr(current->value);
        root->setLeft(factor1);
    } else {
        throw XSharpError("No operator matched after operand");
    }

    if (isStopwords(++current, stopwords))
        throw XSharpError("No operand after operator");

    root->setRight(operand());

    while (!isStopwords(current, stopwords)) {
        oper2 = new BinaryOperatorNode;
        oper2->setOperatorStr(current->value);
        forward();

        if (isStopwords(current, stopwords))
            throw XSharpError("No operand after operator");

        factor3 = operand();

        if (priority(oper2) > priority(root)) {
            oper2->setLeft(root);
            oper2->setRight(factor3);
            root = oper2;
        } else if (priority(oper2) == priority(root)) {
            if (assoc(oper2) == LeftToRight) {
                oper2->setLeft(root);
                oper2->setRight(factor3);
                root = oper2;
            } else if (assoc(oper2) == RightToLeft) {
                oper2->setLeft(root->right());
                oper2->setRight(factor3);
                root->setRight(oper2);
            }
        } else {
            BinaryOperatorNode* node = root;
            while (true) {
                if (!node->right()->is<BinaryOperatorNode>()) {
                    oper2->setLeft(node->right());
                    oper2->setRight(factor3);
                    node->setRight(oper2);
                    break;
                }

                BinaryOperatorNode* currentNode =
                    (BinaryOperatorNode*)node->right();

                if (priority(oper2) > priority(currentNode)) {
                    oper2->setLeft(currentNode);
                    oper2->setRight(factor3);
                    node->setRight(oper2);
                    break;
                } else if (priority(oper2) == priority(currentNode)) {
                    if (assoc(oper2) == LeftToRight) {
                        oper2->setLeft(currentNode);
                        oper2->setRight(factor3);
                        node->setRight(oper2);
                    } else if (assoc(oper2) == RightToLeft) {
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

ASTNode* Parser::operand()
{
    UnaryOperatorNode* before = nullptr;
    UnaryOperatorNode* after = nullptr;
    ASTNode* operand = nullptr;
    if (current->type == Operator) {
        before = new UnaryOperatorNode;
        before->setOperatorStr(current->value);

        current++;
    }

    if (current->type == Integer) {
        operand = new IntegerNode(current->value.toInteger<int64_t>());
    } else if (current->type == DecimalFraction) {
        operand = new DecimalFractionNode(current->value.toDouble());
    } else if (current->type == Boolean) {
        operand = new BooleanNode(current->value == "true");
    } else if (current->type == String) {
        operand = new StringNode(current->value);
    } else if (current->type == OpenParenthesis) {
        forward();
        operand = new BoxNode(expression({CloseParenthesis}));
    } else if (current->type == Identifier) {
        operand = new VariableNode(current->value);
    } else {
        delete before;
        throw XSharpError("Not a operand");
    }

    current++;

    while (true) {
        if (current->type == Dot) {
            current++;
            if (current->type == Identifier) {
                MemberNode* member = new MemberNode(current->value);
                member->setObject(operand);
                operand = member;
            } else {
                throw XSharpError("No member matched with '.'");
            }

        } else if (current->type == OpenParenthesis) {
            current++;
            FunctionCallNode* funcCall = new FunctionCallNode;
            funcCall->setParams(paramsList());
            funcCall->setFunction(operand);
            operand = funcCall;
        } else if (current->type == OpenBracket) {
            current++;
            IndexNode* index = new IndexNode;
            index->setOperand(operand);
            index->setIndexExpr(expression({CloseBracket}));
            operand = index;
        } else {
            break;
        }
        current++;
    }

    if (current->type == Operator) {
        if ((current + 1)->type != Integer &&
            (current + 1)->type != DecimalFraction &&
            (current + 1)->type != Boolean && (current + 1)->type != String &&
            (current + 1)->type != OpenParenthesis &&
            (current + 1)->type != Identifier) {
            after = new UnaryOperatorNode;
            after->setOperatorStr(current->value);
            current++;
        }
    }

    if (!(before || after)) {
        return operand;
    } else if (before && !after) {
        before->setOperand(operand);
        return before;
    } else if (!before && after) {
        after->setOperand(operand);
        return after;
    } else {
        if (priority(before) < priority(after)) {
            before->setOperand(operand);
            after->setOperand(before);
            return after;
        } else {
            after->setOperand(operand);
            before->setOperand(after);
            return before;
        }
    }
}

TypeNode* Parser::type()
{
    bool isConst = false;
    uint arrayDimension = 0;
    XString baseName;
    if (current->type == Keyword && current->value == "const") {
        isConst = true;
        forward();
    }

    if (current->type == Identifier) {
        baseName = current->value;
        forward();

        while (current->type == OpenBracket) {
            forward();
            if (current->type == CloseBracket) {
                arrayDimension++;
                forward();
            } else {
                throw XSharpError("No close bracket expected");
            }
        }
    } else {
        throw XSharpError("No typename matched");
    }

    if (arrayDimension == 0) {
        return XSharp::getTypeFor(baseName);
    } else {
        TypeNode* arrayType =
            XSharp::getArrayType(XSharp::getTypeFor(baseName), arrayDimension);
        return arrayType;
    }
}

int Parser::priority(BinaryOperatorNode* oper)
{
    return XSharp::binaryOperInfo[oper->operatorStr()].priority;
}

int Parser::priority(UnaryOperatorNode* oper)
{
    return XSharp::unaryOperInfo[oper->operatorStr()].priority;
}

Assoc Parser::assoc(BinaryOperatorNode* oper)
{
    return XSharp::binaryOperInfo[oper->operatorStr()].assoc;
}

bool Parser::isStopwords(Iterator tokenIter,
                         std::vector<TokenType> stopwords) const
{
    // TODO: complete the cornor case that token is end
    return (std::find(stopwords.begin(), stopwords.end(), tokenIter->type) !=
            stopwords.end()) &&
           (tokenIter != this->end);
}

void Parser::forward()
{
    if (current != end)
        current++;
    else
        throw XSharpError("Reach the end without completing parsing");
}

void Parser::backward() { current--; }
