#pragma once

#include <unordered_map>
#include <vector>
#include "xsharp_global.h"
#include "XString.h"
#include "Type.h"

namespace XSharp {

class TypeContext
{
   public:
    TypeContext();

    uint registerType(const XSharp::TypeNode& type);

    // return typeid
    // If no type with the name exists ,return 0
    uint typeIDOf(XString name);

    TypeNode* typeOf(int typeId);

    std::unordered_map<XString, uint> typesMap;
    std::vector<TypeNode*> typesList;

    ~TypeContext();

   private:
    uint registerNum = 0;
};

}  // namespace XSharp
