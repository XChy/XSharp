#include "ControlFlowAST.h"
#include <form.h>
#include "XSharp/ASTNodes.h"
#include "XSharp/Types/Type.h"
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
