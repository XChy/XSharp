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
    while (!shouldStopOn(current))
        if (current->isKeyword("class"))
            root->decls.push_back(classDecl());
        else if (isVariableDecl())
            root->decls.push_back(variableDecl({SentenceEnd}));
        else if (isFunctionDecl())
            root->decls.push_back(functionDecl());
        else
            throw XSharpError("Not a definition in global");

    return root;
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
    if (localCurrent->is(OpenParen)) return true;

    return false;
}

MemberMethodNode* Parser::memberMethodDecl()
{
    MemberMethodNode* root = new MemberMethodNode;

    root->setReturnType(type());

    root->setName(current->value);
    forward();

    // start with '('
    if (current->type != OpenParen) {
        throw XSharpError("No '(' matched");
    }
    forward();

    root->setParams(parameters());
    // end with ')', so skip ')'
    forward();

    root->setImpl(block());
    return root;
}

ConstructorNode* Parser::constructor()
{
    ConstructorNode* root = new ConstructorNode;

    // skip keyword 'new'
    forward();

    // start with '('
    if (current->type != OpenParen) {
        throw XSharpError("No '(' matched");
    }
    forward();

    root->parameters = parameters();
    // end with ')', so skip ')'
    forward();

    root->impl = block();

    return root;
}

FunctionNode* Parser::functionDecl()
{
    FunctionNode* root = new FunctionNode;

    root->setReturnType(type());

    root->setName(current->value);
    forward();

    // start with '('
    if (current->type != OpenParen) throw XSharpError("No '(' matched");

    forward();

    root->setParams(parameters());
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
        if (localCurrent->is(CloseBracket))
            localCurrent++;
        else
            return false;
    }

    // <name>
    if (!localCurrent->is(Identifier)) return false;

    localCurrent++;

    // Initializer or SentenceEnd

    if (localCurrent->is(SentenceEnd)) return true;
    if (localCurrent->isOperator("=")) return true;

    return false;
}

VarDeclNode* Parser::variableDecl(const std::vector<TokenType>& stopwords)
{
    VarDeclNode* root = new VarDeclNode;
    root->setType(type());

    if (current->type == Identifier) {
        root->setName(current->value);
        forward();
    } else {
        throw XSharpError("No name after a typename");
    }

    if (shouldStopOn(current, stopwords)) {
        root->setInit(nullptr);
        forward();
    } else if (current->isOperator("=")) {
        forward();
        root->setInit(expr(stopwords));
        forward();
    } else {
        throw XSharpError("variable defintion error");
    }
    return root;
}

std::vector<VarDeclNode*> Parser::parameters()
{
    std::vector<VarDeclNode*> params;

    // if no parameter in parentheses, then return empty paramsDef
    if (current->type == CloseParen) return params;

    while (true) {
        params.push_back(variableDecl({Comma, CloseParen}));
        backward();

        if (current->is(CloseParen))
            break;
        else if (current->is(Comma))
            forward();
        else
            throw XSharpError("')' expected is missing");
    }
    return params;
}

