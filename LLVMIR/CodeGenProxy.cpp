#include "CodeGenProxy.h"
#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/GlobalVariable.h>
#include "LLVMIR/LLVMTypes.h"
#include "LLVMIR/Utils.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Types/TypeSystem.h"
#include "XSharp/XString.h"

using namespace XSharp;
using namespace XSharp::LLVMCodeGen;

ValueAndType CodeGenProxy<DefinitionsNode>::codeGen(DefinitionsNode* ast,
                                                    CodeGenContext* helper,
                                                    const Generator& generator)
{
    if (ast->is<DefinitionsNode>()) {
        DefinitionsNode* definitions = ast->to<DefinitionsNode>();

        for (auto decl : definitions->decls) {
            auto [val, type] = generator(decl);
            if (!type) return {nullptr, nullptr};
        }
    }

    return {nullptr, XSharp::getVoidType()};
}

ValueAndType CodeGenProxy<IntegerNode>::codeGen(IntegerNode* ast,
                                                CodeGenContext* helper,
                                                const Generator& generator)
{
    using llvm::APInt;
    using llvm::ConstantInt;
    auto val = ConstantInt::get(helper->llvm_ctx, APInt(64, ast->value()));

    return {val, XSharp::getI64Type()};
}

ValueAndType CodeGenProxy<FPNode>::codeGen(FPNode* ast, CodeGenContext* helper,
                                           const Generator& generator)
{
    using llvm::APFloat;
    using llvm::ConstantFP;
    auto val = ConstantFP::get(helper->llvm_ctx, APFloat(ast->value()));

    return {val, XSharp::getDoubleType()};
}

ValueAndType CodeGenProxy<CharNode>::codeGen(CharNode* ast,
                                             CodeGenContext* helper,
                                             const Generator& generator)
{
    using llvm::APInt;
    using llvm::ConstantInt;

    auto val =
        ConstantInt::get(helper->llvm_ctx,
                         APInt(Types::get("char")->bits(), ast->value.value()));
    return {val, Types::get("char")};
}

ValueAndType CodeGenProxy<StringNode>::codeGen(StringNode* ast,
                                               CodeGenContext* ctx,
                                               const Generator& generator)
{
    auto x_array_type = XSharp::getArrayType(Types::get("char"), 1);
    auto eltTy = castToLLVM(x_array_type->elementType(), ctx->llvm_ctx);

    std::vector<llvm::Constant*> chars;

    for (int i = 0; i < ast->value().size(); ++i) {
        chars.push_back(
            ctx->llvm_builder.getInt(llvm::APInt(16, ast->value()[i].value())));
    }

    llvm::ConstantInt* length_data = llvm::ConstantInt::get(
        ctx->llvm_ctx, llvm::APInt(64, ast->value().size()));

    llvm::Constant* chars_data = llvm::ConstantArray::get(
        llvm::ArrayType::get(eltTy, chars.size()), chars);
    llvm::GlobalVariable* glob_chars = new llvm::GlobalVariable(
        ctx->module, chars_data->getType(), true,
        llvm::GlobalValue::ExternalLinkage, chars_data);

    llvm::Constant* array_data = llvm::ConstantStruct::get(
        structForArray(ctx->llvm_ctx), {length_data, glob_chars});

    llvm::GlobalVariable* glob = new llvm::GlobalVariable(
        ctx->module, array_data->getType(), true,
        llvm::GlobalValue::ExternalLinkage, array_data);

    return {glob, XSharp::getArrayType(Types::get("char"), 1)};
}

ValueAndType CodeGenProxy<BooleanNode>::codeGen(BooleanNode* ast,
                                                CodeGenContext* helper,
                                                const Generator& generator)
{
    using llvm::APInt;
    using llvm::ConstantInt;

    auto val = ConstantInt::get(helper->llvm_ctx, APInt(1, ast->value()));
    return {val, XSharp::getBooleanType()};
}

ValueAndType CodeGenProxy<BlockNode>::codeGen(BlockNode* ast,
                                              CodeGenContext* helper,
                                              const Generator& generator)
{
    for (ASTNode* content : ast->contents()) {
        auto [val, type] = generator(content);
        if (!type) return {nullptr, nullptr};
    }
    return {nullptr, XSharp::getVoidType()};
}

ValueAndType CodeGenProxy<VarExprNode>::codeGen(VarExprNode* ast,
                                                CodeGenContext* helper,
                                                const Generator& generator)
{
    auto symbol = helper->currentSymbols->findVariable(ast->name());
    assertWithError(symbol.symbolType != SymbolType::NoneSymbol, helper->error,
                    "No such variable named {}", ast->name());
    return {symbol.definition, symbol.type};
}
