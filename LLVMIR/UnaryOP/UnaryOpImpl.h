#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Types/TypeAdapter.h"

namespace XSharp {
namespace LLVMCodeGen {

typedef ValueAndType UnaryOpImpl(UnaryOperatorNode*, CodeGenContextHelper*,
                                 const Generator&);

ValueAndType PositiveImpl(UnaryOperatorNode*, CodeGenContextHelper* helper,
                          const Generator& generator);
ValueAndType NegativeImpl(UnaryOperatorNode*, CodeGenContextHelper* helper,
                          const Generator& generator);
ValueAndType NotImpl(UnaryOperatorNode*, CodeGenContextHelper* helper,
                     const Generator& generator);

ValueAndType NewImpl(UnaryOperatorNode*, CodeGenContextHelper* helper,
                     const Generator& generator);

}  // namespace LLVMCodeGen
}  // namespace XSharp
