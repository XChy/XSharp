#pragma once
#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"
namespace XSharp {
namespace LLVMCodeGen {

template <>
class CodeGenProxy<IndexNode>
{
   public:
    ValueAndType codeGen(IndexNode* ast, CodeGenContextHelper* helper,
                         const Generator& generator);
};

}  // namespace LLVMCodeGen
}  // namespace XSharp
