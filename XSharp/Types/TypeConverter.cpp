#include "XSharp/Types/TypeConverter.h"
#include <llvm-14/llvm/IR/Instruction.h>
#include <llvm-14/llvm/IR/Type.h>
#include "LLVMIR/LLVMTypes.h"

using namespace XSharp;

bool NumberConverter::convertable(TypeNode* from, TypeNode* to)
{
    return isNumber(from) && isNumber(to);
}

bool NumberConverter::implicitConvertable(TypeNode* from, TypeNode* to)
{
    // TODO: tell apart explict and implicit conversions
    return convertable(from, to);
}

bool NumberConverter::isNumber(TypeNode* type) const
{
    if (type->category == TypeNode::Basic) {
        if (type->basicType() == BasicType::I32 ||
            type->basicType() == BasicType::I64 ||
            type->basicType() == BasicType::UI32 ||
            type->basicType() == BasicType::UI64 ||
            type->basicType() == BasicType::Float ||
            type->basicType() == BasicType::Double) {
            return true;
        }
    }
    return false;
}

#ifdef XSharp_LLVMIR_SUPPORT
llvm::Value* NumberConverter::convert(TypeNode* from, TypeNode* to,
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
