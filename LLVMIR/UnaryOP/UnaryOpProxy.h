#include <unordered_map>
#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "LLVMIR/UnaryOP/UnaryOpImpl.h"
#include "XSharp/ASTNodes.h"

namespace XSharp::LLVMCodeGen {

template <>
class CodeGenProxy<UnaryOperatorNode>
{
   public:
    CodeGenProxy();
    ValueAndType codeGen(UnaryOperatorNode* ast, CodeGenContext* helper,
                         const Generator& generator);

   private:
    std::unordered_map<XString, UnaryOpImpl*> processors;
};

}  // namespace XSharp::LLVMCodeGen
