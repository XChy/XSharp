#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Types/TypeAdapter.h"

typedef ValueAndType UnaryOpImpl(UnaryOperatorNode*, CodeGenContextHelper*,
                                 const Generator&);
namespace XSharp {
ValueAndType PositiveImpl(UnaryOperatorNode*, CodeGenContextHelper* helper,
                          const Generator& generator);
ValueAndType NegativeImpl(UnaryOperatorNode*, CodeGenContextHelper* helper,
                          const Generator& generator);
ValueAndType NotImpl(UnaryOperatorNode*, CodeGenContextHelper* helper,
                     const Generator& generator);

}  // namespace XSharp
