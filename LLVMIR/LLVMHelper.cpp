#include <llvm/IR/Argument.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/Alignment.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include "LLVMIR/LLVMHelper.h"
#include "LLVMIR/LLVMTypes.h"
#include "LLVMIR/BuiltIn.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/ControlFlow/ControlFlowAST.h"
#include "XSharp/Symbol.h"
#include "XSharp/Type.h"
#include "XSharp/TypeSystem.h"
#include "XSharp/XSharpUtils.h"
#include "XSharp/XString.h"

LLVMHelper::LLVMHelper()
    : module("XSharp", context), builder(context), optimizer(&module)
{
    // module.setDataLayout("");
    // module.setTargetTriple("i386-pc-linux-gnu");
    setUpBuildIn(module, context, globalSymbols);
    currentSymbols = &globalSymbols;
}

std::vector<std::byte> LLVMHelper::generateLLVMIR(ASTNode* ast,
                                                  const XString& filename)
{
    using namespace llvm;
    std::vector<std::byte> bytecodes;

    if (ast->is<DefinitionsNode>()) {
        DefinitionsNode* definitions = ast->to<DefinitionsNode>();
        for (auto var : definitions->variableDeclarations()) {
            auto [val, type] = genGlobalVariable(var);
        }
        for (auto funcNode : definitions->functionDeclarations()) {
            genFunction(funcNode);
        }
        for (auto classDef : definitions->classDeclarations()) {
        }
    }

    SmallVector<char> buffer;
    BitcodeWriter bitcodeWriter(buffer);
    bitcodeWriter.writeModule(module);
    std::error_code code;
    llvm::raw_fd_ostream out(filename.toStdString(), code);
    WriteBitcodeToFile(module, out);
    module.dump();
    return bytecodes;
}

ValueAndType LLVMHelper::genGlobalVariable(VariableDeclarationNode* varNode)
{
    if (globalSymbols.hasSymbol(varNode->name())) {
        errors.push_back(
            {XSharpErrorType::SemanticsError,
             fmt::format("Redefinition of variable {}", varNode->name())});
        return {nullptr, nullptr};
    }

    TypeNode* type = varNode->type();

    // TODO: variable's initValue's processing
    llvm::GlobalVariable* globalVar = new llvm::GlobalVariable(
        module, castToLLVM(type, context), type->isConst,
        llvm::GlobalVariable::ExternalLinkage, nullptr,
        varNode->name().toStdString());

    globalSymbols.addSymbol({.name = varNode->name(),
                             .symbolType = XSharp::SymbolType::GlobalVariable,
                             .type = XSharp::getReferenceType(type),
                             .definition = globalVar});
    return {globalVar, type};
}

ValueAndType LLVMHelper::genLocalVariable(VariableDeclarationNode* varNode)
{
    if (currentSymbols->hasSymbol(varNode->name())) {
        errors.push_back(
            {XSharpErrorType::SemanticsError,
             fmt::format("Redefinition of variable {}", varNode->name())});
        return {nullptr, nullptr};
    }

    TypeNode* xsharpType = XSharp::getReferenceType(varNode->type());

    // TODO: variable's initValue's processing
    auto alloca =
        builder.CreateAlloca(castToLLVM(xsharpType->innerType(), context),
                             nullptr, varNode->name().toStdString());

    currentSymbols->addSymbol({.name = varNode->name(),
                               .symbolType = XSharp::SymbolType::LocalVariable,
                               .type = xsharpType,
                               .definition = alloca});

    return {alloca, xsharpType};
}

ValueAndType LLVMHelper::genFunction(FunctionDeclarationNode* node)
{
    // TODO: SymbolTable-related
    using llvm::BasicBlock;
    using llvm::ConstantInt;
    using llvm::Function;

    if (currentSymbols->hasSymbol(node->name())) {
        errors.push_back(
            {XSharpErrorType::SemanticsError,
             fmt::format("Redefinition of function {}", node->name())});
        return {nullptr, nullptr};
    }

    std::vector<llvm::Type*> paramsType;
    for (auto param : node->params()) {
        auto paramType = param->type();
        paramsType.push_back(castToLLVM(paramType, context));
    }

    auto retType = node->returnType();
    llvm::FunctionType* functionType = llvm::FunctionType::get(
        castToLLVM(retType, context), paramsType, false);

    Function* func = Function::Create(functionType, Function::ExternalLinkage,
                                      node->name().toStdString(), module);

    BasicBlock* block = BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(block);

    auto iter = func->arg_begin();
    for (int i = 0; i < func->arg_size(); ++i) {
        auto arg_alloca = builder.CreateAlloca(iter->getType());
        builder.CreateStore(iter, arg_alloca);

        iter->setName(node->params()[i]->name().toStdString());
        currentSymbols->addSymbol(
            {.name = node->params()[i]->name(),
             .symbolType = XSharp::SymbolType::Argument,
             .type = XSharp::getReferenceType(node->params()[i]->type()),
             .definition = arg_alloca});
        iter++;
    }

    for (auto content : node->impl()->contents()) {
        codegen(content);
    }

    std::vector<TypeNode*> xParamsType;
    TypeNode* type = XSharp::getFunctionType(node->returnType(), xParamsType);
    for (auto var : node->params()) xParamsType.push_back(var->type());
    currentSymbols->addSymbol({.name = node->name(),
                               .symbolType = XSharp::SymbolType::Function,
                               .type = type,
                               .definition = func});

    optimizer.functionPassManager.run(*func);
    return {func, type};
}

