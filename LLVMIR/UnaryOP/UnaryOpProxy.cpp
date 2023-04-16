#include "UnaryOpProxy.h"
#include "XSharp/ASTNodes.h"

using namespace XSharp;
using namespace XSharp::LLVMCodeGen;

CodeGenProxy<UnaryOperatorNode>::CodeGenProxy()
{
    processors["+"] = PositiveImpl;
    processors["-"] = NegativeImpl;
    processors["!"] = NotImpl;
}

ValueAndType CodeGenProxy<UnaryOperatorNode>::codeGen(
    UnaryOperatorNode* ast, CodeGenContext* helper, const Generator& generator)
{
    if (processors.contains(ast->operatorStr()))
        return processors[ast->operatorStr()](ast, helper, generator);
    else {
        helper->error("Not support the unary operator {}", ast->operatorStr());
        return {nullptr, nullptr};
    }
}
