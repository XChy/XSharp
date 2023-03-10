#pragma once
#include "XSharp/Types/Type.h"
#include "XSharp/XString.h"
#include "XSharp/xsharp_global.h"

#ifdef XSharp_LLVMIR_SUPPORT
#include "LLVMIR/LLVMTypes.h"
#endif

namespace XSharp {

class TypeConverter
{
   public:
    virtual bool convertable(TypeNode* from, TypeNode* to) = 0;
    virtual bool implicitConvertable(TypeNode* from, TypeNode* to) = 0;

#ifdef XSharp_LLVMIR_SUPPORT
    virtual llvm::Value* convert(TypeNode* from, TypeNode* to,
                                 llvm::IRBuilder<>* builder,
                                 llvm::LLVMContext* context,
                                 llvm::Value* val) = 0;
#endif

    virtual ~TypeConverter(){};
};

class XSharp_EXPORT NumberConverter : public TypeConverter
{
   public:
    NumberConverter() {}
    bool convertable(TypeNode* from, TypeNode* to);
    bool implicitConvertable(TypeNode* from, TypeNode* to);

#ifdef XSharp_LLVMIR_SUPPORT
    llvm::Value* convert(TypeNode* from, TypeNode* to,
                         llvm::IRBuilder<>* builder, llvm::LLVMContext* context,
                         llvm::Value* val);
#endif

   private:
};

}  // namespace XSharp
