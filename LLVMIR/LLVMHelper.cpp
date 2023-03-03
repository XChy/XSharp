#include "LLVMIR/LLVMHelper.h"
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
#include <algorithm>
#include <vector>
#include "LLVMIR/LLVMTypes.h"
#include "LLVMIR/BuiltIn.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Type.h"
#include "XSharp/TypeSystem.h"
#include "XSharp/XSharpUtils.h"
#include "XSharp/XString.h"

LLVMHelper::LLVMHelper() : module("XSharp", context), builder(context)
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
            genGlobalVariable(var);
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

llvm::GlobalVariable* LLVMHelper::genGlobalVariable(
    VariableDeclarationNode* varNode)
{
    if (globalSymbols.hasSymbol(varNode->name())) {
        errors.push_back(
            {XSharpErrorType::SemanticsError, "Redefinition of variable"});
        return nullptr;
    }

    TypeNode* typenode =
        XSharp::GlobalTypeContext.registerType(varNode->type());

    // TODO: variable's initValue's processing
    llvm::GlobalVariable* global = new llvm::GlobalVariable(
        module, llvmTypeFor(typenode, context), typenode->isConst,
        llvm::GlobalVariable::ExternalLinkage, nullptr,
        varNode->name().toStdString());

    globalSymbols.addSymbol({.name = varNode->name(),
                             .symbolType = XSharp::SymbolType::GlobalVariable,
                             .valueType = typenode,
                             .definition = global});
}

llvm::AllocaInst* LLVMHelper::genLocalVariable(VariableDeclarationNode* varNode)
{
    // TODO: FIX BUGS of Var
    if (currentSymbols->hasSymbol(varNode->name())) {
        errors.push_back(
            {XSharpErrorType::SemanticsError, "Redefinition of variable"});
        return nullptr;
    }

    TypeNode* typenode =
        XSharp::GlobalTypeContext.registerType(varNode->type());

    // TODO: variable's initValue's processing
    auto xsharpType = varNode->type();
    auto llvmValue =
        builder.CreateAlloca(llvmTypeFor(xsharpType, context), nullptr,
                             varNode->name().toStdString());

    currentSymbols->addSymbol({.name = varNode->name(),
                               .symbolType = XSharp::SymbolType::LocalVariable,
                               .valueType = typenode,
                               .definition = llvmValue});
}

llvm::Function* LLVMHelper::genFunction(FunctionDeclarationNode* node)
{
    // TODO: SymbolTable-related
    using llvm::BasicBlock;
    using llvm::ConstantInt;
    using llvm::Function;

    if (currentSymbols->hasSymbol(node->name())) {
        errors.push_back(
            {XSharpErrorType::SemanticsError, "Redefinition of function:{}"});
        return nullptr;
    }

    std::vector<llvm::Type*> paramsType;
    for (auto param : node->params()) {
        auto paramType = param->type();
        paramsType.push_back(llvmTypeFor(paramType, context));
    }

    auto retType = node->returnType();
    llvm::FunctionType* functionType = llvm::FunctionType::get(
        llvmTypeFor(retType, context), paramsType, false);

    Function* func = Function::Create(functionType, Function::ExternalLinkage,
                                      node->name().toStdString(), module);

    BasicBlock* block = BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(block);

    auto iter = func->arg_begin();
    for (int i = 0; i < func->arg_size(); ++i) {
        iter->setName(node->params()[i]->name().toStdString());
        iter++;
    }

    for (auto content : node->impl()->contents()) {
        codegen(content);
    }

    std::vector<TypeNode*> xParamsType;
    for (auto var : node->params()) xParamsType.push_back(var->type());
    // TODO: complete type for function
    currentSymbols->addSymbol(
        {.name = node->name(),
         .symbolType = XSharp::SymbolType::Function,
         .valueType = XSharp::getFunctionType(node->returnType(), xParamsType),
         .definition = func});

    return func;
}

