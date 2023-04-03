#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/TypeFinder.h>
#include <llvm/Support/Casting.h>
#include <llvm/IR/Function.h>
#include <cstdint>
#include "LLVMIR/BuiltIn.h"
#include "XSharp/SymbolTable.h"
#include "XSharp/Types/TypeSystem.h"

namespace XSharp::LLVMCodeGen {

void setUpBuildIn(llvm::Module& module, llvm::LLVMContext& context,
                  XSharp::SymbolTable& symbols)
{
    module.getOrInsertFunction(
        "printI64", llvm::FunctionType::get(llvm::Type::getInt8Ty(context),
                                            {llvm::Type::getInt64Ty(context)},
                                            false));  // IO
    module.getOrInsertFunction(
        "printI32", llvm::FunctionType::get(llvm::Type::getInt8Ty(context),
                                            {llvm::Type::getInt32Ty(context)},
                                            false));  // IO
    module.getOrInsertFunction(
        "printDouble",
        llvm::FunctionType::get(llvm::Type::getInt8Ty(context),
                                {llvm::Type::getDoubleTy(context)},
                                false));  // IO
    module.getOrInsertFunction(
        "printBoolean",
        llvm::FunctionType::get(llvm::Type::getInt8Ty(context),
                                {llvm::Type::getInt1Ty(context)},
                                false));  // IO
    module.getOrInsertFunction(
        "inputI32", llvm::FunctionType::get(llvm::Type::getInt32Ty(context), {},
                                            false));  // IO

    symbols.addSymbol(
        XSharp::Symbol{.name = "print",
                       .symbolType = XSharp::SymbolType::Function,
                       .type = XSharp::getFunctionType(XSharp::getBooleanType(),
                                                       {XSharp::getI32Type()}),
                       .definition = module.getFunction("printI32")});
    symbols.addSymbol(
        XSharp::Symbol{.name = "print",
                       .symbolType = XSharp::SymbolType::Function,
                       .type = XSharp::getFunctionType(XSharp::getBooleanType(),
                                                       {XSharp::getI64Type()}),
                       .definition = module.getFunction("printI64")});

    symbols.addSymbol(
        XSharp::Symbol{.name = "print",
                       .symbolType = XSharp::SymbolType::Function,
                       .type = XSharp::getFunctionType(
                           XSharp::getBooleanType(), {XSharp::getDoubleType()}),
                       .definition = module.getFunction("printDouble")});

    symbols.addSymbol(XSharp::Symbol{
        .name = "print",
        .symbolType = XSharp::SymbolType::Function,
        .type = XSharp::getFunctionType(XSharp::getBooleanType(),
                                        {XSharp::getBooleanType()}),
        .definition = module.getFunction("printBoolean")});

    symbols.addSymbol(XSharp::Symbol{
        .name = "inputI32",
        .symbolType = XSharp::SymbolType::Function,
        .type = XSharp::getFunctionType(XSharp::getI32Type(), {}),
        .definition = module.getFunction("inputI32")});
}
}  // namespace XSharp::LLVMCodeGen
