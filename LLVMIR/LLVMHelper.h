#pragma once
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/TypeFinder.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include "XSharp/ASTNodes.h"
#include "XSharp/ControlFlow/ControlFlowAST.h"
#include "XSharp/XSharpUtils.h"
#include "XSharp/XString.h"
#include "XSharp/SymbolTable.h"
#include "XSharp/Types/TypeSystem.h"
#include "LLVMIR/LLVMTypes.h"
#include "LLVMIR/Optimizer.h"

typedef std::tuple<llvm::Value*, TypeNode*> ValueAndType;

class LLVMHelper
{
   public:
    LLVMHelper();
    // generate LLVM IR for the ast
    // error saved in LLVMHelper's errors
    std::vector<std::byte> generateLLVMIR(ASTNode* ast,
                                          const XString& filename);

    ValueAndType genGlobalVariable(VariableDeclarationNode* var);
    ValueAndType genLocalVariable(VariableDeclarationNode* var);
    ValueAndType genFunction(FunctionDeclarationNode* func);
    ValueAndType genCall(FunctionCallNode* call);
    ValueAndType genBinaryOp(BinaryOperatorNode* op);
    ValueAndType genUnaryOp(UnaryOperatorNode* op);
    ValueAndType genIf(XSharp::IfNode* ifNode);
    ValueAndType genWhile(XSharp::WhileNode* whileNode);

    // universal code generation for XSharp's AST
    // return [LLVM-IR's Value, XSharp's Type]
    ValueAndType codegen(ASTNode* node);
    ValueAndType deReferenceIf(ASTNode* ast);

    XSharp::SymbolTable symbolTable() const;

    llvm::LLVMContext context;
    llvm::Module module;
    llvm::IRBuilder<> builder;

   private:
    std::vector<XSharpError> errors;

    Optimizer optimizer;

    XSharp::SymbolTable globalSymbols;
    XSharp::SymbolTable* currentSymbols;
    XSharp::Symbol* current;
};
