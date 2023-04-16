#include <unordered_map>
#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "LLVMIR/BinaryOp/ArithmeticOpImpl.h"
#include "LLVMIR/BinaryOp/LogicalOpImpl.h"
#include "XSharp/ASTNodes.h"

namespace XSharp::LLVMCodeGen {

template <>
class CodeGenProxy<BinaryOperatorNode>
{
   public:
    CodeGenProxy();
    ValueAndType codeGen(BinaryOperatorNode* ast, CodeGenContext* helper,
                         const Generator& generator);

   private:
    std::unordered_map<XString, BinaryOpImpl*> processors;
};

}  // namespace XSharp::LLVMCodeGen
