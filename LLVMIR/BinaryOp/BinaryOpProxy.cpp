#include "BinaryOpProxy.h"
#include "XSharp/ASTNodes.h"

using namespace XSharp::LLVMCodeGen;
using namespace XSharp;

CodeGenProxy<BinaryOperatorNode>::CodeGenProxy()
{
    processors["="] = AssignImpl;
    processors["+"] = AddImpl;
    processors["-"] = SubImpl;
    processors["*"] = MulImpl;
    processors["/"] = DivImpl;
    processors["%"] = ModImpl;

    processors["=="] = EqualImpl;
    processors[">"] = GreaterImpl;
    processors["<"] = LessImpl;
    processors[">="] = GreaterOrEqualImpl;
    processors["<="] = LessOrEqualImpl;

    processors["&&"] = LogicalAndImpl;
    processors["||"] = LogicalOrImpl;
}

ValueAndType CodeGenProxy<BinaryOperatorNode>::codeGen(
    BinaryOperatorNode* ast, CodeGenContext* helper, const Generator& generator)
{
    auto iter = processors.find(ast->opStr());
    if (iter != processors.end())
        return processors[ast->opStr()](ast, helper, generator);
    else {
        helper->error("Not support the operator '{}' yet", ast->opStr());
        return {nullptr, nullptr};
    }
}
