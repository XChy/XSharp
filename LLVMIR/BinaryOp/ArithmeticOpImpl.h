#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/Types/TypeAdapter.h"

typedef ValueAndType BinaryOpImpl(BinaryOperatorNode*, CodeGenContextHelper*,
                                  const Generator& generator);

namespace XSharp {
ValueAndType AddImpl(BinaryOperatorNode* op, CodeGenContextHelper* helper,
                     const Generator& generator);
}  // namespace XSharp
