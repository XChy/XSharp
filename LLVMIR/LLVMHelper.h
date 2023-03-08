#pragma once

#include "LLVMIR/CodeGenProxy.h"
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

    XSharp::SymbolTable symbolTable() const;

    llvm::LLVMContext context;
    llvm::Module module;
    llvm::IRBuilder<> builder;

   private:
    template <typename... T>
    void error(const char* info, T... formatargs)
    {
        _errors.push_back({XSharpErrorType::SemanticsError,
                           fmt::format(info, formatargs...)});
    }

    std::vector<XSharpError> _errors;
    Optimizer optimizer;

    XSharp::SymbolTable globalSymbols;
    XSharp::SymbolTable* currentSymbols;

    XSharp::TypeNode* currentReturnType;
};
