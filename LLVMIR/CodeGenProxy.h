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

class CodeGenProxy
{
   public:
    CodeGenProxy() = default;
    virtual ~CodeGenProxy();
};
