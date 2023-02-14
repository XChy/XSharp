#pragma once
#include <llvm-14/llvm/IR/Type.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/TypeFinder.h>
#include "XSharp/TypeSystem.h"

static llvm::Type* llvmTypeFor(XSharp::TypeNode type);
