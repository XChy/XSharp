#include "TypeSystem.h"
#include <algorithm>
#include "XSharp/Type.h"

using namespace XSharp;

TypeContext::TypeContext()
{
    // fill a useless node so that typeID start with 1
    typesList.push_back(TypeNode{});
    // TODO register basic type when initializing
}

uint TypeContext::registerType(const XSharp::TypeNode& type)
{
    registerNum++;

    typesMap[type.typeName()] = registerNum;

    typesList.push_back(type);
    typesList[registerNum].typeID = registerNum;
    return registerNum;
}

TypeNode* TypeContext::typeOf(int typeId) { return &typesList[typeId]; }

uint TypeContext::typeIDOf(XString name)
{
    auto iterator_found = typesMap.find(name);

    if (iterator_found != typesMap.end()) {
        return iterator_found->second;
    }

    return 0;
}
