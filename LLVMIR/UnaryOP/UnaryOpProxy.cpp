#include "UnaryOpProxy.h"
#include "XSharp/ASTNodes.h"

CodeGenProxy<UnaryOperatorNode>::CodeGenProxy()
{
    processors["+"] = XSharp::PositiveImpl;
}

ValueAndType CodeGenProxy<UnaryOperatorNode>::codeGen(
    UnaryOperatorNode* ast, CodeGenContextHelper* helper,
    const Generator& generator)
{
    return processors[ast->operatorStr()](ast, helper, generator);
}
