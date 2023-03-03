#include "XSharp/Semantics.h"
using namespace XSharp;

Semantics::Semantics() {}

Semantics::Semantics(ASTNode* ast) { analyze(ast); }

void Semantics::analyze(ASTNode* ast)
{
    // TODO: complete Semantic Analysis
}

std::vector<std::byte> Semantics::getXSharpIR()
{
    // TODO: generate XSharp's IR
}

std::vector<std::byte> Semantics::getLLVMIR(){
#ifdef XSharp_LLVMIR_SUPPORT
// TODO: generate LLVM IR
#endif
}

XSharpError Semantics::error()
{
    return _error;
}

SymbolTable Semantics::rootSymbols() { return rootSymbolTable; }
