#include "LLVMIR/LLVMTypes.h"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <cstddef>
#include <vector>
#include "XSharp/Class/XClass.h"
#include "XSharp/Types/TypeSystem.h"

namespace XSharp {
namespace LLVMCodeGen {

llvm::Type* castToLLVM(XSharp::Type* type, llvm::LLVMContext& context)
{
    std::byte a;
    using XSharp::BasicType;
    using XSharp::Type;
    using llvm::SmallVector;

    switch (type->category) {
        case Type::Basic:
            switch (type->basicType()) {
                case BasicType::Void:
                    return llvm::Type::getVoidTy(context);
                case BasicType::Boolean:
                    return llvm::Type::getInt1Ty(context);
                case BasicType::Char:
                    return llvm::Type::getInt16Ty(context);
                case BasicType::I8:
                case BasicType::U8:
                    return llvm::Type::getInt8Ty(context);
                case BasicType::I16:
                case BasicType::U16:
                    return llvm::Type::getInt16Ty(context);
                case BasicType::I32:
                case BasicType::U32:
                    return llvm::Type::getInt32Ty(context);
                case BasicType::I64:
                case BasicType::U64:
                    return llvm::Type::getInt64Ty(context);
                case BasicType::Float:
                    return llvm::Type::getFloatTy(context);
                case BasicType::Double:
                    return llvm::Type::getDoubleTy(context);
                default:
                    return nullptr;
            }

        case Type::Function: {
            SmallVector<llvm::Type*> llvmTypesForParams;
            for (XSharp::Type* paramType : type->parameterTypes())
                llvmTypesForParams.push_back(castToLLVM(paramType, context));

            return llvm::FunctionType::get(
                castToLLVM(type->returnValueType(), context),
                llvmTypesForParams, false);
        }

        case Type::Class: {
            std::vector<llvm::Type*> llvmTypes;

            // Class Pointer for reflection
            llvmTypes.push_back(
                llvm::Type::getIntNTy(context, sizeof(uintptr_t) * 8));

            for (auto fieid : type->getClassDecl()->dataFields)
                llvmTypes.push_back(castToLLVM(fieid.type, context));

            auto structType = llvm::StructType::get(context, llvmTypes);
            return structType;
        }

        case Type::Array:
        case Type::Closure:
        case Type::Reference:
            return llvm::PointerType::get(context, 0);
        default:
            return nullptr;
    }

    return nullptr;
}

llvm::StructType* structForArray(llvm::LLVMContext& context)
{
    // Allocate XSharp's array on heap
    // So the type of array is the pointer type of its element

    return llvm::StructType::get(context,
                                 {castToLLVM(getArrayLenType(), context),
                                  llvm::PointerType::get(context, 0)});
}

}  // namespace LLVMCodeGen
}  // namespace XSharp
