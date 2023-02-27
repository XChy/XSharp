#pragma once
#include <cstddef>
#include <vector>
#include <llvm-14/llvm/IR/GlobalVariable.h>
#include <llvm/IR/Value.h>
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
#include "XSharp/SymbolTable.h"
#include "XSharp/TypeSystem.h"
#include "LLVMIR/LLVMTypes.h"

class LLVMHelper
{
   public:
    LLVMHelper();
    // generate LLVM IR for the ast
    // error saved in LLVMHelper's error
    std::vector<std::byte> generateLLVMIR(ASTNode* ast,
                                          const XString& filename);

    llvm::GlobalVariable* genGlobalVariable(VariableDeclarationNode* node);
    llvm::Function* genFunction(FunctionDeclarationNode* node);
    llvm::Value* genBinaryOp(BinaryOperatorNode* op);
    llvm::Value* genUnaryOp(UnaryOperatorNode* op);
    llvm::Value* codegen(ASTNode* node);

    XSharp::SymbolTable symbolTable() const;

   private:
    std::vector<XSharpError> errors;

    llvm::LLVMContext context;
    llvm::Module module;
    llvm::IRBuilder<> builder;

    XSharp::SymbolTable symbols;
    XSharp::SymbolTable* currentSymbols;
};
