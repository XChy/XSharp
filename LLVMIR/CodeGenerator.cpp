#include "CodeGenerator.h"
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
#include <functional>
#include <typeindex>
#include "LLVMIR/LLVMTypes.h"
#include "LLVMIR/BuiltIn.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Class/ClassAST.h"
#include "XSharp/ControlFlow/ControlFlowAST.h"
#include "XSharp/Symbol.h"
#include "XSharp/Types/Type.h"
#include "XSharp/Types/TypeAdapter.h"
#include "XSharp/Types/TypeSystem.h"
#include "XSharp/XSharpUtils.h"
#include "XSharp/XString.h"

using namespace XSharp::LLVMCodeGen;

CodeGenerator::CodeGenerator()
{
    setUpBuildIn(ctx.module, ctx.llvm_ctx, ctx.globalSymbols);

    generator = std::bind(&CodeGenerator::codegen, this, std::placeholders::_1);

    addProxy<DefinitionsNode>();
    addProxy<IntegerNode>();
    addProxy<CharNode>();
    addProxy<StringNode>();
    addProxy<BooleanNode>();
    addProxy<FPNode>();
    addProxy<VarExprNode>();
    addProxy<VarDeclNode>();
    addProxy<FunctionNode>();
    addProxy<CallNode>();
    addProxy<BlockNode>();
    addProxy<IfNode>();
    addProxy<WhileNode>();
    addProxy<ContinueNode>();
    addProxy<BreakNode>();
    addProxy<ReturnNode>();
    addProxy<BinaryOperatorNode>();
    addProxy<UnaryOpNode>();
    addProxy<ClassNode>();
    addProxy<MemberExprNode>();
    addProxy<MemberMethodNode>();
    addProxy<ConstructorNode>();
    addProxy<IndexNode>();
}

CodeGenerator::~CodeGenerator()
{
    for (auto pair : proxies) {
        delete pair.second;
    }
}

std::vector<std::byte> CodeGenerator::generateIR(ASTNode* ast,
                                                 const XString& filename)
{
    using namespace llvm;
    std::vector<std::byte> bytecodes;

    auto [val, type] = codegen(ast);
    if (!type) return bytecodes;

    ctx.optimizer.modulePassManager.run(ctx.module);

    SmallVector<char> buffer;
    BitcodeWriter bitcodeWriter(buffer);
    bitcodeWriter.writeModule(ctx.module);
    std::error_code code;
    llvm::raw_fd_ostream out(filename.toStdString(), code);
    WriteBitcodeToFile(ctx.module, out);
    ctx.module.dump();
    return bytecodes;
}

std::vector<std::byte> CodeGenerator::generateTextIR(ASTNode *ast,
                                                     const XString &filename) {
using namespace llvm;
    std::vector<std::byte> bytecodes;

    auto [val, type] = codegen(ast);
    if (!type) return bytecodes;

    ctx.optimizer.modulePassManager.run(ctx.module);

    std::error_code code;
    llvm::raw_fd_ostream out(filename.toStdString(), code);
    ctx.module.print(out, nullptr);
    return bytecodes;

}

ValueAndType CodeGenerator::codegen(ASTNode* node)
{
    auto index = std::type_index(typeid(*node));
    return proxies[index]->codeGen(node, &ctx, generator);
}
