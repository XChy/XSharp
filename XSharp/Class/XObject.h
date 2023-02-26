#include <vector>
#include "XSharp/XString.h"
#include "XSharp/xsharp_global.h"

namespace XSharp {

class XMemberFunction;
class XMember;
struct XObjectHeader;

/*
 * the definition for Class in X#
 */
class XSharp_EXPORT XClass
{
   public:
   private:
    XString _name;
    std::vector<XMemberFunction*> _memberFunctions;
    std::vector<XMember*> _members;
};

/*
 * Header for every Object
 *
 *
 */

struct XSharp_EXPORT XObjectHeader {
    // member marked is applied to implement sweep-mark GC
    bool marked;

    // padding
    char padding[3];

    // member classDef is applied to implement runtime reflection
    XClass* classDef;
};

}  // namespace XSharp
