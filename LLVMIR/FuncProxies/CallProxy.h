#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"
namespace XSharp {
namespace LLVMCodeGen {

template <>
class CodeGenProxy<FunctionCallNode>
{
   public:
    ValueAndType codeGen(FunctionCallNode* ast, CodeGenContext* helper,
                         const Generator& generator);
};

}  // namespace LLVMCodeGen
}  // namespace XSharp
