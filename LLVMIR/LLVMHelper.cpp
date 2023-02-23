#include "LLVMIR/LLVMHelper.h"
#include <llvm-14/llvm/ADT/APFloat.h>
#include <llvm-14/llvm/ADT/APInt.h>
#include <llvm-14/llvm/ADT/SmallVector.h>
#include <llvm-14/llvm/Bitcode/BitcodeWriter.h>
#include <llvm-14/llvm/IR/BasicBlock.h>
#include <llvm-14/llvm/IR/Constant.h>
#include <llvm-14/llvm/IR/Constants.h>
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
    LLVMContext context;

    Module module("XSharp", context);
    module.setDataLayout("");
    module.setTargetTriple("i386-pc-linux-gnu");

    IRBuilder<> builder(context);

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
    // TODO LLVMIR generation for Function Declaration
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
    }
    if (node->is<BinaryOperatorNode>()) {
    }
    if (node->is<UnaryOperatorNode>()) {
    }
    if (node->is<VariableDeclarationNode>()) {
    }
    if (node->is<FunctionDeclarationNode>()) {
    }
    if (node->is<ClassDeclarationNode>()) {
    }
    if (node->is<DefinitionsNode>()) {
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
}
