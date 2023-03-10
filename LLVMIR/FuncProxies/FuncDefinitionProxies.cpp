#include "FuncDefinitionProxies.h"
#include <cstdio>
#include "XSharp/ASTNodes.h"

ValueAndType CodeGenProxy<FunctionDeclarationNode>::codeGen(
    FunctionDeclarationNode* ast, CodeGenContextHelper* helper,
    const Generator& generator)
{
    using llvm::BasicBlock;
    auto& builder = helper->builder;
    auto& context = helper->context;
    auto& module = helper->module;

    using llvm::BasicBlock;
    using llvm::ConstantInt;
    using llvm::Function;

    if (helper->currentSymbols->hasSymbol(ast->name())) {
        helper->error("Redefinition of function {}", ast->name());
        return {nullptr, nullptr};
    }

    std::vector<llvm::Type*> paramsType;
    for (auto param : ast->params()) {
        auto paramType = param->type();
        paramsType.push_back(castToLLVM(paramType, context));
    }

    auto retType = ast->returnType();
    llvm::FunctionType* functionType = llvm::FunctionType::get(
        castToLLVM(retType, context), paramsType, false);

    Function* func = Function::Create(functionType, Function::ExternalLinkage,
                                      ast->name().toStdString(), module);

    BasicBlock* block = BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(block);

    auto iter = func->arg_begin();
    for (int i = 0; i < func->arg_size(); ++i) {
        auto arg_alloca = builder.CreateAlloca(iter->getType());
        builder.CreateStore(iter, arg_alloca);

        iter->setName(ast->params()[i]->name().toStdString());
        helper->currentSymbols->addSymbol(
            {.name = ast->params()[i]->name(),
             .symbolType = XSharp::SymbolType::Argument,
             .type = XSharp::getReferenceType(ast->params()[i]->type()),
             .definition = arg_alloca});
        iter++;
    }

    for (auto content : ast->impl()->contents()) {
        generator(content);
    }

    std::vector<TypeNode*> xParamsType;
    for (auto var : ast->params()) {
        xParamsType.push_back(var->type());
    }
    TypeNode* type = XSharp::getFunctionType(ast->returnType(), xParamsType);
    helper->currentSymbols->addSymbol(
        {.name = ast->name(),
         .symbolType = XSharp::SymbolType::Function,
         .type = type,
         .definition = func});

    helper->optimizer.functionPassManager.run(*func);
    return {func, type};
}
