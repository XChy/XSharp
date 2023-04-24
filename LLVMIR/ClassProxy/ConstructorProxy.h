#pragma once
#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/Class/ClassAST.h"

namespace XSharp::LLVMCodeGen {

template <>
class CodeGenProxy<ConstructorNode>
{
   public:
    ValueAndType codeGen(ConstructorNode* ast, CodeGenContext* ctx,
                         const Generator& generator);
};

}  // namespace XSharp::LLVMCodeGen
