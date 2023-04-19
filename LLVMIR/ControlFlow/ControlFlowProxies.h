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
    ValueAndType codeGen(IfNode* ast, CodeGenContext* ctx,
                         const Generator& generator);
};

template <>
class CodeGenProxy<WhileNode>
{
   public:
    ValueAndType codeGen(WhileNode* ast, CodeGenContext* ctx,
                         const Generator& generator);
};

template <>
class CodeGenProxy<ContinueNode>
{
   public:
    ValueAndType codeGen(ContinueNode* ast, CodeGenContext* ctx,
                         const Generator& generator);
};

template <>
class CodeGenProxy<BreakNode>
{
   public:
    ValueAndType codeGen(BreakNode* ast, CodeGenContext* ctx,
                         const Generator& generator);
};

template <>
class CodeGenProxy<ReturnNode>
{
   public:
    ValueAndType codeGen(ReturnNode* ast, CodeGenContext* ctx,
                         const Generator& generator);
};

}  // namespace LLVMCodeGen
}  // namespace XSharp
