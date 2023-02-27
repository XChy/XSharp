#pragma once
#include "XSharp/Type.h"
#include "XSharp/XString.h"
#include "XSharp/TypeSystem.h"

#ifdef XSharp_LLVM_SUPPORT
#include "llvm/IR/Value.h"
#endif

namespace XSharp {
struct Symbol {
    // Symbol's name
    XString name;

    // X# Type
    TypeNode* type;

    // if we apply LLVM as the backend,
    // the symbol also needs to save LLVM-related information
#ifdef XSharp_LLVM_SUPPORT
    llvm::Value* definition;
#endif
};

}  // namespace XSharp
