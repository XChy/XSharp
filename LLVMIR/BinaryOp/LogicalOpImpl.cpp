#include "LogicalOpImpl.h"
#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/Utils.h"
#include "XSharp/Types/TypeSystem.h"
#include "XSharp/XString.h"
namespace XSharp {
namespace LLVMCodeGen {

ValueAndType EqualImpl(BinaryOperatorNode* op, CodeGenContextHelper* helper,
                       const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->left()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->right()), helper);

    passErrorIfNot(lhs_type);
    passErrorIfNot(rhs_type);

    if (!(lhs_type->isNumber() && rhs_type->isNumber())) {
        // TODO: Support customed operator
        helper->error("Cannot compare non-numbers");
        return {nullptr, nullptr};
    }

    Type* merged_type = XSharp::getMergedType(lhs_type, rhs_type);
    passErrorIfNot(merged_type);

    lhs = TypeAdapter::llvmConvert(lhs_type, merged_type, lhs);
    rhs = TypeAdapter::llvmConvert(rhs_type, merged_type, rhs);

    if (merged_type->isInteger())
        return {helper->builder.CreateICmpEQ(lhs, rhs),
                XSharp::getBooleanType()};
    else
        return {helper->builder.CreateFCmpUEQ(lhs, rhs),
                XSharp::getBooleanType()};
}

ValueAndType NotEqualImpl(BinaryOperatorNode* op, CodeGenContextHelper* helper,
                          const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->left()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->right()), helper);

    passErrorIfNot(lhs_type);
    passErrorIfNot(rhs_type);

    if (!(lhs_type->isNumber() && rhs_type->isNumber())) {
        // TODO: Support customed operator
        helper->error("Cannot compare non-numbers");
        return {nullptr, nullptr};
    }

    Type* merged_type = XSharp::getMergedType(lhs_type, rhs_type);
    passErrorIfNot(merged_type);

    lhs = TypeAdapter::llvmConvert(lhs_type, merged_type, lhs);
    rhs = TypeAdapter::llvmConvert(rhs_type, merged_type, rhs);

    if (merged_type->isInteger())
        return {helper->builder.CreateICmpNE(lhs, rhs),
                XSharp::getBooleanType()};
    else
        return {helper->builder.CreateFCmpUNE(lhs, rhs),
                XSharp::getBooleanType()};
}
ValueAndType GreaterImpl(BinaryOperatorNode* op, CodeGenContextHelper* helper,
                         const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->left()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->right()), helper);

    passErrorIfNot(lhs_type);
    passErrorIfNot(rhs_type);

    if (!(lhs_type->isNumber() && rhs_type->isNumber())) {
        // TODO: Support customed operator
        helper->error("Cannot compare non-numbers");
        return {nullptr, nullptr};
    }

    Type* merged_type = XSharp::getMergedType(lhs_type, rhs_type);
    passErrorIfNot(merged_type);

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

ValueAndType LessImpl(BinaryOperatorNode* op, CodeGenContextHelper* helper,
                      const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->left()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->right()), helper);

    passErrorIfNot(lhs_type);
    passErrorIfNot(rhs_type);

    if (!(lhs_type->isNumber() && rhs_type->isNumber())) {
        // TODO: Support customed operator
        helper->error("Cannot compare non-numbers");
        return {nullptr, nullptr};
    }

    Type* merged_type = XSharp::getMergedType(lhs_type, rhs_type);
    passErrorIfNot(merged_type);

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

ValueAndType GreaterOrEqualImpl(BinaryOperatorNode* op,
                                CodeGenContextHelper* helper,
                                const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->left()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->right()), helper);

    passErrorIfNot(lhs_type);
    passErrorIfNot(rhs_type);

    if (!(lhs_type->isNumber() && rhs_type->isNumber())) {
        // TODO: Support customed operator
        helper->error("Cannot compare non-numbers");
        return {nullptr, nullptr};
    }

    Type* merged_type = XSharp::getMergedType(lhs_type, rhs_type);
    passErrorIfNot(merged_type);

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

ValueAndType LessOrEqualImpl(BinaryOperatorNode* op,
                             CodeGenContextHelper* helper,
                             const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->left()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->right()), helper);

    passErrorIfNot(lhs_type);
    passErrorIfNot(rhs_type);

    if (!(lhs_type->isNumber() && rhs_type->isNumber())) {
        // TODO: Support customed operator
        helper->error("Cannot compare non-numbers");
        return {nullptr, nullptr};
    }

    Type* merged_type = XSharp::getMergedType(lhs_type, rhs_type);
    passErrorIfNot(merged_type);

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

ValueAndType LogicalAndImpl(BinaryOperatorNode* op,
                            CodeGenContextHelper* helper,
                            const Generator& generator)
{
    auto [lhs, lhs_type] = generator(op->left());
    auto [rhs, rhs_type] = generator(op->right());

    passErrorIfNot(lhs_type);
    passErrorIfNot(rhs_type);

    lhs = TypeAdapter::llvmConvert(lhs_type, XSharp::getBooleanType(), lhs);
    rhs = TypeAdapter::llvmConvert(rhs_type, XSharp::getBooleanType(), rhs);

    if (!lhs || !rhs) {
        helper->error("And operator can only be applied on two boolean");
        return {nullptr, nullptr};
    }

    return {helper->builder.CreateAnd(lhs, rhs), XSharp::getBooleanType()};
}

ValueAndType LogicalOrImpl(BinaryOperatorNode* op, CodeGenContextHelper* helper,
                           const Generator& generator)
{
    auto [lhs, lhs_type] = generator(op->left());
    auto [rhs, rhs_type] = generator(op->right());

    passErrorIfNot(lhs_type);
    passErrorIfNot(rhs_type);

    lhs = TypeAdapter::llvmConvert(lhs_type, XSharp::getBooleanType(), lhs);
    rhs = TypeAdapter::llvmConvert(rhs_type, XSharp::getBooleanType(), rhs);

    if (!lhs || !rhs) {
        helper->error("And operator can only be applied on two boolean");
        return {nullptr, nullptr};
    }

    return {helper->builder.CreateOr(lhs, rhs), XSharp::getBooleanType()};
}

}  // namespace LLVMCodeGen
}  // namespace XSharp
