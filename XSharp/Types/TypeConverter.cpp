#include "XSharp/Types/TypeConverter.h"
#include <llvm-14/llvm/IR/Instruction.h>
#include <llvm-14/llvm/IR/Type.h>
#include "LLVMIR/LLVMTypes.h"

using namespace XSharp;

bool NumberConverter::convertable(Type* from, Type* to)
{
    return from->isNumber() && to->isNumber();
}

bool NumberConverter::implicitConvertable(Type* from, Type* to)
{
    // TODO: tell apart explict and implicit conversions
    return convertable(from, to);
}

#ifdef XSharp_LLVMIR_SUPPORT
llvm::Value* NumberConverter::convert(Type* from, Type* to,
                                      llvm::IRBuilder<>* builder,
                                      llvm::LLVMContext* context,
                                      llvm::Value* val)
{
    if (from->equals(to)) return val;

    if (!convertable(from, to)) return nullptr;

    // integer to integer
    if (from->isInteger() && to->isInteger()) {
        return builder->CreateIntCast(val, castToLLVM(to, *context),
                                      from->isSigned());
    }

    // TODO: integer and float-point
    if (from->isInteger() && !to->isInteger()) {
        if (from->isSigned())
            return builder->CreateSIToFP(val, castToLLVM(to, *context));
        else
            return builder->CreateUIToFP(val, castToLLVM(to, *context));
    } else if (!from->isInteger() && to->isInteger()) {
        if (from->isSigned())
            return builder->CreateFPToSI(val, castToLLVM(to, *context));
        else
            return builder->CreateFPToUI(val, castToLLVM(to, *context));
    }

    // float-point to float-point
    if (!from->isInteger() && !to->isInteger()) {
        builder->CreateFPCast(val, castToLLVM(to, *context));
    }

    return nullptr;
}
#endif

bool ObjectConverter::convertable(Type* from, Type* to)
{
    return from->isObject() && to->isObject();
}

bool ObjectConverter::implicitConvertable(Type* from, Type* to)
{
    // TODO: tell apart explict and implicit conversions
    return convertable(from, to);
}

#ifdef XSharp_LLVMIR_SUPPORT
llvm::Value* ObjectConverter::convert(Type* from, Type* to,
                                      llvm::IRBuilder<>* builder,
                                      llvm::LLVMContext* context,
                                      llvm::Value* val)
{
    if (from->equals(to)) return val;

    if (!convertable(from, to)) return nullptr;

    // TODO: handle the cases where one extends another one

    return nullptr;
}
#endif
