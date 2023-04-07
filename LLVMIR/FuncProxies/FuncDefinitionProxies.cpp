#include "FuncDefinitionProxies.h"
#include <llvm/IR/Function.h>
#include <llvm/IR/Verifier.h>
#include <cstdio>
#include "LLVMIR/LLVMTypes.h"
#include "LLVMIR/Utils.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Symbol.h"
#include "XSharp/Types/Type.h"
#include "XSharp/Types/TypeNodes.h"
#include "XSharp/Types/TypeSystem.h"

using namespace XSharp;
using namespace XSharp::LLVMCodeGen;

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

    assertWithError(!helper->currentSymbols->hasSymbol(ast->name()),
                    helper->error, ErrorFormatString::redefinition_func,
                    ast->name());

    Symbol functionSymbol{
        .name = ast->name(),
        .symbolType = XSharp::SymbolType::Function,
    };

    std::vector<Type*> paramTypes;
    for (auto param : ast->params()) {
        auto paramType = asEntityType(param->type()->toType());
        paramTypes.push_back(paramType);
        assertWithError(paramType, helper->error,
                        ErrorFormatString::illegal_type, param->type()->dump());
    }

    auto retType = asEntityType(ast->returnType()->toType());
    assertWithError(retType, helper->error, ErrorFormatString::illegal_type,
                    ast->returnType()->dump());

    Type* functionType = XSharp::getFunctionType(retType, paramTypes);

    Function* func = Function::Create(
        (llvm::FunctionType*)castToLLVM(functionType, context),
        Function::ExternalLinkage, ast->name().toStdString(), module);

    functionSymbol.type = functionType;
    functionSymbol.function = func;

    helper->currentSymbols->addSymbol(functionSymbol);

    BasicBlock* block = BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(block);

    // TODO: maybe support function definition or lambda in function?
    helper->toNewFunctionScope(functionSymbol);

    // transform parameters to arguments
    auto iter = func->arg_begin();
    for (int i = 0; i < func->arg_size(); ++i) {
        auto arg_alloca = builder.CreateAlloca(iter->getType());
        builder.CreateStore(iter, arg_alloca);

        iter->setName(ast->params()[i]->name().toStdString());
        helper->currentSymbols->addSymbol(
            {.name = ast->params()[i]->name(),
             .symbolType = XSharp::SymbolType::Argument,
             .type = XSharp::getReferenceType(paramTypes[i]),
             .definition = arg_alloca});
        iter++;
    }

    auto [impl, impl_type] = generator(ast->impl());
    if (!impl_type) {
        func->eraseFromParent();
        return {nullptr, nullptr};
    }

    if (!builder.GetInsertBlock()->getTerminator()) {
        if (helper->currentReturnType->isBasic() &&
            helper->currentReturnType->basicType() == BasicType::Void) {
            builder.CreateRetVoid();
        } else {
            helper->error(
                "There must be a terminator/returner for the function {}",
                ast->name());
            return {nullptr, nullptr};
        }
    }

    helper->exitScope();

    // for debug
    // llvm::verifyFunction(*func);

    // optimize the function
    helper->optimizer.functionPassManager.run(*func);

    return {func, functionType};
}