ValueAndType LLVMHelper::genCall(FunctionCallNode* call)
{
    // TODO: Call's LLVMIR generation
    // Normal Function Call
    if (call->function()->is<VariableNode>()) {
        VariableNode* calleeNode = call->function()->to<VariableNode>();
        XString calleeName = calleeNode->name();
        std::vector<llvm::Value*> args;
        for (auto ast : call->params()) {
            auto [arg_val, arg_type] = codegen(ast);
            // TODO: args's typecheck

            // BasicType argument is passed as value
            if (arg_type->category == XSharp::TypeNode::Reference &&
                arg_type->innerType()->category == XSharp::TypeNode::Basic)
                args.push_back(builder.CreateLoad(
                    castToLLVM(arg_type->innerType(), context), arg_val));
            else
                args.push_back(arg_val);
        }

        // customed
        if (currentSymbols->hasSymbol(calleeName)) {
            auto symbol = currentSymbols->findSymbol(calleeName)->second;
            // TODO:typecheck
            return {builder.CreateCall(
                        ((llvm::Function*)symbol.definition)->getFunctionType(),
                        symbol.definition, args),
                    symbol.type->returnValueType()};
        }
        // builtin
        else if (module.getFunction(calleeName.toStdString())) {
            auto llvmFunction = module.getFunction(calleeName.toStdString());
            return {builder.CreateCall(llvmFunction->getFunctionType(),
                                       llvmFunction, args),
                    nullptr};
        } else {
            errors.push_back(
                {XSharpErrorType::SemanticsError,
                 fmt::format("The function '{}' doesn't exist", calleeName)});
            return {nullptr, nullptr};
        }
    }
    // This Call
    else if (call->function()->is<MemberNode>()) {
    }
}

ValueAndType LLVMHelper::genBinaryOp(BinaryOperatorNode* op)
{
    // TODO: Type reasoning, truncuation and verifying

    // Add
    if (op->operatorStr() == "+") {
        auto [lhs, lhs_type] = deReferenceIf(op->left());
        auto [rhs, rhs_type] = deReferenceIf(op->right());
        return {builder.CreateAdd(lhs, rhs), XSharp::getI64Type()};
    }

    // Sub
    if (op->operatorStr() == "-") {
        auto [lhs, lhs_type] = codegen(op->left());
        auto [rhs, rhs_type] = codegen(op->right());
        return {builder.CreateAdd(lhs, rhs), XSharp::getI64Type()};
    }

    // Div
    if (op->operatorStr() == "/") {
        auto [lhs, lhs_type] = codegen(op->left());
        auto [rhs, rhs_type] = codegen(op->right());
        return {builder.CreateAdd(lhs, rhs), XSharp::getI64Type()};
    }

    // Mul
    if (op->operatorStr() == "*") {
        auto [lhs, lhs_type] = codegen(op->left());
        auto [rhs, rhs_type] = codegen(op->right());
        return {builder.CreateAdd(lhs, rhs), XSharp::getI64Type()};
    }

    // Assign
    if (op->operatorStr() == "=") {
        auto [lhs, lhs_type] = codegen(op->left());
        auto [rhs, rhs_type] = deReferenceIf(op->right());
        if (lhs_type->category == XSharp::TypeNode::Reference) {
            return {builder.CreateStore(rhs, lhs), lhs_type};
        } else {
            errors.push_back(
                {XSharpErrorType::SemanticsError,
                 "The type of left operand of '=' must be reference"});
            return {nullptr, nullptr};
        }
    }
    return {nullptr, nullptr};
}

ValueAndType LLVMHelper::genUnaryOp(UnaryOperatorNode* op)
{
    // TODO: UnaryOperatorNode to llvmIR
    return {nullptr, nullptr};
}

