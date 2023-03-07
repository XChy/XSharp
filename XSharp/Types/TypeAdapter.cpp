#include "XSharp/Types/TypeAdapter.h"
#include "XSharp/Types/TypeConverter.h"
using namespace XSharp;
static std::vector<TypeConverter*> converters;
bool TypeAdapter::canConvert(TypeNode* from, TypeNode* to)
{
    for (auto converter : converters) {
        if (converter->convertable(from, to)) return true;
    }
    return false;
}

void TypeAdapter::addConverter(TypeConverter* converter)
{
    converters.push_back(converter);
}

#ifdef XSharp_LLVMIR_SUPPORT
llvm::Value* TypeAdapter::convert(TypeNode* from, TypeNode* to,
                                  llvm::Value* val)
{
    for (auto converter : converters) {
        if (converter->convertable(from, to)) {
            return converter->convert(from, to, *llvmBuilder, *llvmContext,
                                      val);
        }
    }
    return nullptr;
}
#endif
