#pragma once
#include "XSharp/xsharp_global.h"

#ifdef XSharp_LLVMIR_SUPPORT
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#endif

namespace XSharp {

class TypeNode;
class TypeConverter;

class XSharp_EXPORT TypeAdapter
{
   public:
    static bool canConvert(TypeNode* from, TypeNode* to);

    static void addConverter(TypeConverter* converter);

#ifdef XSharp_LLVMIR_SUPPORT
    static llvm::Value* convert(TypeNode* from, TypeNode* to, llvm::Value* val);

    static llvm::IRBuilder<>* llvmBuilder;
    static llvm::LLVMContext* llvmContext;
#endif

   private:
};

}  // namespace XSharp
