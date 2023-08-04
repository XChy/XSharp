#include "Utils.h"
#include <llvm/IR/Value.h>
#include "LLVMIR/LLVMTypes.h"
#include "XSharp/Types/Type.h"
#include "XSharp/Types/TypeSystem.h"
#include "XSharp/XString.h"

namespace XSharp::LLVMCodeGen {

llvm::Value* genMalloc(CodeGenContext* helper, XSharp::Type* type)
{
    auto& module = helper->module;
    auto& context = helper->llvm_ctx;
    llvm::FunctionCallee newFunc = module.getOrInsertFunction(
        "GC_new_object",
        llvm::FunctionType::get(castToLLVM(type, context),
                                {llvm::Type::getInt64Ty(context)}, false));

    if (type->isBasic()) {
        // TODO: how to handle this case ?
        return nullptr;
    } else if (type->category == Type::Class || type->isObject()) {
        auto sizeofObject =
            llvm::ConstantInt::get(context, llvm::APInt(64, type->size()));

        return helper->llvm_builder.CreateCall(newFunc, {sizeofObject});
    } else {
        helper->error("Do not support 'new' operator for {}", type->typeName());
        return nullptr;
    }
}

llvm::Value* genArrayMalloc(CodeGenContext* helper, XSharp::Type* type,
                            llvm::Value* element_count)
{
    auto& module = helper->module;
    auto& context = helper->llvm_ctx;
    llvm::FunctionCallee newFunc = module.getOrInsertFunction(
        "GC_new_object",
        llvm::FunctionType::get(
            castToLLVM(type->elementType(), context)->getPointerTo(),
            {llvm::Type::getInt64Ty(context)}, false));

    auto array_struct_type = structForArray(helper->llvm_ctx);

    if (type->isArray()) {
        llvm::Value* sizeofElement = llvm::ConstantInt::get(
            context, llvm::APInt(64, type->elementType()->size()));

        llvm::Value* sizeofArray =
            helper->llvm_builder.CreateMul(element_count, sizeofElement);

        llvm::Value* array_value =
            helper->llvm_builder.CreateAlloca(array_struct_type);

        auto elementsPtr =
            helper->llvm_builder.CreateCall(newFunc, {sizeofArray});

        auto lengthPtr = helper->llvm_builder.CreateStructGEP(
            array_struct_type, array_value, 0);

        auto elementsPtrPtr = helper->llvm_builder.CreateStructGEP(
            array_struct_type, array_value, 1);

        helper->llvm_builder.CreateStore(element_count, lengthPtr);
        helper->llvm_builder.CreateStore(elementsPtr, elementsPtrPtr);

        return array_value;

    } else {
        helper->error("Do not support 'new' operator for {}", type->typeName());
        return nullptr;
    }
}

}  // namespace XSharp::LLVMCodeGen
