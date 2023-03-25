#include "LLVMIR/LLVMTypes.h"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include "XSharp/Class/XClass.h"

llvm::Type* castToLLVM(XSharp::TypeNode* type, llvm::LLVMContext& context)
{
    using XSharp::BasicType;
    // TODO complete XSharp's Type to Variable
    using XSharp::TypeNode;
    switch (type->category) {
        case TypeNode::Basic:
            switch (type->basicType()) {
                case BasicType::Void:
                    return llvm::Type::getVoidTy(context);
                case BasicType::Boolean:
                    return llvm::Type::getInt1Ty(context);
                case BasicType::Char:
                    return llvm::Type::getInt16Ty(context);
                case BasicType::I32:
                case BasicType::UI32:
                    return llvm::Type::getInt32Ty(context);
                case BasicType::I64:
                case BasicType::UI64:
                    return llvm::Type::getInt64Ty(context);
                case BasicType::Float:
                    return llvm::Type::getFloatTy(context);
                case BasicType::Double:
                    return llvm::Type::getDoubleTy(context);
                default:
                    return nullptr;
            }

        case TypeNode::Function: {
            std::vector<llvm::Type*> llvmTypesForParams;
            for (TypeNode* paramTypeNode : type->parameterTypes())
                llvmTypesForParams.push_back(
                    castToLLVM(paramTypeNode, context));

            return llvm::FunctionType::get(
                castToLLVM(type->returnValueType(), context),
                llvmTypesForParams, false);
        }

        case TypeNode::Array:
            // Allocate XSharp's array on heap
            // So the type of array is the pointer type of its element
            return llvm::PointerType::get(
                castToLLVM(type->elementType(), context), 0);

        case TypeNode::Class: {
            // TODO: Complete the related definition of class
            std::vector<llvm::Type*> llvmTypes;
            // Class Pointer for reflection

            llvmTypes.push_back(
                llvm::Type::getIntNTy(context, sizeof(uintptr_t) * 8));

            for (auto fieid : type->getObjectClass()->dataFields) {
                llvmTypes.push_back(castToLLVM(fieid.type, context));
            }

            auto valueType = llvm::StructType::get(context, llvmTypes);
            auto refType = valueType->getPointerTo();
            return refType;
        }

        case TypeNode::Closure:
            break;

        case TypeNode::Reference:
            return llvm::PointerType::get(
                castToLLVM(type->innerType(), context), 0);
        default:
            return nullptr;
    }
    return nullptr;
}
