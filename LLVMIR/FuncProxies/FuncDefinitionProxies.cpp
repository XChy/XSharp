#include "FuncDefinitionProxies.h"
#include <llvm/IR/Function.h>
#include <llvm/IR/Verifier.h>
#include <cstdio>
#include "LLVMIR/LLVMTypes.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Symbol.h"
#include "XSharp/Types/Type.h"
#include "XSharp/Types/TypeSystem.h"

ValueAndType CodeGenProxy<FunctionNode>::codeGen(FunctionNode* ast,
                                                 CodeGenContextHelper* helper,
                                                 const Generator& generator)
{
    using llvm::BasicBlock;
    using XSharp::Symbol;
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

    Symbol functionSymbol{
        .name = ast->name(),
        .symbolType = XSharp::SymbolType::Function,
    };

    std::vector<TypeNode*> paramsType;
    for (auto param : ast->params()) paramsType.push_back(param->type());

    auto retType = ast->returnType();
    TypeNode* functionType = XSharp::getFunctionType(retType, paramsType);

    Function* func = Function::Create(
        (llvm::FunctionType*)castToLLVM(functionType, context),
        Function::ExternalLinkage, ast->name().toStdString(), module);
    func->setGC("shadow-stack");

    functionSymbol.type = functionType;
    functionSymbol.function = func;

    BasicBlock* block = BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(block);

    // TODO: maybe support function definition or lambda in function?
    helper->toNewFunctionScope(functionSymbol);

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

    auto [impl, impl_type] = generator(ast->impl());
    if (!impl_type) {
        func->eraseFromParent();
        return {nullptr, nullptr};
    }

    if (!builder.GetInsertBlock()->getTerminator()) {
        helper->error("There must be a terminator/returner for the function {}",
                      ast->name());
        return {nullptr, nullptr};
    }

    helper->toParentScope();

    helper->currentSymbols->addSymbol(functionSymbol);

    // for debug
    llvm::verifyFunction(*func);

    // optimize the function
    // helper->optimizer.functionPassManager.run(*func);

    return {func, functionType};
}
