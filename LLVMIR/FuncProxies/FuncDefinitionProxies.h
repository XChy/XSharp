#pragma once
#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"

template <>
class CodeGenProxy<FunctionDeclarationNode>
{
   public:
    ValueAndType codeGen(FunctionDeclarationNode* ast,
                         CodeGenContextHelper* helper,
                         const Generator& generator);
};
