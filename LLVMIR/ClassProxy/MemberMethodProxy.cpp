#include "MemberMethodProxy.h"
#include <llvm/ADT/APInt.h>
#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "LLVMIR/LLVMTypes.h"
#include "LLVMIR/Utils.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Class/ClassAST.h"
#include "XSharp/Types/TypeNodes.h"
#include "XSharp/Types/TypeSystem.h"
#include "XSharp/XString.h"

namespace XSharp::LLVMCodeGen {

ValueAndType CodeGenProxy<MemberMethodNode>::codeGen(
    MemberMethodNode* ast, CodeGenContextHelper* helper,
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

    XString funcName = ast->selfClass->name + ":" + ast->name();

    assertWithError(!helper->currentSymbols->hasSymbol(funcName), helper->error,
                    ErrorFormatString::redefinition_func, funcName);

    Symbol functionSymbol{
        .name = funcName,
        .symbolType = XSharp::SymbolType::Function,
    };

    std::vector<Type*> paramTypes;

    // regard 'self' as a parameter
    auto self_type = asEntityType(Types::get(ast->selfClass->name));
    assertWithError(self_type, helper->error, ErrorFormatString::illegal_type,
                    ast->selfClass->name);
    paramTypes.push_back(self_type);

    for (auto param : ast->params()) {
        auto paramType = asEntityType(param->type()->toType());
        assertWithError(paramType, helper->error,
                        ErrorFormatString::illegal_type, param->type()->dump());
        paramTypes.push_back(paramType);
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

    helper->currentSymbols->parent()->addSymbol(functionSymbol);

    BasicBlock* block = BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(block);

    // TODO: maybe support function definition or lambda in function?
    helper->toNewFunctionScope(functionSymbol);

    // the first argument is self
    auto iter = func->arg_begin();
    auto self_alloca = builder.CreateAlloca(iter->getType());
    builder.CreateStore(iter, self_alloca);
    iter->setName("self");

    helper->currentSymbols->addSymbol(
        {.name = "self",
         .symbolType = XSharp::SymbolType::Argument,
         .type = getReferenceType(self_type),
         .definition = self_alloca});

    iter++;

    for (int i = 1; i < func->arg_size(); ++i) {
        auto arg_alloca = builder.CreateAlloca(iter->getType());
        builder.CreateStore(iter, arg_alloca);

        iter->setName(ast->params()[i - 1]->name().toStdString());

        helper->currentSymbols->addSymbol(
            {.name = ast->params()[i - 1]->name(),
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

}  // namespace XSharp::LLVMCodeGen
