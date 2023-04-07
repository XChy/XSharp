#include "CodeGenProxy.h"
#include <llvm-14/llvm/ADT/APFloat.h>
#include <llvm-14/llvm/ADT/APInt.h>
#include <llvm-14/llvm/IR/Constant.h>
#include <llvm-14/llvm/IR/Constants.h>
#include <llvm-14/llvm/IR/DerivedTypes.h>
#include <llvm-14/llvm/IR/GlobalVariable.h>
#include "LLVMIR/LLVMTypes.h"
#include "LLVMIR/Utils.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Types/TypeSystem.h"
#include "XSharp/XString.h"

using namespace XSharp;
using namespace XSharp::LLVMCodeGen;

ValueAndType CodeGenProxy<DefinitionsNode>::codeGen(
    DefinitionsNode* ast, CodeGenContextHelper* helper,
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
                                                CodeGenContextHelper* helper,
                                                const Generator& generator)
{
    using llvm::APInt;
    using llvm::ConstantInt;
    auto val = ConstantInt::get(helper->context, APInt(64, ast->value()));

    return {val, XSharp::getI64Type()};
}

ValueAndType CodeGenProxy<DecimalFractionNode>::codeGen(
    DecimalFractionNode* ast, CodeGenContextHelper* helper,
    const Generator& generator)
{
    using llvm::APFloat;
    using llvm::ConstantFP;
    auto val = ConstantFP::get(helper->context, APFloat(ast->value()));

    return {val, XSharp::getDoubleType()};
}

ValueAndType CodeGenProxy<CharNode>::codeGen(CharNode* ast,
                                             CodeGenContextHelper* helper,
                                             const Generator& generator)
{
    using llvm::APInt;
    using llvm::ConstantInt;

    auto val = ConstantInt::get(
        helper->context, APInt(Types::get("char")->bits(), ast->value.value()));
    return {val, Types::get("char")};
}

ValueAndType CodeGenProxy<StringNode>::codeGen(StringNode* ast,
                                               CodeGenContextHelper* helper,
                                               const Generator& generator)
{
    auto llvm_array_type = (llvm::ArrayType*)castToLLVM(
        XSharp::getArrayType(Types::get("char"), 1), helper->context);
    std::vector<llvm::Constant*> chars;
    for (int i = 0; i <= ast->value().size(); ++i) {
        chars.push_back(
            helper->builder.getInt(llvm::APInt(16, ast->value()[i].value())));
    }

    llvm::Constant* data = llvm::ConstantArray::get(llvm_array_type, chars);
    llvm::GlobalVariable* glob =
        new llvm::GlobalVariable(helper->module, data->getType(), true,
                                 llvm::GlobalValue::ExternalLinkage, data);

    return {glob, XSharp::getArrayType(Types::get("char"), 1)};
}

ValueAndType CodeGenProxy<BooleanNode>::codeGen(BooleanNode* ast,
                                                CodeGenContextHelper* helper,
                                                const Generator& generator)
{
    using llvm::APInt;
    using llvm::ConstantInt;

    auto val = ConstantInt::get(helper->context, APInt(1, ast->value()));
    return {val, XSharp::getBooleanType()};
}

ValueAndType CodeGenProxy<BlockNode>::codeGen(BlockNode* ast,
                                              CodeGenContextHelper* helper,
                                              const Generator& generator)
{
    for (ASTNode* content : ast->contents()) {
        auto [val, type] = generator(content);
        if (!type) return {nullptr, nullptr};
    }
    return {nullptr, XSharp::getVoidType()};
}

ValueAndType CodeGenProxy<VariableExprNode>::codeGen(
    VariableExprNode* ast, CodeGenContextHelper* helper,
    const Generator& generator)
{
    auto symbol = helper->currentSymbols->findVariable(ast->name());
    assertWithError(symbol.symbolType != SymbolType::NoneSymbol, helper->error,
                    "No such variable named {}", ast->name());
    return {symbol.definition, symbol.type};
}
