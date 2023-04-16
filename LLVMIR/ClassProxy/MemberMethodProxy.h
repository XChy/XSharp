#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Class/ClassAST.h"

namespace XSharp {
namespace LLVMCodeGen {

template <>
class CodeGenProxy<MemberMethodNode>
{
   public:
    ValueAndType codeGen(MemberMethodNode* ast, CodeGenContext* helper,
                         const Generator& generator);
};

}  // namespace LLVMCodeGen
}  // namespace XSharp
