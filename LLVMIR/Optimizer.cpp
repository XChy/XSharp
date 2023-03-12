#include "Optimizer.h"
#include <llvm-14/llvm/IR/PassManager.h>
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

Optimizer::Optimizer(llvm::Module* module) : functionPassManager(module)
{
    functionPassManager.add(llvm::createPromoteMemoryToRegisterPass());
    functionPassManager.add(llvm::createLoopSimplifyPass());
    functionPassManager.add(llvm::createInstSimplifyLegacyPass());
    functionPassManager.add(llvm::createIndVarSimplifyPass());
    functionPassManager.add(llvm::createFloat2IntPass());
    functionPassManager.add(llvm::createConstraintEliminationPass());
    functionPassManager.add(llvm::createCFGSimplificationPass());
    functionPassManager.add(llvm::createFloat2IntPass());
    functionPassManager.add(llvm::createInstructionCombiningPass());
    functionPassManager.add(llvm::createReassociatePass());
    functionPassManager.doInitialization();
}