llvm::CallInst* LLVMHelper::genCall(FunctionCallNode* call)
{
    // TODO: Call's LLVMIR generation
    // Normal Function Call
    if (call->function()->is<VariableNode>()) {
        VariableNode* calleeNode = call->function()->to<VariableNode>();
        XString calleeName = calleeNode->name();
        // TODO: args's typecheck
        std::vector<llvm::Value*> args;
        for (auto ast : call->params()) args.push_back(codegen(ast));

        // std::for_each(call->params().begin(), call->params().end(),
        //[&](ASTNode* var) { args.push_back(codegen(var)); });
        // customed
        if (currentSymbols->hasSymbol(calleeName)) {
            auto symbol = currentSymbols->findSymbol(calleeName)->second;
            // TODO:typecheck
            builder.CreateCall(
                (llvm::FunctionType*)llvmTypeFor(symbol.valueType, context),
                symbol.definition, args);
        }
        // builtin
        else if (module.getFunction(calleeName.toStdString())) {
            auto llvmFunction = module.getFunction(calleeName.toStdString());
            return builder.CreateCall(llvmFunction->getFunctionType(),
                                      llvmFunction, args);
        } else {
            errors.push_back({XSharpErrorType::SemanticsError,
                              "The function '{}' doesn't exist"});
            return nullptr;
        }
    }
    // This Call
    else if (call->function()->is<MemberNode>()) {
    }
}

llvm::Value* LLVMHelper::genBinaryOp(BinaryOperatorNode* op)
{
    // TODO: Type reasoning, truncuation and verifying

    // Add
    if (op->operatorStr() == "+") {
        auto lhs = codegen(op->left());
        auto rhs = codegen(op->right());
        return builder.CreateAdd(lhs, rhs);
    }

    // Sub
    if (op->operatorStr() == "-") {
        return builder.CreateSub(codegen(op->left()), codegen(op->right()));
    }

    // Div
    if (op->operatorStr() == "/") {
        return builder.CreateSDiv(codegen(op->left()), codegen(op->right()));
    }

    // Mul
    if (op->operatorStr() == "*") {
        return builder.CreateMul(codegen(op->left()), codegen(op->right()));
    }

    // Assign
    if (op->operatorStr() == "=") {
        return builder.CreateStore(codegen(op->right()), codegen(op->left()));
    }
    return nullptr;
}

llvm::Value* LLVMHelper::genUnaryOp(UnaryOperatorNode* op)
{
    // TODO: UnaryOperatorNode to llvmIR
    return nullptr;
}

llvm::Value* LLVMHelper::codegen(ASTNode* node)
{
    using namespace llvm;
    if (node == nullptr) return nullptr;

    if (node->is<IntegerNode>()) {
        return ConstantInt::get(context,
                                APInt(64, node->to<IntegerNode>()->value()));
    }

    if (node->is<DecimalFractionNode>()) {
        return ConstantFP::get(
            context, APFloat(node->to<DecimalFractionNode>()->value()));
    }

    if (node->is<BooleanNode>()) {
        return ConstantInt::get(context,
                                APInt(1, node->to<BooleanNode>()->value()));
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
        return nullptr;
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
    if (node->is<VariableNode>()) {
        VariableNode* var = node->to<VariableNode>();
        if (symbolTable().hasSymbol(var->name()))
            return symbolTable()[var->name()].definition;
        else {
            errors.push_back(
                {XSharpErrorType::SemanticsError, "Variable {} doesn't exist"});
            return nullptr;
        }
    }
    if (node->is<MemberNode>()) {
    }
    if (node->is<IndexNode>()) {
    }
    if (node->is<ReturnNode>()) {
        return builder.CreateRet(
            codegen(node->to<ReturnNode>()->returnValue()));
    }
    // TODO LLVMIR generation for Value-like ASTNode
    return nullptr;
}

XSharp::SymbolTable LLVMHelper::symbolTable() const { return globalSymbols; }
