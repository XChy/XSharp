#include "ControlFlowProxies.h"
#include "LLVMIR/CodeGenProxy.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/ControlFlow/ControlFlowAST.h"
ValueAndType CodeGenProxy<IfNode>::codeGen(IfNode* ast,
                                           CodeGenContextHelper* helper,
                                           const Generator& generator)
{
    using llvm::BasicBlock;
    auto& builder = helper->builder;
    auto& context = helper->context;
    llvm::Function* currentFunc = builder.GetInsertBlock()->getParent();

    BasicBlock* thenBlock =
        BasicBlock::Create(context, "true_case", currentFunc);
    BasicBlock* elseBlock =
        BasicBlock::Create(context, "false_case", currentFunc);
    BasicBlock* endBlock = BasicBlock::Create(context, "endif", currentFunc);

    auto [cond_val, cond_type] = generator(ast->condition);
    cond_val =
        TypeAdapter::llvmConvert(cond_type, XSharp::getBooleanType(), cond_val);
    if (!cond_val) {
        helper->error("The condition of <if> is not a boolean");
        return {nullptr, nullptr};
    }
    builder.CreateCondBr(cond_val, thenBlock, elseBlock);

    // then
    builder.SetInsertPoint(thenBlock);
    auto [then_val, then_type] = generator(ast->block);
    builder.SetInsertPoint(thenBlock);
    builder.CreateBr(endBlock);

    // else
    builder.SetInsertPoint(elseBlock);
    if (ast->elseAst) {
        auto [else_val, else_type] = generator(ast->elseAst);
        if (!else_type) return {nullptr, nullptr};
    }

    builder.SetInsertPoint(elseBlock);
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

    BasicBlock* loopBlock =
        BasicBlock::Create(context, "while_loop", currentFunc);
    BasicBlock* endBlock = BasicBlock::Create(context, "endWhile", currentFunc);

    builder.SetInsertPoint(loopBlock);

    // Loop inner
    auto [cond_val, cond_type] = generator(ast->condition);
    cond_val = XSharp::TypeAdapter::llvmConvert(
        cond_type, XSharp::getBooleanType(), cond_val);
    if (!cond_val) {
        helper->error("The condition of <while> is not a boolean");
        return {nullptr, nullptr};
    }

    auto [then_val, then_type] = generator(ast->block);
    if (!then_type) return {nullptr, nullptr};

    builder.CreateBr(loopBlock);

    // end loop
    builder.SetInsertPoint(endBlock);

    return {nullptr, XSharp::getVoidType()};
}

ValueAndType CodeGenProxy<ReturnNode>::codeGen(ReturnNode* ast,
                                               CodeGenContextHelper* helper,
                                               const Generator& generator)
{
    using llvm::BasicBlock;
    auto& builder = helper->builder;
    auto& context = helper->context;

    auto [retVal, retType] = generator(ast->returnValue());
    // TODO: check the type of return value is valid
    return {builder.CreateRet(retVal), XSharp::getVoidType()};
}
