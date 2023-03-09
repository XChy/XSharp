#pragma once

#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include "LLVMIR/CodeGenProxy.h"
#include "LLVMIR/LLVMTypes.h"
#include "LLVMIR/Optimizer.h"
#include "XSharp/XSharpUtils.h"
#include "fmt/core.h"

class LLVMHelper
{
   public:
    LLVMHelper();
    // generate LLVM IR for the ast
    // error saved in LLVMHelper's errors
    std::vector<std::byte> generateLLVMIR(ASTNode* ast,
                                          const XString& filename);
    // universal code generation for XSharp's AST
    // return [LLVM-IR's Value, XSharp's Type]
    ValueAndType codegen(ASTNode* node);

    CodeGenContextHelper contextHelper;

    template <typename ASTType>
    void addProxy()
    {
        // TODO: free these proxies
        proxies[std::type_index(typeid(ASTType))] = new CodeGenProxy<ASTType>;
    }

    std::unordered_map<std::type_index, CodeGenBase*> proxies;
    Generator generator;
};
