#include "UnaryOpProxy.h"
#include "XSharp/ASTNodes.h"

CodeGenProxy<UnaryOperatorNode>::CodeGenProxy()
{
    processors["+"] = XSharp::PositiveImpl;
    processors["-"] = XSharp::NegativeImpl;
    processors["!"] = XSharp::NotImpl;
}

ValueAndType CodeGenProxy<UnaryOperatorNode>::codeGen(
    UnaryOperatorNode* ast, CodeGenContextHelper* helper,
    const Generator& generator)
{
    if (processors.contains(ast->operatorStr()))
        return processors[ast->operatorStr()](ast, helper, generator);
    else {
        helper->error("Not support the unary operator {}", ast->operatorStr());
        return {nullptr, nullptr};
    }
}
