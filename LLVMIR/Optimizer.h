#include <llvm/IR/LegacyPassManager.h>

class Optimizer
{
   public:
    Optimizer(llvm::Module* module);

    llvm::legacy::FunctionPassManager functionPassManager;
};
