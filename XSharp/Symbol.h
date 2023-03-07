#pragma once
#include "XSharp/Types/Type.h"
#include "XSharp/XString.h"

#ifdef XSharp_LLVMIR_SUPPORT
#include "llvm/IR/Value.h"
#endif

namespace XSharp {

enum class SymbolType {
    GlobalVariable,
    LocalVariable,
    Argument,
    Function,
    BuiltInFunction,
    Class,
};

struct Symbol {
    // Symbol's name
    XString name;

    // Symbol's type
    SymbolType symbolType;

    // X# Type
    TypeNode* type;

    // if we apply LLVM as the backend,
    // the symbol also save LLVM-related information
#ifdef XSharp_LLVMIR_SUPPORT
    union {
        llvm::Value* definition;
        llvm::Function* function;
    };
#endif

#ifdef XSharp_XSharpIR_SUPPORT
#endif
};

}  // namespace XSharp
