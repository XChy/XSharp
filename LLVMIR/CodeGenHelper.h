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
#include "XSharp/Types/Type.h"
#include "XSharp/XSharpUtils.h"
#include "XSharp/XString.h"
#include "XSharp/SymbolTable.h"
#include "XSharp/Types/TypeSystem.h"
#include "LLVMIR/LLVMTypes.h"
#include "LLVMIR/Optimizer.h"

typedef std::tuple<llvm::Value*, TypeNode*> ValueAndType;

class CodeGenContextHelper
{
   public:
    CodeGenContextHelper();

    XSharp::SymbolTable* toNewScope();
    XSharp::SymbolTable* toParentScope();

    void toNewFunctionScope(const XSharp::Symbol& funcSymbol);

    bool isGlobalScope() const;

    template <typename... T>
    auto inline error(fmt::format_string<T...> info, T&&... formatargs)
    {
        _errors.push_back(
            XSharpError(XSharpErrorType::SemanticsError,
                        vformat(info, fmt::make_format_args(formatargs...))));
    }

    llvm::LLVMContext context;
    llvm::Module module;
    llvm::IRBuilder<> builder;

    std::vector<XSharpError> _errors;
    Optimizer optimizer;

    XSharp::SymbolTable globalSymbols;
    XSharp::SymbolTable* currentSymbols;

    XSharp::Type* currentReturnType;

    // TODO: Apply proxy to change the BasicBlock below
    llvm::BasicBlock* loopStart;
    llvm::BasicBlock* loopEnd;
};

ValueAndType deReference(ValueAndType ref, CodeGenContextHelper* helper);
