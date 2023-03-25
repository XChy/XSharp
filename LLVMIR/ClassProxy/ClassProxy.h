#pragma once
#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/Class/ClassAST.h"

template <>
class CodeGenProxy<ClassNode>
{
   public:
    ValueAndType codeGen(ClassNode* ast, CodeGenContextHelper* helper,
                         const Generator& generator);
};
