#pragma once

#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include "LLVMIR/CodeGenProxy.h"

#include "LLVMIR/FuncProxies/FuncDefinitionProxies.h"
#include "LLVMIR/BinaryOp/BinaryOpProxy.h"
#include "LLVMIR/UnaryOP/UnaryOpProxy.h"
#include "LLVMIR/ControlFlow/ControlFlowProxies.h"
#include "LLVMIR/FuncProxies/CallProxy.h"
#include "LLVMIR/VarDefinition/VarDefinitionProxies.h"
#include "LLVMIR/ClassProxy/ClassProxy.h"
#include "LLVMIR/ClassProxy/MemberExprProxy.h"

#include "LLVMIR/LLVMTypes.h"
#include "LLVMIR/Optimizer.h"
#include "XSharp/XSharpUtils.h"
#include "fmt/core.h"

namespace XSharp::LLVMCodeGen {

class CodeGenerator
{
   public:
    CodeGenerator();
    ~CodeGenerator();

    // generate LLVM-IR for the ast
    // error saved in LLVMHelper's errors
    std::vector<std::byte> generateIR(ASTNode* ast, const XString& filename);
    // universal code generation for XSharp's AST
    // return [LLVM-IR's Value, XSharp's Type]
    ValueAndType codegen(ASTNode* node);

    CodeGenContextHelper contextHelper;

    template <typename ASTType>
    void addProxy()
    {
        // TODO: free these proxies
        proxies[std::type_index(typeid(ASTType))] = new ASTVisitor<ASTType>;
    }

    std::unordered_map<std::type_index, CodeGenBase*> proxies;
    Generator generator;
};

}  // namespace XSharp::LLVMCodeGen
