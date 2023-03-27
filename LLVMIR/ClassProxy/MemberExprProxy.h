#pragma once
#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"

template <>
class CodeGenProxy<MemberExprNode>
{
   public:
    ValueAndType codeGen(MemberExprNode* ast, CodeGenContextHelper* helper,
                         const Generator& generator);
};
