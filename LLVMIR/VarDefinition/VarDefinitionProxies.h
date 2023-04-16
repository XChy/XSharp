#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"

// TODO: Scope-related Variable
namespace XSharp {
namespace LLVMCodeGen {
template <>
class CodeGenProxy<VariableNode>
{
   public:
    ValueAndType codeGen(VariableNode* ast, CodeGenContext* helper,
                         const Generator& generator);
};

ValueAndType genLocalVariable(VariableNode* ast, CodeGenContext* helper,
                              const Generator& generator);

ValueAndType genGlobalVariable(VariableNode* ast, CodeGenContext* helper,
                               const Generator& generator);

ValueAndType genDataMember(VariableNode* ast, CodeGenContext* helper,
                           const Generator& generator);

}  // namespace LLVMCodeGen
}  // namespace XSharp
