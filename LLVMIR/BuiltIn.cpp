#include <llvm-14/llvm/IR/DerivedTypes.h>
#include <llvm-14/llvm/IR/LLVMContext.h>
#include <llvm-14/llvm/IR/Type.h>
#include <llvm-14/llvm/IR/TypeFinder.h>
#include <llvm-14/llvm/Support/Casting.h>
#include <llvm/IR/Function.h>
#include <cstdint>
#include "LLVMIR/BuiltIn.h"
void setUpBuildIn(llvm::Module& module, llvm::LLVMContext& context)
{
    using namespace llvm;
    // IO
    module.getOrInsertFunction(
        "printI64", FunctionType::get(Type::getInt8Ty(context),
                                      {Type::getInt64Ty(context)}, false));
}
