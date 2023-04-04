#pragma once
#include "LLVMIR/LLVMTypes.h"
#include "XSharp/SymbolTable.h"
#include "XSharp/Types/Type.h"
#include "XSharp/XString.h"

namespace XSharp {

class SymbolTable;

namespace LLVMCodeGen {

void setUpBuildIn(llvm::Module& module, llvm::LLVMContext& context,
                  XSharp::SymbolTable& symbolTable);

void addBuildInFunction(const char* linked_name, const XString& name,
                        Type* func_type, llvm::Module& module,
                        llvm::LLVMContext& context, SymbolTable& symbols);

}  // namespace LLVMCodeGen
}  // namespace XSharp
