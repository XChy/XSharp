#include "XSharp/Types/TypeAdapter.h"
#include "LLVMIR/LLVMTypes.h"
#include "XSharp/Types/TypeConverter.h"
using namespace XSharp;

static llvm::IRBuilder<>* llvmBuilder;
static llvm::LLVMContext* llvmContext;

static NumberConverter numberConverter;
static ObjectConverter objectConverter;

static std::vector<TypeConverter*> converters = {&numberConverter,
                                                 &objectConverter};

bool TypeAdapter::canConvert(Type* from, Type* to)
{
    for (auto converter : converters) {
        if (from->category == Type::Reference &&
            to->category != Type::Reference) {
            if (converter->convertable(from->derefType(), to)) return true;
            continue;
        }

        if (from->category == Type::Reference &&
            to->category == Type::Reference) {
            if (converter->convertable(from->derefType(), to->derefType()))
                return true;
            continue;
        }

        if (converter->convertable(from, to)) return true;
    }
    return false;
}

void TypeAdapter::addConverter(TypeConverter* converter)
{
    converters.push_back(converter);
}

#ifdef XSharp_LLVMIR_SUPPORT
llvm::Value* TypeAdapter::llvmConvert(Type* originalType, Type* expectedType,
                                      llvm::Value* val)
{
    if (originalType->equals(expectedType)) return val;

    for (auto converter : converters) {
        // get automatically dereference, to convert the innerValue's type
        if ((originalType->category == Type::Reference &&
             expectedType->category != Type::Reference) ||
            (originalType->isObjectRef() && expectedType->isObject())) {
            if (converter->convertable(originalType->derefType(),
                                       expectedType)) {
                auto loadedType = LLVMCodeGen::castToLLVM(
                    originalType->derefType(), *llvmContext);
                auto loadedValue = llvmBuilder->CreateLoad(loadedType, val);
                return converter->convert(originalType->derefType(),
                                          expectedType, llvmBuilder,
                                          llvmContext, loadedValue);
            }
            continue;
        }

        // Convert between references, used to handle the case
        // where converting Objects, and note that at this case
        // converter's 'convert(..)' is deprecated
        // if (originalType->category == Type::Reference &&
        // expectedType->category == Type::Reference) {
        // if (converter->convertable(originalType->derefType(),
        // expectedType->derefType())) {
        // return llvmBuilder->CreatePointerCast(
        // val, castToLLVM(expectedType->derefType(), *llvmContext));
        //}
        // continue;
        //}

        // directly convert the value type
        if (converter->convertable(originalType, expectedType)) {
            return converter->convert(originalType, expectedType, llvmBuilder,
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
