#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"

// TODO: Scope-related Variable
template <>
class CodeGenProxy<VariableDeclarationNode>
{
   public:
    ValueAndType codeGen(VariableDeclarationNode* ast,
                         CodeGenContextHelper* helper,
                         const Generator& generator);
};
