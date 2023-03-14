#include "XObject.h"
#include <cstdint>
using namespace XSharp;

void XObjectHeader::setMark(char mark) noexcept
{
    classPointer = (intptr_t)getClass() | mark;
}

char XObjectHeader::getMark() const noexcept
{
    constexpr intptr_t mask = -4;
    return classPointer & (~mask);
}

ClassInfo* XObjectHeader::getClass() const noexcept
{
    constexpr intptr_t mask = -4;
    return (ClassInfo*)(classPointer & mask);
}

void XObjectHeader::setClass(ClassInfo* class_ptr) noexcept
{
    char mark = getMark();
    classPointer = (intptr_t)class_ptr | mark;
}
