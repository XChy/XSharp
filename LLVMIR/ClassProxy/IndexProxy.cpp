#include "IndexProxy.h"
#include <llvm/IR/Constants.h>
#include <llvm/IR/Value.h>
#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/LLVMTypes.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Types/TypeAdapter.h"
#include "XSharp/Types/TypeSystem.h"
#include "XSharp/XString.h"

using namespace XSharp::LLVMCodeGen;
using namespace XSharp;

ValueAndType CodeGenProxy<IndexNode>::codeGen(IndexNode *ast,
                                              CodeGenContext *ctx,
                                              const Generator &generator)
{
    using llvm::ConstantInt;
    auto [index, index_type] = generator(ast->index());

    auto [indexed, indexed_type] =
        deReference(generator(ast->operand()), ctx);

    if (!indexed_type->isArray()) {
        ctx->error("Cannot get the element of non-array");
        return {nullptr, nullptr};
    }

    index = TypeAdapter::llvmConvert(index_type, Types::get("i64"), index);

    if (!index) {
        ctx->error("Cannot convert '{}' to '{}'", index_type->typeName(),
                      Types::get("i64")->typeName());

        ctx->error("Index must be an integer");
        return {nullptr, nullptr};
    }

    auto PtrTy = indexed->getType();

    llvm::Value *element_ptr_ptr = ctx->llvm_builder.CreateStructGEP(
        structForArray(ctx->llvm_ctx), indexed, 1);

    llvm::Value *element_ptr_head =
        ctx->llvm_builder.CreateLoad(PtrTy, element_ptr_ptr);

    llvm::Value *element = ctx->llvm_builder.CreateGEP(
        castToLLVM(indexed_type->elementType(), ctx->llvm_ctx),
        element_ptr_head, index, ast->dump().toStdString());

    return {element, getReferenceType(indexed_type->elementType())};
}
