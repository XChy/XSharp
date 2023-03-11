#include "ControlFlowProxies.h"
#include <llvm-14/llvm/IR/BasicBlock.h>
#include <llvm-14/llvm/IR/Type.h>
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/ControlFlow/ControlFlowAST.h"
#include "XSharp/Types/TypeAdapter.h"
#include "XSharp/Types/TypeSystem.h"
ValueAndType CodeGenProxy<IfNode>::codeGen(IfNode* ast,
                                           CodeGenContextHelper* helper,
                                           const Generator& generator)
{
    using llvm::BasicBlock;
    auto& builder = helper->builder;
    auto& context = helper->context;
    llvm::Function* currentFunc = builder.GetInsertBlock()->getParent();

    auto [cond_val, cond_type] = generator(ast->condition);
    cond_val =
        TypeAdapter::llvmConvert(cond_type, XSharp::getBooleanType(), cond_val);
    if (!cond_val) {
        helper->error("The condition of <if> is not a boolean");
        return {nullptr, nullptr};
    }

    BasicBlock* thenBlock =
        BasicBlock::Create(context, "true_case", currentFunc);
    BasicBlock* elseBlock =
        BasicBlock::Create(context, "false_case", currentFunc);
    BasicBlock* endBlock = BasicBlock::Create(context, "endif", currentFunc);

    builder.CreateCondBr(cond_val, thenBlock, elseBlock);

    // then
    builder.SetInsertPoint(thenBlock);
    auto [then_val, then_type] = generator(ast->block);
    if (!then_type) return {nullptr, nullptr};

    builder.SetInsertPoint(thenBlock);
    builder.CreateBr(endBlock);

    // else
    builder.SetInsertPoint(elseBlock);
    if (ast->elseAst) {
        auto [else_val, else_type] = generator(ast->elseAst);
        if (!else_type) return {nullptr, nullptr};
    }
    builder.CreateBr(endBlock);

    // end
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
    BasicBlock* loopstart =
        BasicBlock::Create(context, "loop_start", currentFunc);
    BasicBlock* loopBlock =
        BasicBlock::Create(context, "loop_body", currentFunc);
    BasicBlock* endBlock = BasicBlock::Create(context, "endWhile", currentFunc);

    builder.CreateBr(loopstart);
    builder.SetInsertPoint(loopstart);

    auto [cond_val, cond_type] = generator(ast->condition);
    cond_val = XSharp::TypeAdapter::llvmConvert(
        cond_type, XSharp::getBooleanType(), cond_val);
    if (!cond_val) {
        helper->error("The condition of <while> is not a boolean");
        return {nullptr, nullptr};
    }
    builder.CreateCondBr(cond_val, loopBlock, endBlock);

    builder.SetInsertPoint(loopBlock);

    // Loop inner
    auto [then_val, then_type] = generator(ast->block);
    if (!then_type) return {nullptr, nullptr};

    builder.CreateBr(loopstart);

    // end loop
    builder.SetInsertPoint(endBlock);

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
