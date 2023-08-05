#include "UnaryOpProxy.h"
#include "XSharp/ASTNodes.h"

using namespace XSharp;
using namespace XSharp::LLVMCodeGen;

CodeGenProxy<UnaryOpNode>::CodeGenProxy()
{
    processors["+"] = PositiveImpl;
    processors["-"] = NegativeImpl;
    processors["!"] = NotImpl;
}

ValueAndType CodeGenProxy<UnaryOpNode>::codeGen(UnaryOpNode* ast,
                                                CodeGenContext* helper,
                                                const Generator& generator)
{
    if (processors.contains(ast->opStr()))
        return processors[ast->opStr()](ast, helper, generator);
    else {
        helper->error("Not support the unary operator {}", ast->opStr());
        return {nullptr, nullptr};
    }
}
