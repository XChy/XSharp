#include <vector>
#include "LLVMIR/LLVMHelper.h"
#include "XSharp/ASTNodes.h"

int main()
{
    // LLVMHelper::generateLLVMIR(nullptr, "");

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

    std::vector<Type*> params(1, Type::getDoubleTy(*context));
    FunctionType* functionType =
        FunctionType::get(Type::getDoubleTy(*context), params, false);
    Function* func = Function::Create(functionType, Function::ExternalLinkage,
                                      "abc", module.get());

    BasicBlock* block = BasicBlock::Create(*context, "entry", func);
    builder.SetInsertPoint(block);
    auto value = ConstantInt::get(*context, APInt(64, 10086));
    builder.CreateRet(value);

    // llvm::raw_fd_ostream out("anonymous.bc", code);
    // WriteBitcodeToFile(*module, out);
    module->dump();
    return 0;
}
