#include "LogicalOpImpl.h"
#include "XSharp/Types/TypeSystem.h"
#include "XSharp/XString.h"
ValueAndType XSharp::EqualImpl(BinaryOperatorNode* op,
                               CodeGenContextHelper* helper,
                               const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->left()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->right()), helper);

    if (!(lhs_type->isNumber() && rhs_type->isNumber())) {
        // TODO: Support customed operator
        helper->error("Cannot compare non-numbers");
        return {nullptr, nullptr};
    }

    TypeNode* merged_type = XSharp::getMergedType(lhs_type, rhs_type);

    lhs = TypeAdapter::llvmConvert(lhs_type, merged_type, lhs);
    rhs = TypeAdapter::llvmConvert(rhs_type, merged_type, rhs);

    if (merged_type->isInteger())
        return {helper->builder.CreateICmpEQ(lhs, rhs),
                XSharp::getBooleanType()};
    else
        return {helper->builder.CreateFCmpUEQ(lhs, rhs),
                XSharp::getBooleanType()};
}

ValueAndType XSharp::GreaterImpl(BinaryOperatorNode* op,
                                 CodeGenContextHelper* helper,
                                 const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->left()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->right()), helper);

    if (!(lhs_type->isNumber() && rhs_type->isNumber())) {
        // TODO: Support customed operator
        helper->error("Cannot compare non-numbers");
        return {nullptr, nullptr};
    }

    TypeNode* merged_type = XSharp::getMergedType(lhs_type, rhs_type);

    lhs = TypeAdapter::llvmConvert(lhs_type, merged_type, lhs);
    rhs = TypeAdapter::llvmConvert(rhs_type, merged_type, rhs);

    if (merged_type->isSigned())
        return {helper->builder.CreateICmpSGT(lhs, rhs),
                XSharp::getBooleanType()};
    else if (merged_type->isUnsigned())
        return {helper->builder.CreateICmpUGT(lhs, rhs),
                XSharp::getBooleanType()};
    else
        return {helper->builder.CreateFCmpUGT(lhs, rhs),
                XSharp::getBooleanType()};
}

ValueAndType XSharp::LessImpl(BinaryOperatorNode* op,
                              CodeGenContextHelper* helper,
                              const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->left()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->right()), helper);

    if (!(lhs_type->isNumber() && rhs_type->isNumber())) {
        // TODO: Support customed operator
        helper->error("Cannot compare non-numbers");
        return {nullptr, nullptr};
    }

    TypeNode* merged_type = XSharp::getMergedType(lhs_type, rhs_type);

    lhs = TypeAdapter::llvmConvert(lhs_type, merged_type, lhs);
    rhs = TypeAdapter::llvmConvert(rhs_type, merged_type, rhs);

    if (merged_type->isSigned())
        return {helper->builder.CreateICmpSLT(lhs, rhs),
                XSharp::getBooleanType()};
    else if (merged_type->isUnsigned())
        return {helper->builder.CreateICmpULT(lhs, rhs),
                XSharp::getBooleanType()};
    else
        return {helper->builder.CreateFCmpULT(lhs, rhs),
                XSharp::getBooleanType()};
}

ValueAndType XSharp::GreaterOrEqualImpl(BinaryOperatorNode* op,
                                        CodeGenContextHelper* helper,
                                        const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->left()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->right()), helper);

    if (!(lhs_type->isNumber() && rhs_type->isNumber())) {
        // TODO: Support customed operator
        helper->error("Cannot compare non-numbers");
        return {nullptr, nullptr};
    }

    TypeNode* merged_type = XSharp::getMergedType(lhs_type, rhs_type);

    lhs = TypeAdapter::llvmConvert(lhs_type, merged_type, lhs);
    rhs = TypeAdapter::llvmConvert(rhs_type, merged_type, rhs);

    if (merged_type->isSigned())
        return {helper->builder.CreateICmpSGE(lhs, rhs),
                XSharp::getBooleanType()};
    else if (merged_type->isUnsigned())
        return {helper->builder.CreateICmpUGE(lhs, rhs),
                XSharp::getBooleanType()};
    else
        return {helper->builder.CreateFCmpUGE(lhs, rhs),
                XSharp::getBooleanType()};
}

ValueAndType XSharp::LessOrEqualImpl(BinaryOperatorNode* op,
                                     CodeGenContextHelper* helper,
                                     const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->left()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->right()), helper);

    if (!(lhs_type->isNumber() && rhs_type->isNumber())) {
        // TODO: Support customed operator
        helper->error("Cannot compare non-numbers");
        return {nullptr, nullptr};
    }

    TypeNode* merged_type = XSharp::getMergedType(lhs_type, rhs_type);

    lhs = TypeAdapter::llvmConvert(lhs_type, merged_type, lhs);
    rhs = TypeAdapter::llvmConvert(rhs_type, merged_type, rhs);

    if (merged_type->isSigned())
        return {helper->builder.CreateICmpSLE(lhs, rhs),
                XSharp::getBooleanType()};
    else if (merged_type->isUnsigned())
        return {helper->builder.CreateICmpULE(lhs, rhs),
                XSharp::getBooleanType()};
    else
        return {helper->builder.CreateFCmpULE(lhs, rhs),
                XSharp::getBooleanType()};
}
