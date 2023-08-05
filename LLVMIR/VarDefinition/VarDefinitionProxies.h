#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"

// TODO: Scope-related Variable
namespace XSharp {
namespace LLVMCodeGen {
template <>
class CodeGenProxy<VarDeclNode>
{
   public:
    ValueAndType codeGen(VarDeclNode* ast, CodeGenContext* helper,
                         const Generator& generator);
};

ValueAndType genLocalVariable(VarDeclNode* ast, CodeGenContext* helper,
                              const Generator& generator);

ValueAndType genGlobalVariable(VarDeclNode* ast, CodeGenContext* helper,
                               const Generator& generator);

ValueAndType genDataMember(VarDeclNode* ast, CodeGenContext* helper,
                           const Generator& generator);

}  // namespace LLVMCodeGen
}  // namespace XSharp
