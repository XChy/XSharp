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
    virtual bool convertable(Type* from, Type* to) = 0;
    virtual bool implicitConvertable(Type* from, Type* to) = 0;

#ifdef XSharp_LLVMIR_SUPPORT
    virtual llvm::Value* convert(Type* from, Type* to,
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
    bool convertable(Type* from, Type* to);
    bool implicitConvertable(Type* from, Type* to);

#ifdef XSharp_LLVMIR_SUPPORT
    llvm::Value* convert(Type* from, Type* to, llvm::IRBuilder<>* builder,
                         llvm::LLVMContext* context, llvm::Value* val);
#endif

   private:
};

class XSharp_EXPORT ObjectConverter : public TypeConverter
{
   public:
    ObjectConverter() {}
    bool convertable(Type* from, Type* to);
    bool implicitConvertable(Type* from, Type* to);

#ifdef XSharp_LLVMIR_SUPPORT
    llvm::Value* convert(Type* from, Type* to, llvm::IRBuilder<>* builder,
                         llvm::LLVMContext* context, llvm::Value* val);
#endif

   private:
};

}  // namespace XSharp
