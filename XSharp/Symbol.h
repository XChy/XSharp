#pragma once
#include "XSharp/Type.h"
#include "XSharp/XString.h"
#include "XSharp/TypeSystem.h"

#ifdef XSharp_LLVM_SUPPORT
#include "llvm/IR/Value.h"
#endif

namespace XSharp {
struct Symbol {
    XString name;
    TypeNode* type;
#ifdef XSharp_LLVM_SUPPORT
    llvm::Value* definition;
#endif
};

}  // namespace XSharp
