#include "UnaryOpImpl.h"
#include <llvm-14/llvm/IR/Constant.h>
#include <llvm-14/llvm/IR/Constants.h>
#include <llvm-14/llvm/IR/Function.h>
#include <llvm-14/llvm/IR/Type.h>
#include "LLVMIR/LLVMTypes.h"
#include "XSharp/Types/TypeNodes.h"
#include "XSharp/Types/TypeSystem.h"
using namespace XSharp;

ValueAndType XSharp::PositiveImpl(UnaryOperatorNode* op,
                                  CodeGenContextHelper* helper,
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

ValueAndType XSharp::NegativeImpl(UnaryOperatorNode* op,
                                  CodeGenContextHelper* helper,
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
    // TODO: impliment the minus LLVMIR
    return {operand, operand_type};
}

ValueAndType XSharp::NotImpl(UnaryOperatorNode* op,
                             CodeGenContextHelper* helper,
                             const Generator& generator)
{
    auto [operand, operand_type] =
        deReference(generator(op->operand()), helper);

    if (!operand_type) return {nullptr, nullptr};
    if (operand_type->basicType() == BasicType::Boolean) {
        // TODO: Support customed operator
        helper->error("Cannot get the positive value of non-numbers");
        return {nullptr, nullptr};
    }
    // TODO: impliment the minus LLVMIR
    return {operand, operand_type};
}

ValueAndType XSharp::NewImpl(UnaryOperatorNode* op,
                             CodeGenContextHelper* helper,
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
