#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/TypeFinder.h>
#include <llvm/Support/Casting.h>
#include <llvm/IR/Function.h>
#include <cstdint>
#include "LLVMIR/BuiltIn.h"
#include "LLVMIR/LLVMTypes.h"
#include "XSharp/SymbolTable.h"
#include "XSharp/Types/TypeSystem.h"

namespace XSharp::LLVMCodeGen {

void setUpBuildIn(llvm::Module& module, llvm::LLVMContext& context,
                  XSharp::SymbolTable& symbols)
{
    addBuildInFunction("printI32", "print",
                       getFunctionType(Types::get("i8"), {Types::get("i32")}),
                       module, context, symbols);

    addBuildInFunction("printI64", "print",
                       getFunctionType(Types::get("i8"), {Types::get("i64")}),
                       module, context, symbols);

    addBuildInFunction("printChar", "print",
                       getFunctionType(Types::get("i8"), {Types::get("char")}),
                       module, context, symbols);

    addBuildInFunction("printStr", "print",
                       getFunctionType(Types::get("i8"),
                                       {getArrayType(Types::get("char"), 1)}),
                       module, context, symbols);

    addBuildInFunction(
        "printBoolean", "print",
        getFunctionType(Types::get("i8"), {Types::get("boolean")}), module,
        context, symbols);

    addBuildInFunction(
        "printDouble", "print",
        getFunctionType(Types::get("i8"), {Types::get("double")}), module,
        context, symbols);

    addBuildInFunction("inputI32", "inputI32",
                       getFunctionType(Types::get("i32"), {}), module, context,
                       symbols);
}

void addBuildInFunction(const char* linked_name, const XString& name,
                        Type* func_type, llvm::Module& module,
                        llvm::LLVMContext& context, SymbolTable& symbols)
{
    module.getOrInsertFunction(linked_name, (llvm::FunctionType*)castToLLVM(
                                                func_type, context));  // IO

    symbols.addSymbol(
        XSharp::Symbol{.name = name,
                       .symbolType = XSharp::SymbolType::Function,
                       .type = func_type,
                       .definition = module.getFunction(linked_name)});
}

}  // namespace XSharp::LLVMCodeGen
