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
    if (operand_type->isSigned()) {
        result_val = helper->builder.CreateSub(
            llvm::ConstantInt::get(helper->context,
                                   llvm::APInt(operand_type->bits(), 0)),
            operand);
    } else if (operand_type->isUnsigned()) {
        helper->error(
            "Cannot calculate the minus value of an unsigned integer");
        return {nullptr, nullptr};
    } else if (operand_type->basicType() == BasicType::Float) {
        result_val = helper->builder.CreateSub(
            llvm::ConstantFP::get(helper->context, llvm::APFloat(0.0f)),
            operand);
    } else if (operand_type->basicType() == BasicType::Double) {
        result_val = helper->builder.CreateSub(
            llvm::ConstantFP::get(helper->context, llvm::APFloat(double(0.0))),
            operand);
    }

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

ValueAndType NewImpl(UnaryOperatorNode* op, CodeGenContextHelper* helper,
                     const Generator& generator)
{
    auto node = op->operand()->to<TypeNode>();
    Type* type;

    if (node) type = node->toType();

    if (!node || !type) {
        helper->error(
            "The operand '{}' after 'new' operator must be a type-expression",
            op->operand()->dump());
        return {nullptr, nullptr};
    }

    auto& module = helper->module;
    auto& context = helper->context;
    llvm::FunctionCallee newFunc = module.getOrInsertFunction(
        "GC_new_object",
        llvm::FunctionType::get(castToLLVM(getReferenceType(type), context),
                                {llvm::Type::getInt64Ty(context)}, false));

    if (type->isBasic()) {
        // TODO: how to handle this case ?
        return {nullptr, nullptr};
    } else if (type->category == Type::Class) {
        auto sizeofObject =
            llvm::ConstantInt::get(context, llvm::APInt(64, type->size()));

        return {helper->builder.CreateCall(newFunc, {sizeofObject}),
                getReferenceType(type)};
    } else {
        helper->error("Do not support 'new' operator for {}", type->typeName());
        return {nullptr, nullptr};
    }
}
}  // namespace XSharp::LLVMCodeGen
