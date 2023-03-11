#include "ArithmeticOpImpl.h"
#include <cerrno>
#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/LLVMTypes.h"
#include "XSharp/Types/Type.h"
#include "XSharp/Types/TypeSystem.h"
using namespace XSharp;

ValueAndType XSharp::AssignImpl(BinaryOperatorNode* op,
                                CodeGenContextHelper* helper,
                                const Generator& generator)
{
    auto [lhs, lhs_type] = generator(op->left());
    auto [rhs, rhs_type] = generator(op->right());
    // Only the reference can be assigned to value
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
    auto [lhs, lhs_type] = deReference(generator(op->left()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->right()), helper);

    if (!(lhs_type->isNumber() && rhs_type->isNumber())) {
        // TODO: Support customed operator
        helper->error("Cannot add non-numbers");
        return {nullptr, nullptr};
    }

    TypeNode* merged_type = XSharp::getMergedType(lhs_type, rhs_type);

    lhs = TypeAdapter::llvmConvert(lhs_type, merged_type, lhs);
    rhs = TypeAdapter::llvmConvert(rhs_type, merged_type, rhs);

    return {helper->builder.CreateAdd(lhs, rhs), merged_type};
}

ValueAndType XSharp::SubImpl(BinaryOperatorNode* op,
                             CodeGenContextHelper* helper,
                             const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->left()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->right()), helper);

    if (!(lhs_type->isNumber() && rhs_type->isNumber())) {
        // TODO: Support customed operator
        helper->error("Cannot subtract non-numbers");
        return {nullptr, nullptr};
    }

    TypeNode* merged_type = XSharp::getMergedType(lhs_type, rhs_type);

    lhs = TypeAdapter::llvmConvert(lhs_type, merged_type, lhs);
    rhs = TypeAdapter::llvmConvert(rhs_type, merged_type, rhs);

    return {helper->builder.CreateSub(lhs, rhs), merged_type};
}

ValueAndType XSharp::MulImpl(BinaryOperatorNode* op,
                             CodeGenContextHelper* helper,
                             const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->left()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->right()), helper);

    if (!(lhs_type->isNumber() && rhs_type->isNumber())) {
        // TODO: Support customed operator
        helper->error("Cannot multiply non-numbers");
        return {nullptr, nullptr};
    }

    TypeNode* merged_type = XSharp::getMergedType(lhs_type, rhs_type);

    lhs = TypeAdapter::llvmConvert(lhs_type, merged_type, lhs);
    rhs = TypeAdapter::llvmConvert(rhs_type, merged_type, rhs);

    return {helper->builder.CreateMul(lhs, rhs), merged_type};
}

ValueAndType XSharp::DivImpl(BinaryOperatorNode* op,
                             CodeGenContextHelper* helper,
                             const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->left()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->right()), helper);

    if (!(lhs_type->isNumber() && rhs_type->isNumber())) {
        // TODO: Support customed operator
        helper->error("Cannot divide non-numbers");
        return {nullptr, nullptr};
    }

    TypeNode* merged_type = XSharp::getMergedType(lhs_type, rhs_type);

    lhs = TypeAdapter::llvmConvert(lhs_type, merged_type, lhs);
    rhs = TypeAdapter::llvmConvert(rhs_type, merged_type, rhs);

    if (merged_type->isSigned())
        return {helper->builder.CreateSDiv(lhs, rhs), merged_type};
    else if (merged_type->isUnsigned())
        return {helper->builder.CreateUDiv(lhs, rhs), merged_type};
    else
        return {helper->builder.CreateFDiv(lhs, rhs), merged_type};
}
