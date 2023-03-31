#include "ClassProxy.h"
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/Class/ClassAST.h"
#include "XSharp/Types/Type.h"
#include "XSharp/Types/TypeNodes.h"
#include "XSharp/Types/TypeSystem.h"
#include "XSharp/XString.h"
#include "XSharp/Class/XClass.h"
using namespace XSharp;

ValueAndType CodeGenProxy<ClassNode>::codeGen(ClassNode* ast,
                                              CodeGenContextHelper* helper,
                                              const Generator& generator)
{
    XClass* classInfo = new XClass;
    helper->enterScope();

    classInfo->name = ast->name;

    for (auto member : ast->members) {
        // TODO: variable definition
        Field field;
        field.name = member->name();
        if ((field.type = member->type()->toType())) {
            classInfo->dataFields.push_back(field);
        } else {
            helper->error("No typename matched with {}",
                          member->type()->dump());
            return {nullptr, nullptr};
        }
    }

    for (auto method : ast->methods) {
        // TODO: process class' function
    }

    helper->exitScope();

    // FIXME: memory leak when no variables with the same type are declared
    registerClass(classInfo);
    return {nullptr, getVoidType()};
}
