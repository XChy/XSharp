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
#include <functional>
#include "XSharp/ASTNodes.h"
#include "XSharp/ControlFlow/ControlFlowAST.h"
#include "XSharp/Class/ClassAST.h"
#include "XSharp/Types/Type.h"
#include "XSharp/XSharpUtils.h"
#include "XSharp/XString.h"
#include "XSharp/SymbolTable.h"
#include "XSharp/Types/TypeSystem.h"
#include "LLVMIR/LLVMTypes.h"
#include "LLVMIR/CodeGenHelper.h"

using namespace XSharp;
typedef std::function<ValueAndType(ASTNode*)> Generator;

// interface for codegen
class CodeGenBase
{
   public:
    CodeGenBase() = default;
    virtual ValueAndType codeGen(ASTNode* ast, CodeGenContextHelper* helper,
                                 const Generator& generator) = 0;
    virtual ~CodeGenBase() = default;
};

template <typename T>
class CodeGenProxy
{
   public:
    CodeGenProxy() = default;

    ValueAndType codeGen(T* ast, CodeGenContextHelper* helper,
                         const Generator& generator)
    {
        return {nullptr, nullptr};
    }
};
template <typename T>
class ASTVisitor : public CodeGenBase
{
   public:
    ASTVisitor() = default;
    ValueAndType codeGen(ASTNode* ast, CodeGenContextHelper* helper,
                         const Generator& generator)
    {
        return proxy.codeGen((T*)ast, helper, generator);
    }

   private:
    CodeGenProxy<T> proxy;
};
template <>
class CodeGenProxy<DefinitionsNode>
{
   public:
    ValueAndType codeGen(DefinitionsNode* ast, CodeGenContextHelper* helper,
                         const Generator& generator);
};

template <>
class CodeGenProxy<IntegerNode>
{
   public:
    ValueAndType codeGen(IntegerNode* ast, CodeGenContextHelper* helper,
                         const Generator& generator);
};

template <>
class CodeGenProxy<DecimalFractionNode>
{
   public:
    ValueAndType codeGen(DecimalFractionNode* ast, CodeGenContextHelper* helper,
                         const Generator& generator);
};

template <>
class CodeGenProxy<BooleanNode>
{
   public:
    ValueAndType codeGen(BooleanNode* ast, CodeGenContextHelper* helper,
                         const Generator& generator);
};
template <>
class CodeGenProxy<BlockNode>
{
   public:
    ValueAndType codeGen(BlockNode* ast, CodeGenContextHelper* helper,
                         const Generator& generator);
};

template <>
class CodeGenProxy<VariableExprNode>
{
   public:
    ValueAndType codeGen(VariableExprNode* ast, CodeGenContextHelper* helper,
                         const Generator& generator);
};
