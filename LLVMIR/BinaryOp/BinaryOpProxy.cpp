#include "BinaryOpProxy.h"
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"

CodeGenProxy<BinaryOperatorNode>::CodeGenProxy()
{
    processors["="] = XSharp::AssignImpl;
    processors["+"] = XSharp::AddImpl;
}
ValueAndType CodeGenProxy<BinaryOperatorNode>::codeGen(
    BinaryOperatorNode* ast, CodeGenContextHelper* helper,
    const Generator& generator)
{
    return processors[ast->operatorStr()](ast, helper, generator);
}
