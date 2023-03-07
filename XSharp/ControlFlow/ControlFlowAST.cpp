#include "ControlFlowAST.h"
#include <form.h>
#include "XSharp/ASTNodes.h"
#include "XSharp/Type.h"
#include "fmt/core.h"
using namespace XSharp;

// If
XString IfNode::dump() const
{
    if (!elseAst)
        return fmt::format("If({}){}", condition->dump(), block->dump());
    else
        return fmt::format("If ({}) {} Else {}", condition->dump(),
                           block->dump(), elseAst->dump());
}

IfNode::~IfNode()
{
    delete condition;
    delete block;
    if (elseAst) delete elseAst;
}
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

BlockNode* ElseIfNode::block() { return _block; }
void ElseIfNode::setBlock(BlockNode* block) { _block = block; }

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

BlockNode* ElseNode::block() { return _block; }
void ElseNode::setBlock(BlockNode* block) { _block = block; }

ASTNode* ElseNode::lastIf() { return _lastIf; }
void ElseNode::setLastIf(ASTNode* lastIf) { _lastIf = lastIf; }

// While
XString WhileNode::dump() const
{
    return fmt::format("while({}){}", condition->dump(), block->dump());
}
WhileNode::~WhileNode()
{
    delete condition;
    delete block;
}
// Continue
XString ContinueNode::dump() const { return fmt::format("continue"); }

ContinueNode::~ContinueNode() {}

// Break
XString BreakNode::dump() const { return fmt::format("break"); }

BreakNode::~BreakNode() {}
