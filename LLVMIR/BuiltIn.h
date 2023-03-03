#include "LLVMIR/LLVMTypes.h"
#include "XSharp/SymbolTable.h"

namespace XSharp {
class SymbolTable;
}

void setUpBuildIn(llvm::Module& module, llvm::LLVMContext& context,
                  XSharp::SymbolTable& symbolTable);
