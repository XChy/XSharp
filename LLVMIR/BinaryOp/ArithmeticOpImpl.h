#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Types/TypeAdapter.h"

typedef ValueAndType BinaryOpImpl(BinaryOperatorNode*, CodeGenContextHelper*,
                                  const Generator& generator);

namespace XSharp {

ValueAndType AssignImpl(BinaryOperatorNode* op, CodeGenContextHelper* helper,
                        const Generator& generator);

ValueAndType AddImpl(BinaryOperatorNode* op, CodeGenContextHelper* helper,
                     const Generator& generator);

}  // namespace XSharp
