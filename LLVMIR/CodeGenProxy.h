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

namespace XSharp {
namespace LLVMCodeGen {

typedef std::function<ValueAndType(ASTNode*)> Generator;

// interface for codegen
class CodeGenBase
{
   public:
    CodeGenBase() = default;
    virtual ValueAndType codeGen(ASTNode* ast, CodeGenContext* helper,
                                 const Generator& generator) = 0;
    virtual ~CodeGenBase() = default;
};

template <typename T>
class CodeGenProxy
{
   public:
    CodeGenProxy() = default;

    ValueAndType codeGen(T* ast, CodeGenContext* helper,
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
    ValueAndType codeGen(ASTNode* ast, CodeGenContext* helper,
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
    ValueAndType codeGen(DefinitionsNode* ast, CodeGenContext* helper,
                         const Generator& generator);
};

template <>
class CodeGenProxy<IntegerNode>
{
   public:
    ValueAndType codeGen(IntegerNode* ast, CodeGenContext* helper,
                         const Generator& generator);
};

template <>
class CodeGenProxy<FPNode>
{
   public:
    ValueAndType codeGen(FPNode* ast, CodeGenContext* helper,
                         const Generator& generator);
};

template <>
class CodeGenProxy<CharNode>
{
   public:
    ValueAndType codeGen(CharNode* ast, CodeGenContext* helper,
                         const Generator& generator);
};

template <>
class CodeGenProxy<StringNode>
{
   public:
    ValueAndType codeGen(StringNode* ast, CodeGenContext* helper,
                         const Generator& generator);
};

template <>
class CodeGenProxy<BooleanNode>
{
   public:
    ValueAndType codeGen(BooleanNode* ast, CodeGenContext* helper,
                         const Generator& generator);
};
template <>
class CodeGenProxy<BlockNode>
{
   public:
    ValueAndType codeGen(BlockNode* ast, CodeGenContext* helper,
                         const Generator& generator);
};

template <>
class CodeGenProxy<VarExprNode>
{
   public:
    ValueAndType codeGen(VarExprNode* ast, CodeGenContext* helper,
                         const Generator& generator);
};

}  // namespace LLVMCodeGen
}  // namespace XSharp
