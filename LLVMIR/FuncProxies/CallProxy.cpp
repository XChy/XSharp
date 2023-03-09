#include "CallProxy.h"
#include "XSharp/ASTNodes.h"

ValueAndType CodeGenProxy<FunctionCallNode>::codeGen(
    FunctionCallNode* ast, CodeGenContextHelper* helper,
    const Generator& generator)
{
    using llvm::BasicBlock;
    auto& builder = helper->builder;
    auto& context = helper->context;
    auto& module = helper->module;

    using llvm::BasicBlock;
    using llvm::ConstantInt;
    using llvm::Function;

    if (ast->function()->is<VariableNode>()) {
        VariableNode* asteeNode = ast->function()->to<VariableNode>();
        XString asteeName = asteeNode->name();

        std::vector<llvm::Value*> argumentValues;
        std::vector<TypeNode*> argumentTypes;
        for (auto ast : ast->params()) {
            auto [arg_val, arg_type] = generator(ast);
            if (arg_type == nullptr) return {nullptr, nullptr};
            argumentTypes.push_back(arg_type);
            argumentValues.push_back(arg_val);
        }

        auto symbol =
            helper->currentSymbols->findFunctionFor(asteeName, argumentTypes);

        if (symbol.symbolType == XSharp::SymbolType::NoneSymbol) {
            helper->error("No matching function for '{} (...)'", asteeName);
            return {nullptr, nullptr};
        }

        return {builder.CreateCall(symbol.function->getFunctionType(),
                                   symbol.definition, argumentValues),
                symbol.type->returnValueType()};
    }
    // TODO: thisast
    else if (ast->function()->is<MemberNode>()) {
    }
    // TODO: astable
    else {
    }

    return {nullptr, nullptr};
}
