#include <unordered_map>
#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "LLVMIR/BinaryOp/ArithmeticOpImpl.h"
#include "XSharp/ASTNodes.h"

template <>
class CodeGenProxy<BinaryOperatorNode> : public ASTVisitor<BinaryOperatorNode>
{
   public:
    CodeGenProxy();
    ValueAndType codeGen(BinaryOperatorNode* ast, CodeGenContextHelper* helper,
                         const Generator& generator);

   private:
    std::unordered_map<XString, BinaryOpImpl*> processors;
};
