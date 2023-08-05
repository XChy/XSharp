#include "VarDefinitionProxies.h"
#include <cerrno>
#include "LLVMIR/Utils.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Symbol.h"
#include "XSharp/Types/TypeAdapter.h"
#include "XSharp/Types/TypeSystem.h"
#include "XSharp/Types/TypeNodes.h"

namespace XSharp::LLVMCodeGen {

ValueAndType CodeGenProxy<VarDeclNode>::codeGen(VarDeclNode* ast,
                                                CodeGenContext* helper,
                                                const Generator& generator)
{
    if (helper->isGlobalScope()) {
        return genGlobalVariable(ast, helper, generator);
    } else {
        return genLocalVariable(ast, helper, generator);
    }
}

ValueAndType genLocalVariable(VarDeclNode* ast, CodeGenContext* helper,
                              const Generator& generator)
{
    using XSharp::TypeAdapter;
    // Local Variable
    auto& builder = helper->llvm_builder;
    auto& context = helper->llvm_ctx;

    assertWithError(!helper->currentSymbols->hasSymbol(ast->name()),
                    helper->error, ErrorFormatString::redefinition_var,
                    ast->name());

    Type* var_type = asEntityType(ast->type()->toType());

    assertWithError(var_type, helper->error, ErrorFormatString::illegal_type,
                    ast->type()->dump());

    // lhs must be a reference
    var_type = getReferenceType(var_type);

    auto var_alloca =
        builder.CreateAlloca(castToLLVM(var_type->derefType(), context),
                             nullptr, ast->name().toStdString());

    if (ast->init()) {
        auto [init_val, init_type] = generator(ast->init());
        // validate the initialization
        passErrorIfNot(init_type);

        init_val = TypeAdapter::llvmConvert(init_type, var_type->derefType(),
                                            init_val);
        // validate the type of init_val
        assertWithError(var_type, helper->error,
                        ErrorFormatString::inconvertible, init_type->typeName(),
                        var_type->derefType()->typeName());

        builder.CreateStore(init_val, var_alloca);
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

ValueAndType genGlobalVariable(VarDeclNode* ast, CodeGenContext* helper,
                               const Generator& generator)
{
    assertWithError(!helper->globalSymbols.hasSymbol(ast->name()),
                    helper->error, "Redefinition of variable '{}'",
                    ast->name());

    Type* var_type = asEntityType(ast->type()->toType());

    assertWithError(var_type, helper->error, ErrorFormatString::illegal_type,
                    ast->type()->dump());

    // lhs must be a reference
    var_type = getReferenceType(var_type);

    // TODO: Global variable's initValue's processing
    llvm::GlobalVariable* globalVar = new llvm::GlobalVariable(
        helper->module, castToLLVM(var_type->derefType(), helper->llvm_ctx),
        var_type->isConst, llvm::GlobalVariable::ExternalLinkage, nullptr,
        ast->name().toStdString());

    helper->globalSymbols.addSymbol(
        {.name = ast->name(),
         .symbolType = XSharp::SymbolType::GlobalVariable,
         .type = var_type,
         .definition = globalVar});

    return {globalVar, var_type};
}

ValueAndType genDataMember(VarDeclNode* ast, CodeGenContext* helper,
                           const Generator& generator)
{
    if (helper->currentSymbols->hasSymbol(ast->name())) {
        helper->error("Redefinition of variable {}", ast->name());
        return {nullptr, nullptr};
    }
    return {nullptr, nullptr};
}

}  // namespace XSharp::LLVMCodeGen
