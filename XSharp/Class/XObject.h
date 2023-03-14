#include <cstdint>
#include <vector>
#include "XSharp/Class/XClass.h"
#include "XSharp/XString.h"
#include "XSharp/xsharp_global.h"

namespace XSharp {

struct BuiltinString;

struct XSharp_EXPORT ClassInfo {
    BuiltinString* name;
    // Traits(vtable)
    // Fields
};

/*
 *Common header for every Object
 *
 *
 */

struct XSharp_EXPORT XObjectHeader {
    // the last 2 bits is utilized as mark for GC
    // classInfo is applied to implement runtime reflection/trait
    uintptr_t classPointer;

    void setMark(char mark) noexcept;
    char getMark() const noexcept;
    void setClass(ClassInfo* classptr) noexcept;
    ClassInfo* getClass() const noexcept;
};

}  // namespace XSharp
