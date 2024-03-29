#include "LogicalOpImpl.h"
#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/Utils.h"
#include "XSharp/Types/TypeSystem.h"
#include "XSharp/XString.h"
namespace XSharp {
namespace LLVMCodeGen {

ValueAndType EqualImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                       const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->lhs()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->rhs()), helper);

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
        return {helper->llvm_builder.CreateICmpEQ(lhs, rhs),
                XSharp::getBooleanType()};
    else
        return {helper->llvm_builder.CreateFCmpUEQ(lhs, rhs),
                XSharp::getBooleanType()};
}

ValueAndType NotEqualImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                          const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->lhs()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->rhs()), helper);

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
        return {helper->llvm_builder.CreateICmpNE(lhs, rhs),
                XSharp::getBooleanType()};
    else
        return {helper->llvm_builder.CreateFCmpUNE(lhs, rhs),
                XSharp::getBooleanType()};
}
ValueAndType GreaterImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                         const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->lhs()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->rhs()), helper);

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
        return {helper->llvm_builder.CreateICmpSGT(lhs, rhs),
                XSharp::getBooleanType()};
    else if (merged_type->isUnsigned())
        return {helper->llvm_builder.CreateICmpUGT(lhs, rhs),
                XSharp::getBooleanType()};
    else
        return {helper->llvm_builder.CreateFCmpUGT(lhs, rhs),
                XSharp::getBooleanType()};
}

ValueAndType LessImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                      const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->lhs()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->rhs()), helper);

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
        return {helper->llvm_builder.CreateICmpSLT(lhs, rhs),
                XSharp::getBooleanType()};
    else if (merged_type->isUnsigned())
        return {helper->llvm_builder.CreateICmpULT(lhs, rhs),
                XSharp::getBooleanType()};
    else
        return {helper->llvm_builder.CreateFCmpULT(lhs, rhs),
                XSharp::getBooleanType()};
}

ValueAndType GreaterOrEqualImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                                const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->lhs()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->rhs()), helper);

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
        return {helper->llvm_builder.CreateICmpSGE(lhs, rhs),
                XSharp::getBooleanType()};
    else if (merged_type->isUnsigned())
        return {helper->llvm_builder.CreateICmpUGE(lhs, rhs),
                XSharp::getBooleanType()};
    else
        return {helper->llvm_builder.CreateFCmpUGE(lhs, rhs),
                XSharp::getBooleanType()};
}

ValueAndType LessOrEqualImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                             const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->lhs()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->rhs()), helper);

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
        return {helper->llvm_builder.CreateICmpSLE(lhs, rhs),
                XSharp::getBooleanType()};
    else if (merged_type->isUnsigned())
        return {helper->llvm_builder.CreateICmpULE(lhs, rhs),
                XSharp::getBooleanType()};
    else
        return {helper->llvm_builder.CreateFCmpULE(lhs, rhs),
                XSharp::getBooleanType()};
}

ValueAndType LogicalAndImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                            const Generator& generator)
{
    auto [lhs, lhs_type] = generator(op->lhs());
    auto [rhs, rhs_type] = generator(op->rhs());

    passErrorIfNot(lhs_type);
    passErrorIfNot(rhs_type);

    lhs = TypeAdapter::llvmConvert(lhs_type, XSharp::getBooleanType(), lhs);
    rhs = TypeAdapter::llvmConvert(rhs_type, XSharp::getBooleanType(), rhs);

    if (!lhs || !rhs) {
        helper->error("And operator can only be applied on two boolean");
        return {nullptr, nullptr};
    }

    return {helper->llvm_builder.CreateAnd(lhs, rhs), XSharp::getBooleanType()};
}

ValueAndType LogicalOrImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                           const Generator& generator)
{
    auto [lhs, lhs_type] = generator(op->lhs());
    auto [rhs, rhs_type] = generator(op->rhs());

    passErrorIfNot(lhs_type);
    passErrorIfNot(rhs_type);

    lhs = TypeAdapter::llvmConvert(lhs_type, XSharp::getBooleanType(), lhs);
    rhs = TypeAdapter::llvmConvert(rhs_type, XSharp::getBooleanType(), rhs);

    if (!lhs || !rhs) {
        helper->error("And operator can only be applied on two boolean");
        return {nullptr, nullptr};
    }

    return {helper->llvm_builder.CreateOr(lhs, rhs), XSharp::getBooleanType()};
}

}  // namespace LLVMCodeGen
}  // namespace XSharp
