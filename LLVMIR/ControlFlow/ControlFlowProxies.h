#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/ControlFlow/ControlFlowAST.h"
#include "XSharp/Types/TypeAdapter.h"

// TODO: complete implementation for continue/return
namespace XSharp {
namespace LLVMCodeGen {

template <> /* LLVMCodeGen */
class CodeGenProxy<IfNode>
{
   public:
    ValueAndType codeGen(IfNode* ast, CodeGenContextHelper* helper,
                         const Generator& generator);
};

using namespace XSharp;
template <>
class CodeGenProxy<WhileNode>
{
   public:
    ValueAndType codeGen(WhileNode* ast, CodeGenContextHelper* helper,
                         const Generator& generator);
};

using namespace XSharp;
template <>
class CodeGenProxy<ReturnNode>
{
   public:
    ValueAndType codeGen(ReturnNode* ast, CodeGenContextHelper* helper,
                         const Generator& generator);
};

}  // namespace LLVMCodeGen
}  // namespace XSharp
