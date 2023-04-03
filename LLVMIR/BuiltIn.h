#pragma once
#include "LLVMIR/LLVMTypes.h"
#include "XSharp/SymbolTable.h"

namespace XSharp {

class SymbolTable;

namespace LLVMCodeGen {

void setUpBuildIn(llvm::Module& module, llvm::LLVMContext& context,
                  XSharp::SymbolTable& symbolTable);

}

}  // namespace XSharp
