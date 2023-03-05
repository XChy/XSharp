#include "ControlFlowAST.h"
#include <form.h>
using namespace XSharp;

XString IfNode::dump() const { return "If(){}"; }

IfNode::~IfNode()
{
    delete _condition;
    delete _block;
}

ASTNode* IfNode::condition() { return _condition; }
void IfNode::setCondition(ASTNode* cond) { _condition = cond; }
ASTNode* IfNode::block() { return _block; }
void IfNode::setBlock(ASTNode* block) { _block = block; }
