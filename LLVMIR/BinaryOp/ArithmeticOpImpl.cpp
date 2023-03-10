#include "ArithmeticOpImpl.h"
#include <cerrno>
#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/LLVMTypes.h"
#include "XSharp/Types/Type.h"
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

    TypeNode* merged_type;

    if (!(lhs_type->isNumber() && rhs_type->isNumber())) {
        // TODO: Support customed operator
        helper->error("Cannot add non-numbers");
        return {nullptr, nullptr};
    }

    if (lhs_type->isInteger() && rhs_type->isInteger()) {
        if ((lhs_type->isSigned() && rhs_type->isSigned()) ||
            (lhs_type->isUnsigned() && rhs_type->isUnsigned())) {
            merged_type =
                lhs_type->size() > rhs_type->size() ? lhs_type : rhs_type;
        } else {
            merged_type = lhs_type->isSigned() ? lhs_type : rhs_type;
        }
    } else if (lhs_type->isInteger() || rhs_type->isInteger()) {
        merged_type = lhs_type->isInteger() ? rhs_type : lhs_type;
    } else {
        merged_type = lhs_type->size() > rhs_type->size() ? lhs_type : rhs_type;
    }
    lhs = TypeAdapter::llvmConvert(lhs_type, merged_type, lhs);
    rhs = TypeAdapter::llvmConvert(rhs_type, merged_type, rhs);

    return {helper->builder.CreateAdd(lhs, rhs), merged_type};
}

ValueAndType XSharp::deReference(ValueAndType ref, CodeGenContextHelper* helper)
{
    auto [ref_val, ref_type] = ref;
    if (ref_type->category == XSharp::TypeNode::Reference) {
        return {
            helper->builder.CreateLoad(
                castToLLVM(ref_type->innerType(), helper->context), ref_val),
            ref_type->innerType()};
    } else {
        return ref;
    }
}
