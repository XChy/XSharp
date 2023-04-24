#include "ConstructorProxy.h"
#include "LLVMIR/LLVMTypes.h"
#include "XSharp/Types/TypeNodes.h"
#include "XSharp/Class/ClassAST.h"
#include "XSharp/Types/Type.h"
#include "LLVMIR/CodeGenProxy.h"
#include "LLVMIR/Utils.h"

namespace XSharp::LLVMCodeGen {

ValueAndType CodeGenProxy<ConstructorNode>::codeGen(ConstructorNode* ast,
                                                    CodeGenContext* ctx,
                                                    const Generator& generator)
{
    using llvm::BasicBlock;
    using XSharp::Symbol;
    auto& builder = ctx->llvm_builder;
    auto& context = ctx->llvm_ctx;
    auto& module = ctx->module;

    using llvm::BasicBlock;
    using llvm::ConstantInt;
    using llvm::Function;

    XString funcName = ast->selfClass->name + ":new";

    assertWithError(!ctx->currentSymbols->hasSymbol(funcName), ctx->error,
                    ErrorFormatString::redefinition_func, funcName);

    Symbol functionSymbol{
        .name = funcName,
        .symbolType = XSharp::SymbolType::Function,
    };

    std::vector<Type*> paramTypes;

    // regard 'self' as a parameter
    auto self_type = asEntityType(Types::get(ast->selfClass->name));
    assertWithError(self_type, ctx->error, ErrorFormatString::illegal_type,
                    ast->selfClass->name);

    for (auto param : ast->parameters) {
        auto paramType = asEntityType(param->type()->toType());
        assertWithError(paramType, ctx->error, ErrorFormatString::illegal_type,
                        param->type()->dump());
        paramTypes.push_back(paramType);
    }

    Type* retType = self_type;
    Type* functionType = XSharp::getFunctionType(retType, paramTypes);

    Function* func = Function::Create(
        (llvm::FunctionType*)castToLLVM(functionType, context),
        Function::ExternalLinkage, funcName.toStdString(), module);

    functionSymbol.type = functionType;
    functionSymbol.function = func;

    ctx->currentSymbols->parent()->addSymbol(functionSymbol);

    BasicBlock* block = BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(block);

    // TODO: maybe support function definition or lambda in function?
    ctx->enterFunctionScope(functionSymbol);

    // implicitly emit self-malloc code
    auto self_alloca =
        builder.CreateAlloca(castToLLVM(self_type, ctx->llvm_ctx));
    auto malloced_object =
        genObjectMalloc(ctx, Types::get(ast->selfClass->name));
    builder.CreateStore(malloced_object, self_alloca);

    ctx->currentSymbols->addSymbol({.name = "self",
                                    .symbolType = XSharp::SymbolType::Argument,
                                    .type = getReferenceType(self_type),
                                    .definition = self_alloca});

    // alloca arguments
    auto iter = func->arg_begin();
    for (int i = 0; i < func->arg_size(); ++i) {
        auto arg_alloca = builder.CreateAlloca(iter->getType());
        builder.CreateStore(iter, arg_alloca);

        iter->setName(ast->parameters[i]->name().toStdString());

        ctx->currentSymbols->addSymbol(
            {.name = ast->parameters[i]->name(),
             .symbolType = XSharp::SymbolType::Argument,
             .type = XSharp::getReferenceType(paramTypes[i]),
             .definition = arg_alloca});

        iter++;
    }

    auto [impl, impl_type] = generator(ast->impl);
    if (!impl_type) {
        func->eraseFromParent();
        return {nullptr, nullptr};
    }

    // no return for constructor
    if (builder.GetInsertBlock()->getTerminator()) {
        ctx->error("Extra terminator/returner for the constructor {}",
                   funcName);
        return {nullptr, nullptr};
    } else {
        auto loaded_object = builder.CreateLoad(
            castToLLVM(self_type, ctx->llvm_ctx), self_alloca);
        builder.CreateRet(loaded_object);
    }

    ctx->exitFunctionScope();

    // for debug
    // llvm::verifyFunction(*func);

    // optimize the function
    ctx->optimizer.functionPassManager.run(*func);

    return {func, functionType};
}

}  // namespace XSharp::LLVMCodeGen
