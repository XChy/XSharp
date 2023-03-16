#include "XSharp/Symbol.h"
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

class Field
{
   public:
    XSharp::Symbol* fieldSymbol;
    VisitLevel visitLevel;
    bool isStatic;

    int offset();
};

class XSharp_EXPORT XClass
{
   public:
    XString name;

   private:
    // TODO: trait table
    std::vector<Field> fields;
};

}  // namespace XSharp
