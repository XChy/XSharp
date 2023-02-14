#include "LLVMIR/LLVMHelper.h"
#include <llvm-14/llvm/ADT/SmallVector.h>
#include <llvm-14/llvm/Bitcode/BitcodeWriter.h>
#include <llvm-14/llvm/IR/BasicBlock.h>
#include <llvm-14/llvm/IR/LLVMContext.h>
#include <memory>
#include <system_error>
#include "XSharp/XString.h"

std::vector<std::byte> LLVMHelper::generateLLVMIR(ASTNode* ast,
                                                  const XString& filename)
{
    using namespace llvm;
    std::vector<std::byte> bytecodes;
    std::unique_ptr<LLVMContext> context = std::make_unique<LLVMContext>();

    std::unique_ptr<Module> module =
        std::make_unique<Module>("XSharp", *context);
    module->setDataLayout("");
    module->setTargetTriple("i386-pc-linux-gnu");

    IRBuilder<> builder(*context);

    SmallVector<char> buffer;
    BitcodeWriter bitcodeWriter(buffer);
    bitcodeWriter.writeModule(*module);
    std::error_code code;
    llvm::raw_fd_ostream out("anonymous.bc", code);
    WriteBitcodeToFile(*module, out);
    module->dump();
    return bytecodes;
}

llvm::Function* LLVMHelper::genFunction(FunctionDeclarationNode* node)
{
    // TODO LLVMIR generation for Function Declaration
}

llvm::Value* LLVMHelper::codegen(ASTNode* node)
{
    // TODO LLVMIR generation for Value-like ASTNode
}
