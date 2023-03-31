#include <gc.h>
#include <cstdint>

extern "C" void* GC_new_object(int64_t size);

extern "C" void* GC_new_class(size_t size);
