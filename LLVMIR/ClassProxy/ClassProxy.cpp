#include "ClassProxy.h"
#include "LLVMIR/CodeGenProxy.h"
#include "LLVMIR/Utils.h"
#include "XSharp/Class/ClassAST.h"
#include "XSharp/Types/Type.h"
#include "XSharp/Types/TypeNodes.h"
#include "XSharp/Types/TypeSystem.h"
#include "XSharp/XString.h"
#include "XSharp/Class/XClass.h"

using namespace XSharp;
using namespace XSharp::LLVMCodeGen;

ValueAndType CodeGenProxy<ClassNode>::codeGen(ClassNode* ast,
                                              CodeGenContextHelper* helper,
                                              const Generator& generator)
{
    XClass* classInfo = new XClass;
    helper->enterScope();

    classInfo->name = ast->name;
    registerClass(classInfo);

    for (auto member : ast->members) {
        Field field;
        field.name = member->name();
        field.type = asEntityType(member->type()->toType());

        assertWithError(field.type, helper->error,
                        ErrorFormatString::illegal_type,
                        member->type()->dump());

        classInfo->dataFields.push_back(field);
    }

    for (auto method : ast->methods) {
        auto [_, method_type] = generator(method);

        passErrorIfNot(method_type);
    }

    helper->exitScope();

    // FIXME: memory leak when no variables with the same type are declared
    return {nullptr, getVoidType()};
}
