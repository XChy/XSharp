#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Types/TypeAdapter.h"

namespace XSharp {
namespace LLVMCodeGen {

typedef ValueAndType BinaryOpImpl(BinaryOperatorNode*, CodeGenContext*,
                                  const Generator& generator);

ValueAndType AssignImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                        const Generator& generator);

ValueAndType AddImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                     const Generator& generator);

ValueAndType SubImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                     const Generator& generator);

ValueAndType MulImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                     const Generator& generator);

ValueAndType DivImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                     const Generator& generator);
ValueAndType ModImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                     const Generator& generator);

}  // namespace LLVMCodeGen
}  // namespace XSharp
