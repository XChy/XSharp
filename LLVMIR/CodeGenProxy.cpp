#include "CodeGenProxy.h"
#include <llvm-14/llvm/ADT/APFloat.h>
#include <llvm-14/llvm/ADT/APInt.h>
#include "XSharp/ASTNodes.h"
ValueAndType CodeGenProxy<IntegerNode>::codeGen(IntegerNode* ast,
                                                CodeGenContextHelper* helper,
                                                const Generator& generator)
{
    using llvm::APInt;
    using llvm::ConstantInt;
    auto val = ConstantInt::get(helper->context, APInt(64, ast->value()));

    return {val, XSharp::getI64Type()};
}

ValueAndType CodeGenProxy<DecimalFractionNode>::codeGen(
    DecimalFractionNode* ast, CodeGenContextHelper* helper,
    const Generator& generator)
{
    using llvm::APFloat;
    using llvm::ConstantFP;
    auto val = ConstantFP::get(helper->context, APFloat(ast->value()));

    return {val, XSharp::getI64Type()};
}

ValueAndType CodeGenProxy<BooleanNode>::codeGen(BooleanNode* ast,
                                                CodeGenContextHelper* helper,
                                                const Generator& generator)
{
    using llvm::APInt;
    using llvm::ConstantInt;

    auto val = ConstantInt::get(helper->context, APInt(8, ast->value()));
    return {val, XSharp::getBooleanType()};
}
