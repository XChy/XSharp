#pragma once
#include "XSharp/Type.h"
#include "XSharp/XString.h"
#include "XSharp/TypeSystem.h"

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
    // the symbol also needs to save LLVM-related information
#ifdef XSharp_LLVMIR_SUPPORT
    llvm::Value* definition;
#endif

#ifdef XSharp_XSharpIR_SUPPORT
#endif
};

}  // namespace XSharp
