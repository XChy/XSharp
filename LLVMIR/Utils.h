#pragma once

#include <llvm/IR/Value.h>
#include "LLVMIR/CodeGenHelper.h"
#include "XSharp/Types/Type.h"

namespace XSharp::LLVMCodeGen {

// Inject instructions to malloc a XSharp Object with GC
// @param type : type of object to malloc
llvm::Value* genObjectMalloc(CodeGenContextHelper* helper, XSharp::Type* type);

// Inject instructions to malloc a XSharp Array with GC
// @param type : type of array to malloc
llvm::Value* genArrayMalloc(CodeGenContextHelper* helper, XSharp::Type* type,
                            llvm::Value* element_count);

}  // namespace XSharp::LLVMCodeGen
