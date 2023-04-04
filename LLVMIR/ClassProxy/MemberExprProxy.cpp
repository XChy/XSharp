#include "MemberExprProxy.h"
#include <llvm/ADT/APInt.h>
#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "LLVMIR/LLVMTypes.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Types/TypeSystem.h"
#include "XSharp/XString.h"

using namespace XSharp::LLVMCodeGen;
using namespace XSharp;

ValueAndType CodeGenProxy<MemberExprNode>::codeGen(MemberExprNode *ast,
                                                   CodeGenContextHelper *helper,
                                                   const Generator &generator)
{
    auto [obj, obj_type] = deReference(generator(ast->object()), helper);

    if (!obj_type) return {nullptr, nullptr};

    if (obj_type->isObject()) {
        XClass *classinfo = obj_type->getClassDecl();

        // start from virtual table
        int index = 1;
        for (auto fieid : classinfo->dataFields) {
            if (fieid.name == ast->memberName()) {
                return {helper->builder.CreateStructGEP(
                            castToLLVM(obj_type->derefType(), helper->context),
                            obj, index, ast->dump().toStdString()),
                        getReferenceType(fieid.type)};
            }
            index++;
        }
    }

    helper->error("No member '{}' for '{}'", ast->memberName(),
                  ast->object()->dump());
    return {nullptr, nullptr};
}
