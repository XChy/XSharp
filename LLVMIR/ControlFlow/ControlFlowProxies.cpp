#include "ControlFlowProxies.h"
#include <llvm-14/llvm/IR/BasicBlock.h>
#include <llvm-14/llvm/IR/Type.h>
#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/CodeGenProxy.h"
#include "LLVMIR/Utils.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/ControlFlow/ControlFlowAST.h"
#include "XSharp/Types/TypeAdapter.h"
#include "XSharp/Types/TypeSystem.h"

using namespace XSharp;
using namespace XSharp::LLVMCodeGen;

ValueAndType CodeGenProxy<IfNode>::codeGen(IfNode* ast,
                                           CodeGenContextHelper* helper,
                                           const Generator& generator)
{
    using llvm::BasicBlock;
    auto& builder = helper->builder;
    auto& context = helper->context;
    llvm::Function* currentFunc = builder.GetInsertBlock()->getParent();
    BasicBlock* currentBlock = builder.GetInsertBlock();

    auto [cond_val, cond_type] = generator(ast->condition);
    passErrorIfNot(cond_type);

    cond_val =
        TypeAdapter::llvmConvert(cond_type, XSharp::getBooleanType(), cond_val);
    assertWithError(cond_val, helper->error, ErrorFormatString::inconvertible,
                    cond_type->typeName(), "boolean");

    BasicBlock* thenBlock = BasicBlock::Create(context, "if.then", currentFunc);
    BasicBlock* elseBlock = BasicBlock::Create(context, "if.else");
    BasicBlock* endBlock = BasicBlock::Create(context, "if.end");

    builder.CreateCondBr(cond_val, thenBlock, elseBlock);

    // then
    builder.SetInsertPoint(thenBlock);
    auto [then_val, then_type] = generator(ast->block);
    passErrorIfNot(then_type);

    if (!builder.GetInsertBlock()->getTerminator()) builder.CreateBr(endBlock);

    // else
    elseBlock->insertInto(currentFunc);
    builder.SetInsertPoint(elseBlock);
    if (ast->elseAst) {
        auto [else_val, else_type] = generator(ast->elseAst);
        passErrorIfNot(else_type);
    }
    if (!builder.GetInsertBlock()->getTerminator()) builder.CreateBr(endBlock);

    // end
    endBlock->insertInto(currentFunc);
    builder.SetInsertPoint(endBlock);

    return {nullptr, XSharp::getVoidType()};
}

ValueAndType CodeGenProxy<WhileNode>::codeGen(WhileNode* ast,
                                              CodeGenContextHelper* helper,
                                              const Generator& generator)
{
    using llvm::BasicBlock;
    auto& builder = helper->builder;
    auto& context = helper->context;

    using llvm::BasicBlock;
    llvm::Function* currentFunc = builder.GetInsertBlock()->getParent();
    BasicBlock* loop_cond =
        BasicBlock::Create(context, "while.cond", currentFunc);
    BasicBlock* loop_body = BasicBlock::Create(context, "while.body");
    BasicBlock* loop_end = BasicBlock::Create(context, "while.end");

    builder.CreateBr(loop_cond);
    builder.SetInsertPoint(loop_cond);

    auto [cond_val, cond_type] = generator(ast->condition);
    passErrorIfNot(cond_type);

    cond_val = XSharp::TypeAdapter::llvmConvert(
        cond_type, XSharp::getBooleanType(), cond_val);
    assertWithError(cond_val, helper->error, ErrorFormatString::inconvertible,
                    cond_type->typeName(), "boolean");

    builder.CreateCondBr(cond_val, loop_body, loop_end);

    // Loop inner
    loop_body->insertInto(currentFunc);
    builder.SetInsertPoint(loop_body);

    auto [then_val, then_type] = generator(ast->block);
    passErrorIfNot(then_type);

    builder.CreateBr(loop_cond);

    // end loop
    loop_end->insertInto(currentFunc);
    builder.SetInsertPoint(loop_end);

    return {nullptr, XSharp::getVoidType()};
}

ValueAndType CodeGenProxy<ReturnNode>::codeGen(ReturnNode* ast,
                                               CodeGenContextHelper* helper,
                                               const Generator& generator)
{
    auto& builder = helper->builder;
    auto& context = helper->context;

    if (ast->returnValue()) {
        auto [ret_val, ret_type] = generator(ast->returnValue());
        passErrorIfNot(ret_type);

        ret_val = TypeAdapter::llvmConvert(ret_type, helper->currentReturnType,
                                           ret_val);
        assertWithError(ret_val, helper->error,
                        ErrorFormatString::inconvertible, ret_type->typeName(),
                        helper->currentReturnType->typeName());

        return {builder.CreateRet(ret_val), XSharp::getVoidType()};
    } else {
        return {builder.CreateRetVoid(), getVoidType()};
    }
    // TODO: check the type of return value is valid
}
