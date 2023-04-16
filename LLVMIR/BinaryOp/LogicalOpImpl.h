#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Types/TypeAdapter.h"

namespace XSharp {

namespace LLVMCodeGen {

ValueAndType EqualImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                       const Generator& generator);

ValueAndType NotEqualImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                          const Generator& generator);

ValueAndType GreaterImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                         const Generator& generator);

ValueAndType LessImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                      const Generator& generator);

ValueAndType GreaterOrEqualImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                                const Generator& generator);
ValueAndType LessOrEqualImpl(BinaryOperatorNode* op, CodeGenContext* helper,
                             const Generator& generator);

ValueAndType LogicalAndImpl(BinaryOperatorNode* op, CodeGenContext* header,
                            const Generator& generator);

ValueAndType LogicalOrImpl(BinaryOperatorNode* op, CodeGenContext* header,
                           const Generator& generator);
}  // namespace LLVMCodeGen

}  // namespace XSharp
