#pragma once
#include "XSharp/Types/Type.h"
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

class Field
{
   public:
    VisitLevel visitLevel;
    bool isStatic;
    XString name;
    Type* type;

    int offset();
};

class XSharp_EXPORT XClass
{
   public:
    XString name;

    // TODO: trait table
    std::vector<Field> dataFields;
};

}  // namespace XSharp
