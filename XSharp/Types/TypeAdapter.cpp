#include "XSharp/Types/TypeAdapter.h"
#include "LLVMIR/LLVMTypes.h"
#include "XSharp/Types/TypeConverter.h"
using namespace XSharp;
static llvm::IRBuilder<>* llvmBuilder;
static llvm::LLVMContext* llvmContext;

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
llvm::Value* TypeAdapter::llvmConvert(TypeNode* fromType,
                                      TypeNode* expectedType, llvm::Value* val)
{
    if (fromType->equals(expectedType)) return val;

    for (auto converter : converters) {
        // get automatically dereference, to convert the innerValue's type
        if (fromType->category == TypeNode::Reference &&
            expectedType->category != TypeNode::Reference) {
            if (converter->convertable(fromType->innerType(), expectedType)) {
                auto loadedType =
                    castToLLVM(fromType->innerType(), *llvmContext);
                auto loadedValue = llvmBuilder->CreateLoad(loadedType, val);
                return converter->convert(fromType->innerType(), expectedType,
                                          llvmBuilder, llvmContext,
                                          loadedValue);
            }
            continue;
        }

        // Convert between reference, used to handle the case
        // where converting Objects, and note that at this case
        // converter's 'convert(..)' is deprecated
        if (fromType->category == TypeNode::Reference &&
            expectedType->category == TypeNode::Reference) {
            if (converter->convertable(fromType->innerType(),
                                       expectedType->innerType())) {
                return llvmBuilder->CreatePointerCast(
                    val, castToLLVM(expectedType->innerType(), *llvmContext));
            }
            continue;
        }

        // directly convert the value type
        if (converter->convertable(fromType, expectedType)) {
            return converter->convert(fromType, expectedType, llvmBuilder,
                                      llvmContext, val);
        }
    }
    return nullptr;
}

void TypeAdapter::setLLVMContext(llvm::LLVMContext* context)
{
    llvmContext = context;
}

llvm::LLVMContext* TypeAdapter::getLLVMContext() { return llvmContext; }

void TypeAdapter::setLLVMBuilder(llvm::IRBuilder<>* builder)
{
    llvmBuilder = builder;
}

llvm::IRBuilder<>* TypeAdapter::getLLVMBuilder() { return llvmBuilder; }
#endif
