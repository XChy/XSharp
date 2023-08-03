#include "Optimizer.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Vectorize/LoadStoreVectorizer.h"
#include "llvm/Transforms/Vectorize.h"
#include "llvm/Transforms/IPO.h"

#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

using namespace XSharp::LLVMCodeGen;

Optimizer::Optimizer(llvm::Module* module) : functionPassManager(module)
{
    // promote memory allocation to register allocation
    functionPassManager.add(llvm::createPromoteMemoryToRegisterPass());
    // simplify looping
    functionPassManager.add(llvm::createLoopSimplifyPass());
    functionPassManager.add(llvm::createStructurizeCFGPass());
    functionPassManager.add(llvm::createInstSimplifyLegacyPass());
    functionPassManager.add(llvm::createIndVarSimplifyPass());
    functionPassManager.add(llvm::createFloat2IntPass());
    functionPassManager.add(llvm::createConstraintEliminationPass());
    functionPassManager.add(llvm::createCFGSimplificationPass());
    functionPassManager.add(llvm::createGVNPass());
    functionPassManager.add(llvm::createNewGVNPass());
    functionPassManager.add(llvm::createMemCpyOptPass());
    functionPassManager.add(llvm::createLoopStrengthReducePass());
    functionPassManager.add(llvm::createFloat2IntPass());
    functionPassManager.add(llvm::createLICMPass());
    functionPassManager.add(llvm::createInstructionCombiningPass());
    functionPassManager.add(llvm::createReassociatePass());
    functionPassManager.add(llvm::createConstraintEliminationPass());
    functionPassManager.add(llvm::createLoopSinkPass());
    functionPassManager.add(llvm::createGVNSinkPass());
    functionPassManager.add(llvm::createGVNPass());
    functionPassManager.add(llvm::createDivRemPairsPass());
    functionPassManager.add(llvm::createBitTrackingDCEPass());
    functionPassManager.add(llvm::createNaryReassociatePass());
    functionPassManager.add(llvm::createSLPVectorizerPass());
    functionPassManager.add(llvm::createSROAPass());
    functionPassManager.add(llvm::createDeadStoreEliminationPass());
    functionPassManager.add(llvm::createLibCallsShrinkWrapPass());
    functionPassManager.add(llvm::createSeparateConstOffsetFromGEPPass());
    functionPassManager.add(llvm::createMergedLoadStoreMotionPass());
    functionPassManager.add(llvm::createTailCallEliminationPass());
    functionPassManager.add(llvm::createVectorCombinePass());
    functionPassManager.add(llvm::createLoadStoreVectorizerPass());
    functionPassManager.add(llvm::createInferAddressSpacesPass());
    functionPassManager.add(llvm::createSeparateConstOffsetFromGEPPass());
    functionPassManager.add(llvm::createScalarizeMaskedMemIntrinLegacyPass());
    functionPassManager.add(llvm::createEarlyCSEPass(true));
    functionPassManager.doInitialization();

    modulePassManager.add(llvm::createFunctionInliningPass());
    modulePassManager.add(llvm::createVectorCombinePass());
    modulePassManager.add(llvm::createIPSCCPPass());
}
