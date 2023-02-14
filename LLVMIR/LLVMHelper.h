#pragma once
#include <cstddef>
#include <llvm-14/llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/TypeFinder.h>
#include <llvm/ExecutionEngine/Interpreter.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/Support/TargetSelect.h>
#include "XSharp/ASTNodes.h"
#include "XSharp/XSharpUtils.h"
#include "XSharp/XString.h"
#include "LLVMIR/LLVMTypes.h"

class LLVMHelper
{
   public:
    // generate LLVM IR for the ast
    // error saved in LLVMHelper's error
    static std::vector<std::byte> generateLLVMIR(ASTNode* ast,
                                                 const XString& filename);
    static llvm::Function* genFunction(FunctionDeclarationNode* node);
    static llvm::Value* codegen(ASTNode* node);

    XSharpError error;
};