ValueAndType LLVMHelper::genIf(XSharp::IfNode* ifNode)
{
    using llvm::BasicBlock;
    llvm::Function* currentFunc = builder.GetInsertBlock()->getParent();

    BasicBlock* thenBlock =
        BasicBlock::Create(context, "true_case", currentFunc);
    BasicBlock* elseBlock =
        BasicBlock::Create(context, "false_case", currentFunc);
    BasicBlock* endBlock = BasicBlock::Create(context, "endif", currentFunc);

    auto [cond_val, cond_type] = deReferenceIf(ifNode->condition);
    builder.CreateCondBr(cond_val, thenBlock, elseBlock);

    // then
    builder.SetInsertPoint(thenBlock);
    auto [then_val, then_type] = codegen(ifNode->block);
    builder.CreateBr(endBlock);

    // else
    builder.SetInsertPoint(elseBlock);
    if (ifNode->elseAst) auto [else_val, else_type] = codegen(ifNode->elseAst);
    builder.CreateBr(endBlock);

    // end
    builder.SetInsertPoint(endBlock);

    return {nullptr, XSharp::getVoidType()};
}

ValueAndType LLVMHelper::genWhile(XSharp::WhileNode* whileNode)
{
    using llvm::BasicBlock;
    llvm::Function* currentFunc = builder.GetInsertBlock()->getParent();

    BasicBlock* loopBlock =
        BasicBlock::Create(context, "while_loop", currentFunc);
    BasicBlock* endBlock = BasicBlock::Create(context, "endWhile", currentFunc);

    builder.SetInsertPoint(loopBlock);

    // Loop inner
    auto [cond_val, cond_type] = deReferenceIf(whileNode->condition);
    builder.CreateCondBr(cond_val, loopBlock, endBlock);
    auto [then_val, then_type] = codegen(whileNode->block);
    builder.CreateBr(loopBlock);

    // end loop
    builder.SetInsertPoint(endBlock);

    return {nullptr, XSharp::getVoidType()};
}

ValueAndType LLVMHelper::codegen(ASTNode* node)
{
    using namespace llvm;
    if (node == nullptr) return {nullptr, nullptr};

    if (node->is<IntegerNode>()) {
        auto val = ConstantInt::get(
            context, APInt(64, node->to<IntegerNode>()->value()));

        return {val, XSharp::getI64Type()};
    }

    if (node->is<DecimalFractionNode>()) {
        auto val = ConstantFP::get(
            context, APFloat(node->to<DecimalFractionNode>()->value()));

        return {val, XSharp::getI64Type()};
    }

    if (node->is<BooleanNode>()) {
        auto val = ConstantInt::get(context,
                                    APInt(1, node->to<BooleanNode>()->value()));
        return {val, XSharp::getBooleanType()};
    }

    if (node->is<StringNode>()) {
        // TODO String's LLVM generation
    }

    if (node->is<BoxNode>()) {
        return codegen(node->to<BoxNode>()->child());
    }
    if (node->is<BlockNode>()) {
        // TODO: Jump to next layer of SymbolTable
        for (auto ast : node->to<BlockNode>()->contents()) codegen(ast);
        return {nullptr, XSharp::getVoidType()};
    }
    if (node->is<BinaryOperatorNode>()) {
        return genBinaryOp(node->to<BinaryOperatorNode>());
    }
    if (node->is<UnaryOperatorNode>()) {
        return genUnaryOp(node->to<UnaryOperatorNode>());
    }
    if (node->is<VariableDeclarationNode>()) {
        return genLocalVariable(node->to<VariableDeclarationNode>());
    }
    if (node->is<FunctionDeclarationNode>()) {
        return genFunction(node->to<FunctionDeclarationNode>());
    }
    if (node->is<ClassDeclarationNode>()) {
    }
    if (node->is<FunctionCallNode>()) {
        return genCall(node->to<FunctionCallNode>());
    }
    if (node->is<XSharp::WhileNode>()) {
        return genWhile(node->to<XSharp::WhileNode>());
    }
    if (node->is<VariableNode>()) {
        VariableNode* var = node->to<VariableNode>();
        if (currentSymbols->hasSymbol(var->name())) {
            auto symbol = currentSymbols->at(var->name());
            return {symbol.definition, symbol.type};
        } else {
            errors.push_back(
                {XSharpErrorType::SemanticsError, "Variable {} doesn't exist"});
            return {nullptr, nullptr};
        }
    }
    if (node->is<XSharp::IfNode>()) {
        genIf(node->to<XSharp::IfNode>());
    }
    if (node->is<MemberNode>()) {
    }
    if (node->is<IndexNode>()) {
    }
    if (node->is<ReturnNode>()) {
        auto [retVal, retType] = codegen(node->to<ReturnNode>()->returnValue());
        return {builder.CreateRet(retVal), XSharp::getVoidType()};
    }
    // TODO LLVMIR generation for Value-like ASTNode
    return {nullptr, nullptr};
}

ValueAndType LLVMHelper::deReferenceIf(ASTNode* ast)
{
    auto [val, type] = codegen(ast);
    if (type->category == XSharp::TypeNode::Reference) {
        return {builder.CreateLoad(castToLLVM(type->innerType(), context), val),
                type->innerType()};
    } else {
        return {val, type};
    }
}

XSharp::SymbolTable LLVMHelper::symbolTable() const { return globalSymbols; }
