#include "BinaryOpProxy.h"
#include "XSharp/ASTNodes.h"

CodeGenProxy<BinaryOperatorNode>::CodeGenProxy()
{
    processors["="] = XSharp::AssignImpl;
    processors["+"] = XSharp::AddImpl;
    processors["-"] = XSharp::SubImpl;
    processors["*"] = XSharp::MulImpl;
    processors["/"] = XSharp::DivImpl;
    processors["%"] = XSharp::ModImpl;

    processors["=="] = XSharp::EqualImpl;
    processors[">"] = XSharp::GreaterImpl;
    processors["<"] = XSharp::LessImpl;
    processors[">="] = XSharp::GreaterOrEqualImpl;
    processors["<="] = XSharp::LessOrEqualImpl;

    processors["&&"] = XSharp::LogicalAndImpl;
    processors["||"] = XSharp::LogicalOrImpl;
}

ValueAndType CodeGenProxy<BinaryOperatorNode>::codeGen(
    BinaryOperatorNode* ast, CodeGenContextHelper* helper,
    const Generator& generator)
{
    auto iter = processors.find(ast->operatorStr());
    if (iter != processors.end())
        return processors[ast->operatorStr()](ast, helper, generator);
    else {
        helper->error("Not support the operator '{}' yet", ast->operatorStr());
        return {nullptr, nullptr};
    }
}
