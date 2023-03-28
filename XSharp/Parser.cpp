#include "Parser.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include "XSharp/ASTNodes.h"
#include "XSharp/Class/ClassAST.h"
#include "XSharp/ControlFlow/ControlFlowAST.h"
#include "XSharp/OperInfo.h"
#include "XSharp/Tokens.h"
#include "XSharp/Types/Type.h"
#include "XSharp/Types/TypeNodes.h"
#include "XSharp/XSharpUtils.h"
#include "XSharp/XString.h"

using namespace XSharp;

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
        if (current->isKeyword("class")) {
            root->decls.push_back(classDeclaration());
            // root->addClass(classDeclaration());
        } else if (isVariableDecl()) {
            root->decls.push_back(variableDeclaration({SentenceEnd}));
            // root->addVariable(variableDeclaration({SentenceEnd}));
        } else if (isFunctionDecl()) {
            // root->addFunction(functionDeclaration());
            root->decls.push_back(functionDeclaration());
        } else {
            throw XSharpError("Not a definition in global");
        }
    }
    return root;
}

ClassNode* Parser::classDeclaration()
{
    ClassNode* classNode = new ClassNode;
    // skip 'class'
    forward();

    if (!current->is(Identifier)) throw XSharpError("No name for class");

    classNode->name = current->value;
    forward();

    // skip '{'
    if (current->is(OpenBrace))
        forward();
    else
        throw XSharpError("Expected '{' is missing");

    while (!current->is(CloseBrace)) {
        if (isVariableDecl()) {
            auto var = variableDeclaration({SentenceEnd});
            classNode->members.push_back(var);
        } else if (isFunctionDecl()) {
            auto func = functionDeclaration();
            classNode->methods.push_back(func);
        } else {
            throw XSharpError("Not a field in class");
        }
    }

    // skip '}'
    forward();

    // analyze the class block
    return classNode;
}

bool Parser::isFunctionDecl() const
{
    auto localCurrent = current;

    // <type-expression>
    if (localCurrent->isKeyword("const")) localCurrent++;

    if (!localCurrent->is(Identifier)) return false;

    localCurrent++;

    while (localCurrent->is(OpenBracket)) {
        localCurrent++;
        if (localCurrent->is(CloseBracket)) {
            localCurrent++;
        } else {
            return false;
        }
    }

    // <name>
    if (!localCurrent->is(Identifier)) return false;

    localCurrent++;

    // '(paramlist)'
    if (localCurrent->is(OpenParenthesis)) return true;

    return false;
}
FunctionNode* Parser::functionDeclaration()
{
    FunctionNode* root = new FunctionNode;

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

bool Parser::isVariableDecl() const
{
    auto localCurrent = current;

    // <type-expression>
    if (localCurrent->isKeyword("const")) localCurrent++;

    if (!localCurrent->is(Identifier)) return false;

    localCurrent++;

    while (localCurrent->is(OpenBracket)) {
        localCurrent++;
        if (localCurrent->is(CloseBracket)) {
            localCurrent++;
        } else {
            return false;
        }
    }

    // <name>
    if (!localCurrent->is(Identifier)) return false;

    localCurrent++;

    // Initializer or SentenceEnd

    if (localCurrent->is(SentenceEnd)) return true;
    if (localCurrent->isOperator("=")) return true;

    return false;
}

VariableNode* Parser::variableDeclaration(
    const std::vector<TokenType>& stopwords)
{
    VariableNode* root = new VariableNode;
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
    } else if (current->isOperator("=")) {
        forward();
        root->setInitValue(expression(stopwords));
        forward();
    } else {
        throw XSharpError("variable defintion error");
    }
    return root;
}

std::vector<VariableNode*> Parser::paramsDefinition()
{
    std::vector<VariableNode*> paramsDef;

    // if no parameter in parentheses, then return empty paramsDef
    if (current->type == CloseParenthesis) return paramsDef;

    while (true) {
        paramsDef.push_back(variableDeclaration({Comma, CloseParenthesis}));
        backward();

        if (current->is(CloseParenthesis))
            break;
        else if (current->is(Comma))
            forward();
        else
            throw XSharpError("')' expected is missing");
    }
    return paramsDef;
}