std::vector<ASTNode*> Parser::argsList()
{
    std::vector<ASTNode*> results;
    if (current->is(CloseParen)) return results;

    while (true) {
        results.push_back(expr({CloseParen, Comma}));
        if (current->is(CloseParen))
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
            } else if (shouldStopOn(current)) {
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
                stmt = variableDecl({SentenceEnd});
            } else if (current->value == "return") {
                forward();
                stmt = new ReturnNode(expr({SentenceEnd}));
                forward();
            } else if (current->value == "if") {
                stmt = ifStatement();
            } else if (current->value == "while") {
                stmt = whileStatement();
            } else if (current->value == "new") {
                stmt = expr({SentenceEnd});
            } else if (current->value == "continue") {
                stmt = new ContinueNode;
                forward();
                forward();
            } else if (current->value == "break") {
                stmt = new BreakNode;
                forward();
                forward();
            } else {
                throw XSharpError("Not a statement");
            }
            break;
        case Identifier:
            if ((current + 1)->type == OpenBracket &&
                (current + 2)->type == CloseBracket) {
                stmt = variableDecl({SentenceEnd});
            } else if ((current + 1)->type == Identifier) {
                stmt = variableDecl({SentenceEnd});
            } else {
                stmt = expr({SentenceEnd});
                forward();
            }
            break;
        default:
            stmt = expr({SentenceEnd});
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
    if (current->type == OpenParen) {
        forward();
        condition = expr({CloseParen});
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
        if (current->type == OpenBrace)
            ifNode->elseAst = block();
        else
            ifNode->elseAst = statement();
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
    if (current->type == OpenParen) {
        forward();
        condition = expr({CloseParen});
        forward();
    } else {
        throw XSharpError(ParsingError, "No 'while' matched");
        return nullptr;
    }

    // Code Block
    if (current->type == OpenBrace)
        codeblock = block();
    else
        codeblock = statement();

    return new WhileNode{condition, codeblock};
}

ASTNode* Parser::expr(std::vector<TokenType> stopwords, int ctxPriority)
{
    if (shouldStopOn(current, stopwords)) {
        // TODO: error?
        return nullptr;
    }

    // Pratt Parsing
    ASTNode* lhs = operand();

    while (true) {
        if (shouldStopOn(current, stopwords)) return lhs;

        if (current->type != Operator && !current->isKeyword("new"))
            throw XSharpError("No operator matched after operand");

        if (priority(current->value) <= ctxPriority) break;

        XString op = current->value;

        forward();
        auto right_binding_power =
            assoc(op) == LeftToRight ? priority(op) : priority(op) - 1;
        auto rhs = expr(stopwords, right_binding_power);

        auto new_lhs = new BinaryOperatorNode;
        new_lhs->setOpStr(op);
        new_lhs->setLhs(lhs);
        new_lhs->setRhs(rhs);
        lhs = new_lhs;
    }

    return lhs;
}

ASTNode* Parser::operand()
{
    UnaryOpNode* before = nullptr;
    UnaryOpNode* after = nullptr;
    ASTNode* operand = nullptr;

    if (current->type == Operator) {
        before = new UnaryOpNode;
        before->setOpStr(current->value);
        forward();
    }

    operand = factor();

    while (true) {
        if (current->type == Dot) {
            current++;
            if (current->type == Identifier) {
                MemberExprNode* member = new MemberExprNode(current->value);
                member->setObject(operand);
                operand = member;
            } else
                throw XSharpError("No member matched with '.'");

        } else if (current->type == OpenParen) {
            current++;
            CallNode* funcCall = new CallNode;
            funcCall->setArgs(argsList());
            funcCall->setCallee(operand);
            operand = funcCall;
        } else if (current->type == OpenBracket) {
            current++;
            IndexNode* index = new IndexNode;
            index->setIndexed(operand);
            index->setIndex(expr({CloseBracket}));
            operand = index;
        } else
            break;
        current++;
    }

    //  FIXME: bug of prefix
    //  if (current->type == Operator) {
    //  if ((current + 1)->type != Integer &&
    //(current + 1)->type != DecimalFraction &&
    //(current + 1)->type != Boolean && (current + 1)->type != String &&
    //(current + 1)->type != OpenParenthesis &&
    //(current + 1)->type != Identifier) {
    //  after = new UnaryOperatorNode;
    //  after->setOperatorStr(current->value);
    //  current++;
    // }
    // }

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

ASTNode* Parser::factor()
{
    ASTNode* factor;
    if (current->type == Integer) {
        factor = new IntegerNode(current->value.toInteger<int64_t>());
    } else if (current->type == Decimal) {
        factor = new FPNode(current->value.toDouble());
    } else if (current->type == Boolean) {
        factor = new BooleanNode(current->value == "true");
    } else if (current->type == Char) {
        factor = new CharNode(current->value[0]);
    } else if (current->type == String) {
        factor = new StringNode(current->value);
    } else if (current->type == OpenParen) {
        forward();
        factor = expr({CloseParen});
    } else if (current->type == Identifier) {
        factor = new VarExprNode(current->value);
    } else if (current->isKeyword("new")) {
        forward();

        auto allocatedType = type();
        std::vector<ASTNode*> args;
        if (current->is(OpenParen)) {
            forward();
            args = argsList();
        } else {
            current--;
        }

        auto init_called = new CallNode;
        init_called->setCallee(allocatedType);
        init_called->setArgs(args);

        factor = init_called;
    } else {
        throw XSharpError("Not a operand");
    }

    current++;

    return factor;
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

    // TODO: add decoration
    if (arrayDimension == 0)
        return new IdentifierNode(baseName);
    else
        return new ArrayNode(new IdentifierNode(baseName), arrayDimension);
}

int Parser::priority(const XString& op) { return XSharp::binOp[op].priority; }
int Parser::priority(BinaryOperatorNode* oper)
{
    return XSharp::binOp[oper->opStr()].priority;
}

int Parser::priority(UnaryOpNode* oper)
{
    return XSharp::unaryOp[oper->opStr()].priority;
}

Assoc Parser::assoc(const XString& op) { return XSharp::binOp[op].assoc; }

Assoc Parser::assoc(BinaryOperatorNode* oper)
{
    return XSharp::binOp[oper->opStr()].assoc;
}

bool Parser::shouldStopOn(Iterator tokenIter,
                          std::vector<TokenType> stopwords) const
{
    // TODO: complete the cornor case that token is end
    return (std::find(stopwords.begin(), stopwords.end(), tokenIter->type) !=
            stopwords.end()) ||
           (tokenIter->type == TokenType::Eof);
}

void Parser::forward()
{
    if (current != end && !current->is(Eof))
        current++;
    else
        throw XSharpError("Reach the end without completing parsing");
}

void Parser::backward() { current--; }
