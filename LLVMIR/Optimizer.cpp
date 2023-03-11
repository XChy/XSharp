#include "Optimizer.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

Optimizer::Optimizer(llvm::Module* module) : functionPassManager(module)
{
    // Promote allocas to registers
    functionPassManager.add(llvm::createPromoteMemoryToRegisterPass());
    functionPassManager.add(llvm::createLoopSimplifyPass());
    functionPassManager.add(llvm::createInstSimplifyLegacyPass());
    functionPassManager.add(llvm::createInstructionCombiningPass());
    functionPassManager.add(llvm::createIndVarSimplifyPass());
    functionPassManager.add(llvm::createFloat2IntPass());
    functionPassManager.add(llvm::createConstraintEliminationPass());
    functionPassManager.add(llvm::createFloat2IntPass());
    // Do simple "peephole" optimizations and bit-twiddling optzns.
    // functionPassManager.add(llvm::createInstructionCombiningPass());
    // Reassociate expressions.
    functionPassManager.add(llvm::createReassociatePass());
    functionPassManager.doInitialization();
}
