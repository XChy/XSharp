#include "ArithmeticOpImpl.h"
#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/LLVMTypes.h"
#include "XSharp/Types/Type.h"
#include "XSharp/Types/TypeSystem.h"
namespace XSharp {
namespace LLVMCodeGen {

ValueAndType AssignImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                        const Generator& generator)
{
    auto [lhs, lhs_type] = generator(op->left());
    auto [rhs, rhs_type] = generator(op->right());
    if (!lhs_type || !rhs_type) return {nullptr, nullptr};
    // Only the reference can be assigned to value
    if (lhs_type->category != Type::Reference || lhs_type->isObject()) {
        helper->error("Cannot assign value to non-reference");
        return {nullptr, nullptr};
    }

    // TODO: ObjectRef's assignment
    rhs = TypeAdapter::llvmConvert(rhs_type, lhs_type->derefType(), rhs);
    if (!rhs_type) {
        helper->error("The type '{}' cannot be converted to '{}'",
                      rhs_type->typeName(), lhs_type->derefType()->typeName());
        return {nullptr, nullptr};
    }
    helper->llvm_builder.CreateStore(rhs, lhs);
    return {lhs, lhs_type};
}

ValueAndType AddImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                     const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->left()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->right()), helper);

    if (!lhs_type || !rhs_type) return {nullptr, nullptr};
    if (!(lhs_type->isNumber() && rhs_type->isNumber())) {
        // TODO: Support customed operator
        helper->error("Cannot add non-numbers");
        return {nullptr, nullptr};
    }

    Type* merged_type = getMergedType(lhs_type, rhs_type);

    lhs = TypeAdapter::llvmConvert(lhs_type, merged_type, lhs);
    rhs = TypeAdapter::llvmConvert(rhs_type, merged_type, rhs);

    if (merged_type->isInteger())
        return {helper->llvm_builder.CreateAdd(lhs, rhs), merged_type};
    else
        return {helper->llvm_builder.CreateFAdd(lhs, rhs), merged_type};
}

ValueAndType SubImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                     const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->left()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->right()), helper);
    if (!lhs_type || !rhs_type) return {nullptr, nullptr};

    if (!(lhs_type->isNumber() && rhs_type->isNumber())) {
        // TODO: Support customed operator
        helper->error("Cannot subtract non-numbers");
        return {nullptr, nullptr};
    }

    Type* merged_type = getMergedType(lhs_type, rhs_type);

    lhs = TypeAdapter::llvmConvert(lhs_type, merged_type, lhs);
    rhs = TypeAdapter::llvmConvert(rhs_type, merged_type, rhs);

    return {helper->llvm_builder.CreateSub(lhs, rhs), merged_type};
}

ValueAndType MulImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                     const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->left()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->right()), helper);
    if (!lhs_type || !rhs_type) return {nullptr, nullptr};

    if (!(lhs_type->isNumber() && rhs_type->isNumber())) {
        // TODO: Support customed operator
        helper->error("Cannot multiply non-numbers");
        return {nullptr, nullptr};
    }

    Type* merged_type = getMergedType(lhs_type, rhs_type);

    lhs = TypeAdapter::llvmConvert(lhs_type, merged_type, lhs);
    rhs = TypeAdapter::llvmConvert(rhs_type, merged_type, rhs);

    return {helper->llvm_builder.CreateMul(lhs, rhs), merged_type};
}

ValueAndType DivImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                     const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->left()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->right()), helper);
    if (!lhs_type || !rhs_type) return {nullptr, nullptr};

    if (!(lhs_type->isNumber() && rhs_type->isNumber())) {
        // TODO: Support customed operator
        helper->error("Cannot divide non-numbers");
        return {nullptr, nullptr};
    }

    Type* merged_type = getMergedType(lhs_type, rhs_type);

    lhs = TypeAdapter::llvmConvert(lhs_type, merged_type, lhs);
    rhs = TypeAdapter::llvmConvert(rhs_type, merged_type, rhs);

    if (merged_type->isSigned())
        return {helper->llvm_builder.CreateSDiv(lhs, rhs), merged_type};
    else if (merged_type->isUnsigned())
        return {helper->llvm_builder.CreateUDiv(lhs, rhs), merged_type};
    else
        return {helper->llvm_builder.CreateFDiv(lhs, rhs), merged_type};
}
ValueAndType ModImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                     const Generator& generator)
{
    auto [lhs, lhs_type] = deReference(generator(op->left()), helper);
    auto [rhs, rhs_type] = deReference(generator(op->right()), helper);
    if (!lhs_type || !rhs_type) return {nullptr, nullptr};

    if (!(lhs_type->isNumber() && rhs_type->isNumber())) {
        // TODO: Support customed operator
        helper->error("Cannot mod non-numbers");
        return {nullptr, nullptr};
    }

    Type* merged_type = getMergedType(lhs_type, rhs_type);

    lhs = TypeAdapter::llvmConvert(lhs_type, merged_type, lhs);
    rhs = TypeAdapter::llvmConvert(rhs_type, merged_type, rhs);

    if (merged_type->isSigned())
        return {helper->llvm_builder.CreateSRem(lhs, rhs), merged_type};
    else if (merged_type->isUnsigned())
        return {helper->llvm_builder.CreateURem(lhs, rhs), merged_type};
    else
        return {helper->llvm_builder.CreateFRem(lhs, rhs), merged_type};
}

}  // namespace LLVMCodeGen
}  // namespace XSharp
