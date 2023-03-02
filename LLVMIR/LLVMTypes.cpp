#include "LLVMIR/LLVMTypes.h"
#include <llvm-14/llvm/IR/DerivedTypes.h>
#include <llvm-14/llvm/IR/LLVMContext.h>
#include <llvm-14/llvm/IR/Type.h>
#include <vector>
#include "XSharp/ASTNodes.h"
#include "XSharp/Type.h"

llvm::Type* llvmTypeFor(XSharp::TypeNode* type, llvm::LLVMContext& context)
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
            for (TypeNode* paramTypeNode : type->paramsType())
                llvmTypesForParams.push_back(
                    llvmTypeFor(paramTypeNode, context));

            return llvm::FunctionType::get(llvmTypeFor(type, context),
                                           llvmTypesForParams, false);
        }
        case TypeNode::Array:
            // Allocate XSharp's array on heap
            // So the type of array is the pointer type of its element
            return llvm::PointerType::get(
                llvmTypeFor(type->elementType(), context), 0);
        case TypeNode::Class:
            // TODO: Complete the related definition of class
            return llvm::StructType::get(context, std::vector<llvm::Type*>());
        case TypeNode::Closure:
            break;
        default:
            return nullptr;
    }
}
