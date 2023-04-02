#pragma once
#include <llvm/IR/LegacyPassManager.h>

namespace XSharp {
namespace LLVMCodeGen {

class Optimizer
{
   public:
    Optimizer(llvm::Module* module);

    llvm::legacy::FunctionPassManager functionPassManager;
};

}  // namespace LLVMCodeGen
}  // namespace XSharp
