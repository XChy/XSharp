#include <cstdint>
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
    ~XClass();

   private:
    XString _name;
    std::vector<XMemberFunction*> _memberFunctions;
    std::vector<XMember*> _fields;
};

/*
 * Header for every Object
 *
 *
 */

struct XSharp_EXPORT XObjectHeader {
    // the last 2 bits is utilized as mark for GC
    int headerBits;
    // member classDef is applied to implement runtime reflection
    XClass* classDef;
};

}  // namespace XSharp
