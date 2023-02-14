#include "TypeSystem.h"
#include <algorithm>

using namespace XSharp;

static uint XSharp::registerType(const XSharp::TypeNode& type)
{
    registerNum++;
    types.push_back(type);
    return registerNum;
}

TypeNode* XSharp::typeOf(int typeId) { return &types[typeId]; }

static uint XSharp::typeIDOf(XString name)
{
    auto iterator_found = std::find_if(
        types.begin(), types.end(),
        [&](auto element) -> bool { return element.typeName == name; });

    if (iterator_found != types.end()) {
        return iterator_found->typeID;
    }

    return 0;
}
