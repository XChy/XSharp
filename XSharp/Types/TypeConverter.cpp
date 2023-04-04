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
        return builder->CreateIntCast(
            val, LLVMCodeGen::castToLLVM(to, *context), from->isSigned());
    }

    // TODO: integer and float-point
    if (from->isInteger() && !to->isInteger()) {
        if (from->isSigned())
            return builder->CreateSIToFP(val,
                                         LLVMCodeGen::castToLLVM(to, *context));
        else
            return builder->CreateUIToFP(val,
                                         LLVMCodeGen::castToLLVM(to, *context));
    } else if (!from->isInteger() && to->isInteger()) {
        if (from->isSigned())
            return builder->CreateFPToSI(val,
                                         LLVMCodeGen::castToLLVM(to, *context));
        else
            return builder->CreateFPToUI(val,
                                         LLVMCodeGen::castToLLVM(to, *context));
    }

    // float-point to float-point
    if (!from->isInteger() && !to->isInteger()) {
        builder->CreateFPCast(val, LLVMCodeGen::castToLLVM(to, *context));
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

bool ArrayConverter::convertable(Type* from, Type* to)
{
    return from->isArray() && to->isArray();
}

bool ArrayConverter::implicitConvertable(Type* from, Type* to)
{
    // TODO: tell apart explict and implicit conversions
    return convertable(from, to);
}

#ifdef XSharp_LLVMIR_SUPPORT
llvm::Value* ArrayConverter::convert(Type* from, Type* to,
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

bool EqualConverter::convertable(Type* from, Type* to)
{
    return from->equals(to);
}

bool EqualConverter::implicitConvertable(Type* from, Type* to)
{
    // TODO: tell apart explict and implicit conversions
    return convertable(from, to);
}

#ifdef XSharp_LLVMIR_SUPPORT
llvm::Value* EqualConverter::convert(Type* from, Type* to,
                                     llvm::IRBuilder<>* builder,
                                     llvm::LLVMContext* context,
                                     llvm::Value* val)
{
    if (from->equals(to)) return val;

    return nullptr;
}

#endif
