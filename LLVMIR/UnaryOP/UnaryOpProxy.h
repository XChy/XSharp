#include <unordered_map>
#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "LLVMIR/UnaryOP/UnaryOpImpl.h"
#include "XSharp/ASTNodes.h"

namespace XSharp {
namespace LLVMCodeGen {

template <>
class CodeGenProxy<UnaryOperatorNode>
{
   public:
    CodeGenProxy();
    ValueAndType codeGen(UnaryOperatorNode* ast, CodeGenContextHelper* helper,
                         const Generator& generator);

   private:
    std::unordered_map<XString, UnaryOpImpl*> processors;
};

}  // namespace LLVMCodeGen
}  // namespace XSharp
