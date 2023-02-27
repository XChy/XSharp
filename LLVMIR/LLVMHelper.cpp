#include "LLVMIR/LLVMHelper.h"
#include <llvm-14/llvm/ADT/APFloat.h>
#include <llvm-14/llvm/ADT/APInt.h>
#include <llvm-14/llvm/ADT/SmallVector.h>
#include <llvm-14/llvm/Bitcode/BitcodeWriter.h>
#include <llvm-14/llvm/IR/BasicBlock.h>
#include <llvm-14/llvm/IR/Constant.h>
#include <llvm-14/llvm/IR/Constants.h>
#include <llvm-14/llvm/IR/DerivedTypes.h>
#include <llvm-14/llvm/IR/Function.h>
#include <llvm-14/llvm/IR/LLVMContext.h>
#include <llvm-14/llvm/IR/Module.h>
#include <memory>
#include <system_error>
#include "LLVMIR/LLVMTypes.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/XString.h"

LLVMHelper::LLVMHelper() : module("XSharp", context), builder(context)
{
    module.setDataLayout("");
    module.setTargetTriple("i386-pc-linux-gnu");
}

std::vector<std::byte> LLVMHelper::generateLLVMIR(ASTNode* ast,
                                                  const XString& filename)
{
    using namespace llvm;
    std::vector<std::byte> bytecodes;

    if (ast->is<DefinitionsNode>()) {
        DefinitionsNode* definitions = ast->to<DefinitionsNode>();
        for (auto var : definitions->variableDeclarations()) {
        }
        for (auto func : definitions->functionDeclarations()) {
        }
        for (auto classDef : definitions->classDeclarations()) {
        }
    }

    SmallVector<char> buffer;
    BitcodeWriter bitcodeWriter(buffer);
    bitcodeWriter.writeModule(module);
    std::error_code code;
    llvm::raw_fd_ostream out("anonymous.bc", code);
    WriteBitcodeToFile(module, out);
    module.dump();
    return bytecodes;
}

llvm::Function* LLVMHelper::genFunction(FunctionDeclarationNode* node)
{
    // TODO: SymbolTable-related
    using llvm::BasicBlock;
    using llvm::ConstantInt;
    using llvm::Function;

    std::vector<llvm::Type*> paramsType(node->params().size());
    for (auto param : node->params()) {
        auto paramType = param->type();
        paramsType.push_back(llvmTypeFor(&paramType, context));
    }

    auto retType = node->returnType();
    llvm::FunctionType* functionType = llvm::FunctionType::get(
        llvmTypeFor(&retType, context), paramsType, false);

    Function* func = Function::Create(functionType, Function::ExternalLinkage,
                                      "abc", module);

    BasicBlock* block = BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(block);

    for (auto content : node->impl()->contents()) {
        codegen(content);
    }
}

llvm::Value* LLVMHelper::genBinaryOp(BinaryOperatorNode* op)
{
    // TODO: BinaryOperatorNode to llvmIR
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
    }
    if (node->is<BinaryOperatorNode>()) {
        return genBinaryOp(node->to<BinaryOperatorNode>());
    }
    if (node->is<UnaryOperatorNode>()) {
        return genUnaryOp(node->to<UnaryOperatorNode>());
    }
    if (node->is<VariableDeclarationNode>()) {
    }
    if (node->is<FunctionDeclarationNode>()) {
        return genFunction(node->to<FunctionDeclarationNode>());
    }
    if (node->is<ClassDeclarationNode>()) {
    }
    if (node->is<FunctionCallNode>()) {
    }
    if (node->is<VariableNode>()) {
    }
    if (node->is<MemberNode>()) {
    }
    if (node->is<IndexNode>()) {
    }
    // TODO LLVMIR generation for Value-like ASTNode
    return nullptr;
}

XSharp::SymbolTable LLVMHelper::symbolTable() const { return symbols; }
