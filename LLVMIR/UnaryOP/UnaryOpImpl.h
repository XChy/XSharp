#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Types/TypeAdapter.h"

namespace XSharp::LLVMCodeGen {

typedef ValueAndType UnaryOpImpl(UnaryOperatorNode*, CodeGenContext*,
                                 const Generator&);

ValueAndType PositiveImpl(UnaryOperatorNode*, CodeGenContext* helper,
                          const Generator& generator);
ValueAndType NegativeImpl(UnaryOperatorNode*, CodeGenContext* helper,
                          const Generator& generator);
ValueAndType NotImpl(UnaryOperatorNode*, CodeGenContext* helper,
                     const Generator& generator);
}  // namespace XSharp::LLVMCodeGen
