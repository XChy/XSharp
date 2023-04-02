#include "VarDefinitionProxies.h"
#include <cerrno>
#include "XSharp/ASTNodes.h"
#include "XSharp/Symbol.h"
#include "XSharp/Types/TypeAdapter.h"
#include "XSharp/Types/TypeSystem.h"
#include "XSharp/Types/TypeNodes.h"

namespace XSharp::LLVMCodeGen {

ValueAndType CodeGenProxy<VariableNode>::codeGen(VariableNode* ast,
                                                 CodeGenContextHelper* helper,
                                                 const Generator& generator)
{
    if (helper->isGlobalScope()) {
        return genGlobalVariable(ast, helper, generator);
    } else {
        return genLocalVariable(ast, helper, generator);
    }
}

ValueAndType genLocalVariable(VariableNode* ast, CodeGenContextHelper* helper,
                              const Generator& generator)
{
    using XSharp::TypeAdapter;
    // Local Variable
    auto& builder = helper->builder;
    auto& context = helper->context;

    if (helper->currentSymbols->hasSymbol(ast->name())) {
        helper->error("Redefinition of variable '{}'", ast->name());
    }

    Type* var_type;
    if (ast->type()->toType()->isBasic())
        var_type = XSharp::getReferenceType(ast->type()->toType());
    else
        var_type = XSharp::getReferenceType(
            XSharp::getReferenceType(ast->type()->toType()));

    auto var_alloca =
        builder.CreateAlloca(castToLLVM(var_type->derefType(), context),
                             nullptr, ast->name().toStdString());

    if (ast->initValue()) {
        auto [init_val, init_type] = generator(ast->initValue());
        // validate the initialization
        if (!init_type) return {nullptr, nullptr};

        init_val = TypeAdapter::llvmConvert(init_type, var_type->derefType(),
                                            init_val);
        // validate the type of init_val
        if (init_val)
            builder.CreateStore(init_val, var_alloca);
        else {
            helper->error("Cannot convert '{}' to '{}'", init_type->typeName(),
                          var_type->derefType()->typeName());
            return {nullptr, nullptr};
        }
    } else {
        // TODO: default initialization
    }

    helper->currentSymbols->addSymbol(
        {.name = ast->name(),
         .symbolType = XSharp::SymbolType::LocalVariable,
         .type = var_type,
         .definition = var_alloca});

    return {var_alloca, var_type};
}

ValueAndType genGlobalVariable(VariableNode* ast, CodeGenContextHelper* helper,
                               const Generator& generator)
{
    if (helper->globalSymbols.hasSymbol(ast->name())) {
        helper->error("Redefinition of variable {}", ast->name());
        return {nullptr, nullptr};
    }

    Type* var_type = XSharp::getReferenceType(ast->type()->toType());

    // TODO: Global variable's initValue's processing
    llvm::GlobalVariable* globalVar = new llvm::GlobalVariable(
        helper->module, castToLLVM(var_type->derefType(), helper->context),
        var_type->isConst, llvm::GlobalVariable::ExternalLinkage, nullptr,
        ast->name().toStdString());

    helper->globalSymbols.addSymbol(
        {.name = ast->name(),
         .symbolType = XSharp::SymbolType::GlobalVariable,
         .type = XSharp::getReferenceType(var_type),
         .definition = globalVar});

    return {globalVar, var_type};
}

ValueAndType genDataMember(VariableNode* ast, CodeGenContextHelper* helper,
                           const Generator& generator)
{
    if (helper->currentSymbols->hasSymbol(ast->name())) {
        helper->error("Redefinition of variable {}", ast->name());
        return {nullptr, nullptr};
    }
}

}  // namespace XSharp::LLVMCodeGen
