#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"
namespace XSharp {
namespace LLVMCodeGen {

template <>
class CodeGenProxy<CallNode>
{
   public:
    ValueAndType codeGen(CallNode* ast, CodeGenContext* helper,
                         const Generator& generator);
};

}  // namespace LLVMCodeGen
}  // namespace XSharp
