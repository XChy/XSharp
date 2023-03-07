#include "XSharp/Types/TypeConverter.h"

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
                                      llvm::IRBuilder<>& builder,
                                      llvm::LLVMContext& context,
                                      llvm::Value* val)
{
    if (from->equals(to)) return val;

    return nullptr;
#endif
