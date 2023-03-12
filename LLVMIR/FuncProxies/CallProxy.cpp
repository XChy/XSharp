#include "CallProxy.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Types/TypeAdapter.h"

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

        for (int i = 0; i < argumentValues.size(); ++i) {
            auto arg = argumentValues[i];
            auto arg_type = argumentTypes[i];
            auto param_type = symbol.type->parameterTypes()[i];
            argumentValues[i] =
                XSharp::TypeAdapter::llvmConvert(arg_type, param_type, arg);
            if (!argumentValues[i]) {
                helper->error("Cannot convert argument to '{}'",
                              param_type->typeName());
                return {nullptr, nullptr};
            }
        }

        return {builder.CreateCall(symbol.function->getFunctionType(),
                                   symbol.definition, argumentValues),
                symbol.type->returnValueType()};
    }
    // TODO: thiscall
    else if (ast->function()->is<MemberNode>()) {
    }
    // TODO: callable
    else {
    }

    return {nullptr, nullptr};
}
