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
#include <stack>
#include "XSharp/ASTNodes.h"
#include "XSharp/ControlFlow/ControlFlowAST.h"
#include "XSharp/Types/Type.h"
#include "XSharp/XSharpUtils.h"
#include "XSharp/XString.h"
#include "XSharp/SymbolTable.h"
#include "XSharp/Types/TypeSystem.h"
#include "LLVMIR/LLVMTypes.h"
#include "LLVMIR/Target.h"
#include "LLVMIR/Optimizer.h"

namespace XSharp::LLVMCodeGen {

typedef std::tuple<llvm::Value*, XSharp::Type*> ValueAndType;

struct Loop {
    llvm::BasicBlock* cond;
    llvm::BasicBlock* body;
    llvm::BasicBlock* end;
};

class CodeGenContext
{
   public:
    CodeGenContext();

    void optimize();

    XSharp::SymbolTable* enterScope();
    XSharp::SymbolTable* exitScope();

    void enterFunctionScope(const XSharp::Symbol& funcSymbol);
    void exitFunctionScope();

    Type* currentRetType() const;

    bool isGlobalScope() const;

    template <typename... T>
    auto inline error(fmt::format_string<T...> info, T&&... formatargs)
    {
        _errors.push_back(
            XSharpError(XSharpErrorType::SemanticsError,
                        vformat(info, fmt::make_format_args(formatargs...))));
    }

    llvm::LLVMContext llvm_ctx;
    llvm::Module module;
    llvm::IRBuilder<> llvm_builder;

    std::vector<XSharpError> _errors;
    Optimizer optimizer;

    XSharp::SymbolTable globalSymbols;
    XSharp::SymbolTable* currentSymbols;

    std::stack<Type*> retTypes;
    std::stack<Loop> loops;
};

ValueAndType deReference(ValueAndType ref, CodeGenContext* ctx);

}  // namespace XSharp::LLVMCodeGen
