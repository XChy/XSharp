#include "CodeGenProxy.h"
#include <llvm-14/llvm/ADT/APFloat.h>
#include <llvm-14/llvm/ADT/APInt.h>
#include "XSharp/ASTNodes.h"
#include "XSharp/Types/TypeSystem.h"

ValueAndType CodeGenProxy<DefinitionsNode>::codeGen(
    DefinitionsNode* ast, CodeGenContextHelper* helper,
    const Generator& generator)
{
    if (ast->is<DefinitionsNode>()) {
        DefinitionsNode* definitions = ast->to<DefinitionsNode>();

        for (auto var : definitions->variableDeclarations()) {
            auto [val, type] = generator(var);
            if (!type) return {nullptr, nullptr};
        }
        for (auto funcNode : definitions->functionDeclarations()) {
            auto [val, type] = generator(funcNode);
            if (!type) return {nullptr, nullptr};
        }
        for (auto classDef : definitions->classDeclarations()) {
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

ValueAndType CodeGenProxy<BooleanNode>::codeGen(BooleanNode* ast,
                                                CodeGenContextHelper* helper,
                                                const Generator& generator)
{
    using llvm::APInt;
    using llvm::ConstantInt;

    auto val = ConstantInt::get(helper->context, APInt(1, ast->value()));
    return {val, XSharp::getBooleanType()};
}

ValueAndType CodeGenProxy<BoxNode>::codeGen(BoxNode* ast,
                                            CodeGenContextHelper* helper,
                                            const Generator& generator)
{
    return generator(ast->child());
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

ValueAndType CodeGenProxy<VariableNode>::codeGen(VariableNode* ast,
                                                 CodeGenContextHelper* helper,
                                                 const Generator& generator)
{
    if (helper->currentSymbols->hasSymbol(ast->name())) {
        auto symbol = helper->currentSymbols->findVariable(ast->name());
        return {symbol.definition, symbol.type};
    } else {
        helper->error("No such variable named {}", ast->name());
        return {nullptr, nullptr};
    }
}
