#pragma once

#include <llvm/IR/Value.h>
#include "LLVMIR/CodeGenHelper.h"
#include "XSharp/Types/Type.h"

namespace XSharp::LLVMCodeGen {

// Inject instructions to malloc a XSharp Object with GC
// @param type : type of object to malloc
llvm::Value* genObjectMalloc(CodeGenContext* helper, XSharp::Type* type);

// Inject instructions to malloc a XSharp Array with GC
// @param type : type of array to malloc
llvm::Value* genArrayMalloc(CodeGenContext* helper, XSharp::Type* type,
                            llvm::Value* element_count);

namespace ErrorFormatString {

constexpr auto illegal_type = "Not a legal type-expression : '{}'";
constexpr auto redefinition_var = "Redefinition of variable '{}'";
constexpr auto redefinition_func = "Redefinition of function '{}'";
constexpr auto redefinition_class = "Redefinition of class '{}'";
constexpr auto inconvertible = "Cannot convert '{}' to '{}'";

}  // namespace ErrorFormatString

#define assertWithError(cond, error_func, fmt_string, fmt_args...) \
    if (!(cond)) {                                                 \
        error_func(fmt_string, fmt_args);                          \
        return {nullptr, nullptr};                                 \
    }

#define passErrorIfNot(type)       \
    if (type == nullptr) {         \
        return {nullptr, nullptr}; \
    }

}  // namespace XSharp::LLVMCodeGen
