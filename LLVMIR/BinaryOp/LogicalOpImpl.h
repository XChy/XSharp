#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Types/TypeAdapter.h"

namespace XSharp {

ValueAndType EqualImpl(BinaryOperatorNode* op, CodeGenContextHelper* helper,
                       const Generator& generator);

ValueAndType GreaterImpl(BinaryOperatorNode* op, CodeGenContextHelper* helper,
                         const Generator& generator);

ValueAndType LessImpl(BinaryOperatorNode* op, CodeGenContextHelper* helper,
                      const Generator& generator);

ValueAndType GreaterOrEqualImpl(BinaryOperatorNode* op,
                                CodeGenContextHelper* helper,
                                const Generator& generator);
ValueAndType LessOrEqualImpl(BinaryOperatorNode* op,
                             CodeGenContextHelper* helper,
                             const Generator& generator);

ValueAndType LogicalAndImpl(BinaryOperatorNode* op,
                            CodeGenContextHelper* header,
                            const Generator& generator);

ValueAndType LogicalOrImpl(BinaryOperatorNode* op, CodeGenContextHelper* header,
                           const Generator& generator);
}  // namespace XSharp
