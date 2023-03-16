#include "xsharp_gc.h"
#include "gc.h"

void* GC_new_object(size_t size) { return GC_MALLOC(size); }

void* GC_new_class(size_t size)
{
    void* classPtr = GC_MALLOC(size);
    GC_add_roots(classPtr, nullptr);
    return classPtr;
}
