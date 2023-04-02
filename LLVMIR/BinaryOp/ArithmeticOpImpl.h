#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Types/TypeAdapter.h"

namespace XSharp {
namespace LLVMCodeGen {

typedef ValueAndType BinaryOpImpl(BinaryOperatorNode*, CodeGenContextHelper*,
                                  const Generator& generator);

ValueAndType AssignImpl(BinaryOperatorNode* op, CodeGenContextHelper* helper,
                        const Generator& generator);

ValueAndType AddImpl(BinaryOperatorNode* op, CodeGenContextHelper* helper,
                     const Generator& generator);

ValueAndType SubImpl(BinaryOperatorNode* op, CodeGenContextHelper* helper,
                     const Generator& generator);

ValueAndType MulImpl(BinaryOperatorNode* op, CodeGenContextHelper* helper,
                     const Generator& generator);

ValueAndType DivImpl(BinaryOperatorNode* op, CodeGenContextHelper* helper,
                     const Generator& generator);
ValueAndType ModImpl(BinaryOperatorNode* op, CodeGenContextHelper* helper,
                     const Generator& generator);

}  // namespace LLVMCodeGen
}  // namespace XSharp
