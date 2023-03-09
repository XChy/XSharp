#include "VarDefinitionProxies.h"
#include <cerrno>
#include "XSharp/ASTNodes.h"
#include "XSharp/Symbol.h"

ValueAndType CodeGenProxy<VariableDeclarationNode>::codeGen(
    VariableDeclarationNode* ast, CodeGenContextHelper* helper,
    const Generator& generator)
{
    // Local Variable
    auto& builder = helper->builder;
    auto& context = helper->context;

    if (helper->currentSymbols->findVariable(ast->name()).symbolType !=
        XSharp::SymbolType::NoneSymbol) {
        // TODO: can override variable in local scope
        helper->error("Redefinition of variable '{}'", ast->name());
    }

    TypeNode* varType = XSharp::getReferenceType(ast->type());

    // TODO: variable's initValue's processing
    auto alloca =
        builder.CreateAlloca(castToLLVM(varType->innerType(), context), nullptr,
                             ast->name().toStdString());

    helper->currentSymbols->addSymbol(
        {.name = ast->name(),
         .symbolType = XSharp::SymbolType::LocalVariable,
         .type = varType,
         .definition = alloca});

    return {alloca, varType};
    // Global Variable
    // if (globalSymbols.hasSymbol(varNode->name())) {
    // error("Redefinition of variable {}", varNode->name());
    // return {nullptr, nullptr};
    //}

    // TypeNode* type = varNode->type();

    //// TODO: variable's initValue's processing
    // llvm::GlobalVariable* globalVar = new llvm::GlobalVariable(
    // module, castToLLVM(type, context), type->isConst,
    // llvm::GlobalVariable::ExternalLinkage, nullptr,
    // varNode->name().toStdString());

    // globalSymbols.addSymbol({.name = varNode->name(),
    //.symbolType = XSharp::SymbolType::GlobalVariable,
    //.type = XSharp::getReferenceType(type),
    //.definition = globalVar});
    // return {globalVar, type};
}
