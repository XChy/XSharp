#pragma once

#include "xsharp_global.h"
#include "XString.h"

namespace XSharp {

class TypeNode
{
   public:
    enum Categories {
        Variable,
        Class,
        Function,
        Array,
        Closure,
        Basic
    } category;

    uint typeID;

    bool isConst;
    int arrayDimension;
};

static uint registerType(const XSharp::TypeNode& type);

// return typeid
// If no type with the name exists ,return 0
static uint typeIDOf(XString name);

static TypeNode* typeOf(int typeId);

static uint registerNum = 0;
static std::vector<TypeNode> types{TypeNode()};

}  // namespace XSharp
