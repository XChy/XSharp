#pragma once
#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"

namespace XSharp {
namespace LLVMCodeGen {

template <>
class CodeGenProxy<FunctionNode>
{
   public:
    ValueAndType codeGen(FunctionNode* ast, CodeGenContext* helper,
                         const Generator& generator);
};

}  // namespace LLVMCodeGen
}  // namespace XSharp
