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
#include "LLVMIR/LLVMHelper.h"
#include "LLVMIR/LLVMTypes.h"
#include "LLVMIR/BuiltIn.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/ControlFlow/ControlFlowAST.h"
#include "XSharp/Symbol.h"
#include "XSharp/Types/Type.h"
#include "XSharp/Types/TypeAdapter.h"
#include "XSharp/Types/TypeSystem.h"
#include "XSharp/XSharpUtils.h"
#include "XSharp/XString.h"

LLVMHelper::LLVMHelper()
{
    // module.setDataLayout("");
    // module.setTargetTriple("i386-pc-linux-gnu");
    setUpBuildIn(contextHelper.module, contextHelper.context,
                 contextHelper.globalSymbols);

    generator = std::bind(&LLVMHelper::codegen, this, std::placeholders::_1);

    addProxy<DefinitionsNode>();
    addProxy<IntegerNode>();
    addProxy<BooleanNode>();
    addProxy<DecimalFractionNode>();
    addProxy<VariableExprNode>();
    addProxy<VariableNode>();
    addProxy<FunctionNode>();
    addProxy<FunctionCallNode>();
    addProxy<BoxNode>();
    addProxy<BlockNode>();
    addProxy<IfNode>();
    addProxy<WhileNode>();
    addProxy<ReturnNode>();
    addProxy<BinaryOperatorNode>();
    addProxy<UnaryOperatorNode>();
}

LLVMHelper::~LLVMHelper()
{
    for (auto pair : proxies) {
        delete pair.second;
    }
}

std::vector<std::byte> LLVMHelper::generateLLVMIR(ASTNode* ast,
                                                  const XString& filename)
{
    using namespace llvm;
    std::vector<std::byte> bytecodes;

    auto [val, type] = codegen(ast);
    if (!type) return bytecodes;

    SmallVector<char> buffer;
    BitcodeWriter bitcodeWriter(buffer);
    bitcodeWriter.writeModule(contextHelper.module);
    std::error_code code;
    llvm::raw_fd_ostream out(filename.toStdString(), code);
    WriteBitcodeToFile(contextHelper.module, out);
    contextHelper.module.dump();
    return bytecodes;
}

ValueAndType LLVMHelper::codegen(ASTNode* node)
{
    auto index = std::type_index(typeid(*node));
    return proxies[index]->codeGen(node, &contextHelper, generator);
}