std::vector<ASTNode*> Parser::paramsList()
{
    std::vector<ASTNode*> results;
    if (current->is(CloseParenthesis)) return results;

    while (true) {
        results.push_back(expression({CloseParenthesis, Comma}));
        if (current->is(CloseParenthesis))
            break;
        else if (current->is(Comma))
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
                stmt = ifStatement();
            } else if (current->value == "while") {
                stmt = whileStatement();
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

IfNode* Parser::ifStatement()
{
    IfNode* ifNode = nullptr;
    ASTNode* condition;
    ASTNode* codeblock;
    if (!current->isKeyword("if")) {
        throw XSharpError(ParsingError, "No 'if' matched");
        return nullptr;
    }
    forward();

    // Condtion
    if (current->type == OpenParenthesis) {
        forward();
        condition = expression({CloseParenthesis});
        forward();
    } else {
        throw XSharpError(ParsingError, "No 'if' matched");
        return nullptr;
    }

    // Code Block
    if (current->type == OpenBrace) {
        codeblock = block();
    } else {
        codeblock = statement();
    }

    ifNode = new IfNode{condition, codeblock};

    while (current->isKeyword("else")) {
        forward();
        if (current->type == OpenBrace) {
            ifNode->elseAst = block();
        } else {
            ifNode->elseAst = statement();
        }
    }

    return ifNode;
}

WhileNode* Parser::whileStatement()
{
    WhileNode* whileNode = nullptr;
    ASTNode* condition;
    ASTNode* codeblock;
    if (!current->isKeyword("while")) {
        throw XSharpError(ParsingError, "No 'while' matched");
        return nullptr;
    }
    forward();

    // Condtion
    if (current->type == OpenParenthesis) {
        forward();
        condition = expression({CloseParenthesis});
        forward();
    } else {
        throw XSharpError(ParsingError, "No 'while' matched");
        return nullptr;
    }

    // Code Block
    if (current->type == OpenBrace) {
        codeblock = block();
    } else {
        codeblock = statement();
    }

    return new WhileNode{condition, codeblock};
}

ASTNode* Parser::expression(std::vector<TokenType> stopwords, int ctxPriority)
{
    if (isStopwords(current, stopwords)) {
        // TODO: error?
        return nullptr;
    }

    // Pratt Parsing
    ASTNode* lhs = operand();

    while (true) {
        if (isStopwords(current, stopwords)) return lhs;

        if (current->type != Operator)
            throw XSharpError("No operator matched after operand");

        if (priority(current->value) <= ctxPriority) break;

        XString op = current->value;

        forward();
        auto right_binding_power =
            assoc(op) == LeftToRight ? priority(op) : priority(op) - 1;
        auto rhs = expression(stopwords, right_binding_power);

        auto new_lhs = new BinaryOperatorNode;
        new_lhs->setOperatorStr(op);
        new_lhs->setLeft(lhs);
        new_lhs->setRight(rhs);
        lhs = new_lhs;
    }

    return lhs;
}

ASTNode* Parser::operand()
{
    UnaryOperatorNode* before = nullptr;
    UnaryOperatorNode* after = nullptr;
    ASTNode* operand = nullptr;
    if (current->type == Operator) {
        before = new UnaryOperatorNode;
        before->setOperatorStr(current->value);
        forward();
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
        operand = expression({CloseParenthesis});
    } else if (current->type == Identifier) {
        operand = new VariableExprNode(current->value);
    } else {
        delete before;
        throw XSharpError("Not a operand");
    }

    current++;

    while (true) {
        if (current->type == Dot) {
            current++;
            if (current->type == Identifier) {
                MemberExprNode* member = new MemberExprNode(current->value);
                member->setObject(operand);
                operand = member;
            } else {
                throw XSharpError("No member matched with '.'");
            }

        } else if (current->type == OpenParenthesis) {
            current++;
            FunctionCallNode* funcCall = new FunctionCallNode;
            funcCall->setArgs(paramsList());
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
    Decoration decoration = {.isMutable = true, .isConstexpr = false};
    int arrayDimension = 0;
    XString baseName;
    if (current->isKeyword("immutable")) {
        decoration.isMutable = false;
        forward();
    }

    if (current->is(Identifier)) {
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

    // TODO: Complete the type for Identifier and Array
    // TODO: add decoration
    if (arrayDimension == 0) {
        return new IdentifierNode(baseName);
    } else {
        return new ArrayNode(new IdentifierNode(baseName), arrayDimension);
    }
}

int Parser::priority(const XString& op)
{
    return XSharp::binaryOperInfo[op].priority;
}
int Parser::priority(BinaryOperatorNode* oper)
{
    return XSharp::binaryOperInfo[oper->operatorStr()].priority;
}

int Parser::priority(UnaryOperatorNode* oper)
{
    return XSharp::unaryOperInfo[oper->operatorStr()].priority;
}

Assoc Parser::assoc(const XString& op)
{
    return XSharp::binaryOperInfo[op].assoc;
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
    if (current != end && !current->is(Eof))
        current++;
    else
        throw XSharpError("Reach the end without completing parsing");
}

void Parser::backward() { current--; }
