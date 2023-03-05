#include "ControlFlowAST.h"
#include <form.h>
using namespace XSharp;

// If
XString IfNode::dump() const
{
    return fmt::format("If({}){}", _condition->dump(), _block->dump());
}

IfNode::~IfNode()
{
    delete _condition;
    delete _block;
}

ASTNode* IfNode::condition() { return _condition; }
void IfNode::setCondition(ASTNode* cond) { _condition = cond; }
ASTNode* IfNode::block() { return _block; }
void IfNode::setBlock(ASTNode* block) { _block = block; }

// Else if
XString ElseIfNode::dump() const
{
    return fmt::format("{}ElseIf({}){}", _lastIf->dump(), _condition->dump(),
                       _block->dump());
}

ElseIfNode::~ElseIfNode()
{
    delete _lastIf;
    delete _condition;
    delete _block;
}

ASTNode* ElseIfNode::condition() { return _condition; }
void ElseIfNode::setCondition(ASTNode* cond) { _condition = cond; }
ASTNode* ElseIfNode::block() { return _block; }
void ElseIfNode::setBlock(ASTNode* block) { _block = block; }

ASTNode* ElseIfNode::lastIf() { return _lastIf; }
void ElseIfNode::setLastIf(ASTNode* lastIf) { _lastIf = lastIf; }

// Else
XString ElseNode::dump() const
{
    return fmt::format("{}Else{}", _lastIf->dump(), _block->dump());
}

ElseNode::~ElseNode()
{
    delete _lastIf;
    delete _block;
}

ASTNode* ElseNode::block() { return _block; }
void ElseNode::setBlock(ASTNode* block) { _block = block; }

ASTNode* ElseNode::lastIf() { return _lastIf; }
void ElseNode::setLastIf(ASTNode* lastIf) { _lastIf = lastIf; }
