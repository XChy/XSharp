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

ValueAndType CodeGenProxy<IfNode>::codeGen(IfNode* ast, CodeGenContext* ctx,
                                           const Generator& generator)
{
    using llvm::BasicBlock;
    auto& builder = ctx->llvm_builder;
    auto& context = ctx->llvm_ctx;
    llvm::Function* currentFunc = builder.GetInsertBlock()->getParent();
    BasicBlock* currentBlock = builder.GetInsertBlock();

    auto [cond_val, cond_type] = generator(ast->condition);
    passErrorIfNot(cond_type);

    cond_val =
        TypeAdapter::llvmConvert(cond_type, XSharp::getBooleanType(), cond_val);
    assertWithError(cond_val, ctx->error, ErrorFormatString::inconvertible,
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
                                              CodeGenContext* ctx,
                                              const Generator& generator)
{
    using llvm::BasicBlock;
    auto& builder = ctx->llvm_builder;
    auto& context = ctx->llvm_ctx;

    using llvm::BasicBlock;
    llvm::Function* currentFunc = builder.GetInsertBlock()->getParent();
    BasicBlock* loop_cond =
        BasicBlock::Create(context, "while.cond", currentFunc);
    BasicBlock* loop_body = BasicBlock::Create(context, "while.body");
    BasicBlock* loop_end = BasicBlock::Create(context, "while.end");

    if (!builder.GetInsertBlock()->getTerminator()) builder.CreateBr(loop_cond);

    builder.SetInsertPoint(loop_cond);

    ctx->enterScope();
    ctx->loops.push(
        Loop{.cond = loop_cond, .body = loop_body, .end = loop_end});

    auto [cond_val, cond_type] = generator(ast->condition);
    passErrorIfNot(cond_type);

    cond_val = XSharp::TypeAdapter::llvmConvert(
        cond_type, XSharp::getBooleanType(), cond_val);
    assertWithError(cond_val, ctx->error, ErrorFormatString::inconvertible,
                    cond_type->typeName(), "boolean");

    if (!builder.GetInsertBlock()->getTerminator())
        builder.CreateCondBr(cond_val, loop_body, loop_end);

    // Loop inner
    loop_body->insertInto(currentFunc);
    builder.SetInsertPoint(loop_body);

    auto [then_val, then_type] = generator(ast->block);
    passErrorIfNot(then_type);

    ctx->exitScope();

    if (!builder.GetInsertBlock()->getTerminator()) builder.CreateBr(loop_cond);

    // end loop
    loop_end->insertInto(currentFunc);
    builder.SetInsertPoint(loop_end);

    return {nullptr, XSharp::getVoidType()};
}

ValueAndType CodeGenProxy<ContinueNode>::codeGen(ContinueNode* ast,
                                                 CodeGenContext* ctx,
                                                 const Generator& generator)
{
    using llvm::BasicBlock;
    assertWithError(!ctx->loops.empty(), ctx->error, "No loop to continue", 0);

    if (!ctx->llvm_builder.GetInsertBlock()->getTerminator()) {
        ctx->llvm_builder.CreateBr(ctx->loops.top().cond);
        BasicBlock* deadblock =
            BasicBlock::Create(ctx->llvm_ctx, "deadblock",
                               ctx->llvm_builder.GetInsertBlock()->getParent());
        ctx->llvm_builder.SetInsertPoint(deadblock);
    }

    return {nullptr, getVoidType()};
}

ValueAndType CodeGenProxy<BreakNode>::codeGen(BreakNode* ast,
                                              CodeGenContext* ctx,
                                              const Generator& generator)
{
    using llvm::BasicBlock;
    assertWithError(!ctx->loops.empty(), ctx->error, "No loop to break", 0);

    if (!ctx->llvm_builder.GetInsertBlock()->getTerminator()) {
        auto i = ctx->llvm_builder.CreateBr(ctx->loops.top().end);
        BasicBlock* deadblock =
            BasicBlock::Create(ctx->llvm_ctx, "deadblock",
                               ctx->llvm_builder.GetInsertBlock()->getParent());
        ctx->llvm_builder.SetInsertPoint(deadblock);
    }

    return {nullptr, getVoidType()};
}

ValueAndType CodeGenProxy<ReturnNode>::codeGen(ReturnNode* ast,
                                               CodeGenContext* ctx,
                                               const Generator& generator)
{
    auto& builder = ctx->llvm_builder;
    auto& context = ctx->llvm_ctx;

    if (ast->returnValue()) {
        auto [ret_val, ret_type] = generator(ast->returnValue());
        passErrorIfNot(ret_type);

        ret_val =
            TypeAdapter::llvmConvert(ret_type, ctx->retTypes.top(), ret_val);

        assertWithError(ret_val, ctx->error, ErrorFormatString::inconvertible,
                        ret_type->typeName(), ctx->retTypes.top()->typeName());

        return {builder.CreateRet(ret_val), XSharp::getVoidType()};
    } else {
        return {builder.CreateRetVoid(), getVoidType()};
    }
}
