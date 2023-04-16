#include "CodeGenHelper.h"
#include <cerrno>
#include "LLVMIR/Target.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

namespace XSharp::LLVMCodeGen {

CodeGenContext::CodeGenContext()
    : module("XSharp", llvm_ctx), llvm_builder(llvm_ctx), optimizer(&module)
{
    currentSymbols = &globalSymbols;

    module.setTargetTriple(target_triple());
    module.setDataLayout(data_layout());
}

void CodeGenContext::optimize() { optimizer.modulePassManager.run(module); }
XSharp::SymbolTable* CodeGenContext::enterScope()
{
    auto newScopeSymbolTable = currentSymbols->createChild();
    currentSymbols = newScopeSymbolTable;
    return currentSymbols;
}

XSharp::SymbolTable* CodeGenContext::exitScope()
{
    if (isGlobalScope()) return nullptr;
    currentSymbols = currentSymbols->parent();
    return currentSymbols;
}

void CodeGenContext::enterFunctionScope(const XSharp::Symbol& funcSymbol)
{
    retTypes.push(funcSymbol.type->returnValueType());
    enterScope();
}

void CodeGenContext::exitFunctionScope()
{
    retTypes.pop();
    exitScope();
}

Type* CodeGenContext::currentRetType() const { return retTypes.top(); }

bool CodeGenContext::isGlobalScope() const
{
    return currentSymbols == &globalSymbols;
}

ValueAndType deReference(ValueAndType ref, CodeGenContext* helper)
{
    auto [ref_val, ref_type] = ref;
    if (!ref_type) {
        return {nullptr, nullptr};
    }
    if (ref_type->category == XSharp::Type::Reference) {
        return {
            helper->llvm_builder.CreateLoad(
                castToLLVM(ref_type->derefType(), helper->llvm_ctx), ref_val),
            ref_type->derefType()};
    } else {
        return ref;
    }
}
}  // namespace XSharp::LLVMCodeGen
