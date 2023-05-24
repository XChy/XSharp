#pragma once
#include "XSharp/XString.h"
#include "XSharp/xsharp_global.h"
/*
 * the definition for Class in X#
 */

namespace XSharp {

enum class VisitLevel {
    Public,
    Private,
};

class Type;

struct Field {
    VisitLevel visitLevel;
    bool isStatic;
    XString name;
    Type* type;
};

class XSharp_EXPORT XClass
{
   public:
    XString name;

    std::vector<Field> dataFields;
    XClass* superClass;

    // TODO: Interfaces table
};

}  // namespace XSharp
