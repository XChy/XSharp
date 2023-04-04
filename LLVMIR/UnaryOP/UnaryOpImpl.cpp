#include "UnaryOpImpl.h"
#include <llvm-14/llvm/ADT/APInt.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include "LLVMIR/LLVMTypes.h"
#include "XSharp/Types/TypeAdapter.h"
#include "XSharp/Types/TypeNodes.h"
#include "XSharp/Types/TypeSystem.h"

namespace XSharp::LLVMCodeGen {

ValueAndType PositiveImpl(UnaryOperatorNode* op, CodeGenContextHelper* helper,
                          const Generator& generator)
{
    auto [operand, operand_type] =
        deReference(generator(op->operand()), helper);

    if (!operand_type) return {nullptr, nullptr};

    if (!operand_type->isNumber()) {
        // TODO: Support customed operator
        helper->error("Cannot get the positive value of non-numbers");
        return {nullptr, nullptr};
    }

    return {operand, operand_type};
}

ValueAndType NegativeImpl(UnaryOperatorNode* op, CodeGenContextHelper* helper,
                          const Generator& generator)
{
    auto [operand, operand_type] =
        deReference(generator(op->operand()), helper);

    if (!operand_type) return {nullptr, nullptr};

    if (!operand_type->isNumber()) {
        // TODO: Support customed operator
        helper->error("Cannot get the positive value of non-numbers");
        return {nullptr, nullptr};
    }

    llvm::Value* result_val;
    result_val = helper->builder.CreateNeg(operand);
    return {result_val, operand_type};
}

ValueAndType NotImpl(UnaryOperatorNode* op, CodeGenContextHelper* helper,
                     const Generator& generator)
{
    auto [operand, operand_type] =
        deReference(generator(op->operand()), helper);

    if (!operand_type) return {nullptr, nullptr};

    operand = TypeAdapter::llvmConvert(operand_type, getBooleanType(), operand);

    if (!operand) {
        // TODO: Support customed operator
        helper->error("Cannot convert '{}' to '{}'", operand_type->typeName(),
                      getBooleanType()->typeName());
        return {nullptr, nullptr};
    }

    return {helper->builder.CreateNot(operand), operand_type};
}
}  // namespace XSharp::LLVMCodeGen
