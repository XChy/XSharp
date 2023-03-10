#include "ArithmeticOpImpl.h"
#include <cerrno>
using namespace XSharp;

ValueAndType XSharp::AssignImpl(BinaryOperatorNode* op,
                                CodeGenContextHelper* helper,
                                const Generator& generator)
{
    auto [lhs, lhs_type] = generator(op->left());
    auto [rhs, rhs_type] = generator(op->right());
    if (lhs_type->category != TypeNode::Reference) {
        helper->error("Cannot assign value to non-reference");
        return {nullptr, nullptr};
    }

    rhs = TypeAdapter::llvmConvert(rhs_type, lhs_type->innerType(), rhs);
    if (!rhs_type) {
        helper->error("The type '{}' cannot be converted to '{}'",
                      rhs_type->typeName(), lhs_type->innerType()->typeName());
        return {nullptr, nullptr};
    }

    return {helper->builder.CreateStore(rhs, lhs), lhs_type};
}

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
