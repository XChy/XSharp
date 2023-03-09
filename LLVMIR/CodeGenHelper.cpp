#include "CodeGenHelper.h"

CodeGenContextHelper::CodeGenContextHelper()
    : module("XSharp", context), builder(context), optimizer(&module)
{
    currentSymbols = &globalSymbols;
}

XSharp::SymbolTable* CodeGenContextHelper::toNewScope() { return nullptr; }

XSharp::SymbolTable* CodeGenContextHelper::toParentScope() { return nullptr; }

bool CodeGenContextHelper::isGlobalScope() const
{
    return currentSymbols == &globalSymbols;
}
