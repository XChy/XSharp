#include "CodeGenHelper.h"
#include <cerrno>
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

CodeGenContextHelper::CodeGenContextHelper()
    : module("XSharp", context), builder(context), optimizer(&module)
{
    currentSymbols = &globalSymbols;

    module.setTargetTriple(LLVM_DEFAULT_TARGET_TRIPLE);
}

XSharp::SymbolTable* CodeGenContextHelper::enterScope()
{
    auto newScopeSymbolTable = currentSymbols->createChild();
    currentSymbols = newScopeSymbolTable;
    return currentSymbols;
}

XSharp::SymbolTable* CodeGenContextHelper::exitScope()
{
    if (isGlobalScope()) return nullptr;
    currentSymbols = currentSymbols->parent();
    return currentSymbols;
}

void CodeGenContextHelper::toNewFunctionScope(const XSharp::Symbol& funcSymbol)
{
    currentReturnType = funcSymbol.type->returnValueType();
    enterScope();
}

bool CodeGenContextHelper::isGlobalScope() const
{
    return currentSymbols == &globalSymbols;
}

ValueAndType deReference(ValueAndType ref, CodeGenContextHelper* helper)
{
    auto [ref_val, ref_type] = ref;
    if (!ref_type) {
        return {nullptr, nullptr};
    }
    if (ref_type->category == XSharp::Type::Reference) {
        return {
            helper->builder.CreateLoad(
                castToLLVM(ref_type->derefType(), helper->context), ref_val),
            ref_type->derefType()};
    } else {
        return ref;
    }
}
