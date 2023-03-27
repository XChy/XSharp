#include "MemberExprProxy.h"
#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"

ValueAndType CodeGenProxy<MemberExprNode>::codeGen(MemberExprNode *ast,
                                                   CodeGenContextHelper *helper,
                                                   const Generator &generator)
{
    auto [obj, obj_type] = generator(ast->object());

    if (!obj_type) return {nullptr, nullptr};
}
