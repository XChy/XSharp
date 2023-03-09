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
#include "XSharp/Types/Type.h"
#include "XSharp/XSharpUtils.h"
#include "XSharp/XString.h"
#include "XSharp/SymbolTable.h"
#include "XSharp/Types/TypeSystem.h"
#include "LLVMIR/LLVMTypes.h"
#include "LLVMIR/CodeGenHelper.h"

typedef std::function<ValueAndType(ASTNode*)> Generator;

class CodeGenBase
{
   public:
    CodeGenBase() = default;
    virtual ValueAndType codeGen(ASTNode* ast, CodeGenContextHelper* helper,
                                 const Generator& generator) = 0;
    virtual ~CodeGenBase() = default;
};

template <typename T>
class CodeGenProxy : public CodeGenBase
{
   public:
    CodeGenProxy() = default;
    virtual ValueAndType codeGen(ASTNode* ast, CodeGenContextHelper* helper,
                                 const Generator& generator)
    {
        return codeGen((T*)ast, helper, generator);
    }

    ValueAndType codeGen(T* ast, CodeGenContextHelper* helper,
                         const Generator& generator)
    {
        return {nullptr, nullptr};
    }

    virtual ~CodeGenProxy() = default;
};

template <typename T>
class ASTVisitor : public CodeGenBase
{
   public:
    ASTVisitor() = default;
    virtual ValueAndType codeGen(ASTNode* ast, CodeGenContextHelper* helper,
                                 const Generator& generator)
    {
        return ((CodeGenProxy<T>*)this)->codeGen((T*)ast, helper, generator);
    }

    virtual ValueAndType codeGen(T* ast, CodeGenContextHelper* helper,
                                 const Generator& generator) = 0;

    virtual ~ASTVisitor() = default;
};

template <>
class CodeGenProxy<DefinitionsNode> : public ASTVisitor<DefinitionsNode>
{
   public:
    ValueAndType codeGen(DefinitionsNode* ast, CodeGenContextHelper* helper,
                         const Generator& generator);
};

template <>
class CodeGenProxy<IntegerNode> : public ASTVisitor<IntegerNode>
{
   public:
    ValueAndType codeGen(IntegerNode* ast, CodeGenContextHelper* helper,
                         const Generator& generator);
};

template <>
class CodeGenProxy<DecimalFractionNode> : public ASTVisitor<DecimalFractionNode>
{
   public:
    ValueAndType codeGen(DecimalFractionNode* ast, CodeGenContextHelper* helper,
                         const Generator& generator);
};

template <>
class CodeGenProxy<BooleanNode> : public ASTVisitor<BooleanNode>
{
   public:
    ValueAndType codeGen(BooleanNode* ast, CodeGenContextHelper* helper,
                         const Generator& generator);
};

template <>
class CodeGenProxy<BoxNode> : public ASTVisitor<BoxNode>
{
   public:
    ValueAndType codeGen(BoxNode* ast, CodeGenContextHelper* helper,
                         const Generator& generator);
};

template <>
class CodeGenProxy<VariableNode> : public ASTVisitor<VariableNode>
{
   public:
    ValueAndType codeGen(VariableNode* ast, CodeGenContextHelper* helper,
                         const Generator& generator);
};
