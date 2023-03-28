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

        // auto [mem_val, mem_type] = generator(member);

        // if (!mem_type) {
        // delete classInfo;
        // helper->toParentScope();
        // return {nullptr, nullptr};
        //}

        Field field;
        field.name = member->name();
        field.type = member->type()->toType();
        classInfo->dataFields.push_back(field);
    }

    for (auto method : ast->methods) {
        // TODO: process class' function
    }

    helper->exitScope();

    // FIXME: memory leak when no variables with the same type are declared
    registerClass(classInfo);
    return {nullptr, getVoidType()};
}
