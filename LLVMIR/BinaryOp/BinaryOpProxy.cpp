#include "BinaryOpProxy.h"
#include "XSharp/ASTNodes.h"

CodeGenProxy<BinaryOperatorNode>::CodeGenProxy()
{
    processors["="] = XSharp::AssignImpl;
    processors["+"] = XSharp::AddImpl;
    processors["-"] = XSharp::SubImpl;
    processors["*"] = XSharp::MulImpl;
    processors["/"] = XSharp::DivImpl;
    processors["=="] = XSharp::EqualImpl;
    processors[">"] = XSharp::GreaterImpl;
    processors["<"] = XSharp::LessImpl;
    processors[">="] = XSharp::GreaterOrEqualImpl;
    processors["<="] = XSharp::LessOrEqualImpl;
}
ValueAndType CodeGenProxy<BinaryOperatorNode>::codeGen(
    BinaryOperatorNode* ast, CodeGenContextHelper* helper,
    const Generator& generator)
{
    return processors[ast->operatorStr()](ast, helper, generator);
}
