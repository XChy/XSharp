#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Types/TypeAdapter.h"

namespace XSharp::LLVMCodeGen {

typedef ValueAndType UnaryOpImpl(UnaryOpNode*, CodeGenContext*,
                                 const Generator&);

ValueAndType PositiveImpl(UnaryOpNode*, CodeGenContext* helper,
                          const Generator& generator);
ValueAndType NegativeImpl(UnaryOpNode*, CodeGenContext* helper,
                          const Generator& generator);
ValueAndType NotImpl(UnaryOpNode*, CodeGenContext* helper,
                     const Generator& generator);
}  // namespace XSharp::LLVMCodeGen
