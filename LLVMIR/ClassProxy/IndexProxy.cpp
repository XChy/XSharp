#include "IndexProxy.h"
#include <llvm-14/llvm/IR/Value.h>
#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/LLVMTypes.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Types/TypeAdapter.h"
#include "XSharp/Types/TypeSystem.h"
#include "XSharp/XString.h"

using namespace XSharp::LLVMCodeGen;
using namespace XSharp;

ValueAndType CodeGenProxy<IndexNode>::codeGen(IndexNode *ast,
                                              CodeGenContextHelper *helper,
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

    llvm::Value *element = helper->builder.CreateInBoundsGEP(
        castToLLVM(indexed_type->elementType(), helper->context), indexed,
        index, ast->dump().toStdString());
    return {element, getReferenceType(indexed_type->elementType())};
}
