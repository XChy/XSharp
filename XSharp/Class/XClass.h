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

class TypeNode;
class Field
{
   public:
    VisitLevel visitLevel;
    bool isStatic;
    XString name;
    TypeNode* type;

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
