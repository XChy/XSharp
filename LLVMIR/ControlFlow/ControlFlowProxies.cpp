#include "ControlFlowProxies.h"
#include <llvm-14/llvm/IR/BasicBlock.h>
#include <llvm-14/llvm/IR/Type.h>
#include "LLVMIR/CodeGenProxy.h"
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
    cond_val =
        TypeAdapter::llvmConvert(cond_type, XSharp::getBooleanType(), cond_val);
    if (!cond_val) {
        helper->error("The condition of <if> is not a boolean");
        return {nullptr, nullptr};
    }

    BasicBlock* thenBlock = BasicBlock::Create(context, "if.then", currentFunc);
    BasicBlock* elseBlock = BasicBlock::Create(context, "if.else");
    BasicBlock* endBlock = BasicBlock::Create(context, "if.end");

    builder.CreateCondBr(cond_val, thenBlock, elseBlock);

    // then
    builder.SetInsertPoint(thenBlock);
    auto [then_val, then_type] = generator(ast->block);
    if (!then_type) return {nullptr, nullptr};

    if (!builder.GetInsertBlock()->getTerminator()) builder.CreateBr(endBlock);

    // else
    elseBlock->insertInto(currentFunc);
    builder.SetInsertPoint(elseBlock);
    if (ast->elseAst) {
        auto [else_val, else_type] = generator(ast->elseAst);
        if (!else_type) return {nullptr, nullptr};
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
    cond_val = XSharp::TypeAdapter::llvmConvert(
        cond_type, XSharp::getBooleanType(), cond_val);
    if (!cond_val) {
        helper->error("The condition of <while> is not a boolean");
        return {nullptr, nullptr};
    }
    builder.CreateCondBr(cond_val, loop_body, loop_end);

    // Loop inner
    loop_body->insertInto(currentFunc);
    builder.SetInsertPoint(loop_body);
    auto [then_val, then_type] = generator(ast->block);
    if (!then_type) return {nullptr, nullptr};

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
        auto [retVal, retType] = generator(ast->returnValue());
        if (!retType) return {nullptr, nullptr};

        retVal = TypeAdapter::llvmConvert(retType, helper->currentReturnType,
                                          retVal);
        if (!retVal) {
            helper->error("Cannot convert '{}' to '{}'", retType->typeName(),
                          helper->currentReturnType->typeName());
            return {nullptr, nullptr};
        }

        return {builder.CreateRet(retVal), XSharp::getVoidType()};
    } else {
        return {builder.CreateRetVoid(), getVoidType()};
    }
    // TODO: check the type of return value is valid
}
