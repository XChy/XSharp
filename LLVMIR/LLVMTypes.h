#pragma once
#include <llvm/IR/Type.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/TypeFinder.h>
#include "XSharp/Types/Type.h"

llvm::Type* castToLLVM(XSharp::Type* type, llvm::LLVMContext& context);
