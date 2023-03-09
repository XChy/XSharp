#include "ArithmeticOpImpl.h"
using namespace XSharp;

ValueAndType XSharp::AddImpl(BinaryOperatorNode* op,
                             CodeGenContextHelper* helper,
                             const Generator& generator)
{
    auto [lhs, lhs_type] = generator(op->left());
    lhs = TypeAdapter::llvmConvert(lhs_type, XSharp::getI64Type(), lhs);
    auto [rhs, rhs_type] = generator(op->right());
    rhs = TypeAdapter::llvmConvert(rhs_type, XSharp::getI64Type(), rhs);
    return {helper->builder.CreateAdd(lhs, rhs), XSharp::getI64Type()};
}
