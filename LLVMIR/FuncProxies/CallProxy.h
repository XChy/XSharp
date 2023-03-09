#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"
template <>
class CodeGenProxy<FunctionCallNode> : public ASTVisitor<FunctionCallNode>
{
   public:
    ValueAndType codeGen(FunctionCallNode* ast, CodeGenContextHelper* helper,
                         const Generator& generator);
};
