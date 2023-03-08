#pragma once
#include "XSharp/xsharp_global.h"
#include "XSharp/Types/TypeConverter.h"

#ifdef XSharp_LLVMIR_SUPPORT
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#endif

namespace XSharp {

// Serve as Single Instance
class XSharp_EXPORT TypeAdapter
{
   public:
    static bool canConvert(TypeNode* from, TypeNode* to);

    static void addConverter(TypeConverter* converter);

#ifdef XSharp_LLVMIR_SUPPORT
    static llvm::Value* llvmConvert(TypeNode* from, TypeNode* to,
                                    llvm::Value* val);

    static void setLLVMContext(llvm::LLVMContext* context);
    static llvm::LLVMContext* getLLVMContext();

    static void setLLVMBuilder(llvm::IRBuilder<>* builder);
    static llvm::IRBuilder<>* getLLVMBuilder();
#endif

   private:
};

}  // namespace XSharp
