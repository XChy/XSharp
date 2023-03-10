#include "CodeGenHelper.h"

CodeGenContextHelper::CodeGenContextHelper()
    : module("XSharp", context), builder(context), optimizer(&module)
{
    currentSymbols = &globalSymbols;
}

XSharp::SymbolTable* CodeGenContextHelper::toNewScope()
{
    auto newScopeSymbolTable = currentSymbols->createChild();
    currentSymbols = newScopeSymbolTable;
    return currentSymbols;
}

XSharp::SymbolTable* CodeGenContextHelper::toParentScope()
{
    if (isGlobalScope()) return nullptr;
    currentSymbols = currentSymbols->parent();
    return currentSymbols;
}

void CodeGenContextHelper::toNewFunctionScope(const XSharp::Symbol& funcSymbol)
{
    currentReturnType = funcSymbol.type->returnValueType();
    toNewScope();
}

bool CodeGenContextHelper::isGlobalScope() const
{
    return currentSymbols == &globalSymbols;
}
