#pragma once
#include <cstddef>
#include "XSharp/ASTNodes.h"
#include "xsharp_global.h"
#include "XSharp/XSharpUtils.h"
#include "SymbolTable.h"

#ifdef XSharp_LLVM_SUPPORT
#include "LLVMIR/LLVMHelper.h"
#endif

namespace XSharp {

class XSharp_EXPORT Semantics
{
   public:
    Semantics();
    Semantics(ASTNode* ast);

    void analyze(ASTNode* ast);

    /*
     * Function: getXSharpIR
     * Description: return intermedia codes which XSharp's VM can execute
     */
    std::vector<std::byte> getXSharpIR();

    /*
     * Function: getLLVMIR
     * Description: return intermedia codes which LLVM can compile into binary
     */
    std::vector<std::byte> getLLVMIR();

    XSharpError error();
    SymbolTable rootSymbols();

   private:
    SymbolTable rootSymbolTable;
    XSharpError _error;
};

}  // namespace XSharp
