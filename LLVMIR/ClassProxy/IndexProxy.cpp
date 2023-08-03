#include "IndexProxy.h"
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
                                              CodeGenContext *helper,
                                              const Generator &generator)
{
    auto [index, index_type] = generator(ast->index());

    auto [indexed, indexed_type] =
        deReference(generator(ast->operand()), helper);

    if (!indexed_type->isArray()) {
        helper->error("Cannot get the element of non-array");
        return {nullptr, nullptr};
    }

    index = TypeAdapter::llvmConvert(index_type, Types::get("i64"), index);

    if (!index) {
        helper->error("Cannot convert '{}' to '{}'", index_type->typeName(),
                      Types::get("i64")->typeName());

        helper->error("Index must be an integer");
        return {nullptr, nullptr};
    }

    auto array_struct_type = indexed->getType()->getContainedType(0);

    llvm::Value *element_ptr_ptr =
        helper->llvm_builder.CreateStructGEP(array_struct_type, indexed, 1);

    llvm::Value *element_ptr_head = helper->llvm_builder.CreateLoad(
        element_ptr_ptr->getType()->getContainedType(0), element_ptr_ptr);

    llvm::Value *element = helper->llvm_builder.CreateInBoundsGEP(
        element_ptr_head->getType()->getContainedType(0), element_ptr_head,
        index, ast->dump().toStdString());

    return {element, getReferenceType(indexed_type->elementType())};
}
