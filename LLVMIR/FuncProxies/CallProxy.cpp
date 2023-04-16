#include "CallProxy.h"
#include "LLVMIR/CodeGenHelper.h"
#include "LLVMIR/Utils.h"
#include "XSharp/ASTNodes.h"
#include "XSharp/Types/Type.h"
#include "XSharp/Types/TypeAdapter.h"
#include "XSharp/Types/TypeNodes.h"
#include "XSharp/Types/TypeSystem.h"
#include "XSharp/XString.h"

using namespace XSharp;
using namespace XSharp::LLVMCodeGen;

ValueAndType CodeGenProxy<FunctionCallNode>::codeGen(FunctionCallNode* ast,
                                                     CodeGenContext* helper,
                                                     const Generator& generator)
{
    using llvm::BasicBlock;
    auto& builder = helper->llvm_builder;
    auto& context = helper->llvm_ctx;
    auto& module = helper->module;

    using llvm::BasicBlock;
    using llvm::ConstantInt;
    using llvm::Function;

    if (ast->callee()->is<VariableExprNode>()) {
        XString calleeName = ast->callee()->to<VariableExprNode>()->name();

        std::vector<llvm::Value*> argumentValues;
        std::vector<Type*> argumentTypes;
        for (auto ast : ast->args()) {
            auto [arg_val, arg_type] = generator(ast);
            if (arg_type == nullptr) return {nullptr, nullptr};
            argumentTypes.push_back(arg_type);
            argumentValues.push_back(arg_val);
        }

        auto symbol =
            helper->currentSymbols->findFunctionFor(calleeName, argumentTypes);

        if (symbol.symbolType == XSharp::SymbolType::NoneSymbol) {
            helper->error("No matching function for '{} (...)'", calleeName);
            return {nullptr, nullptr};
        }

        for (int i = 0; i < argumentValues.size(); ++i) {
            auto arg = argumentValues[i];
            auto arg_type = argumentTypes[i];
            auto param_type = symbol.type->parameterTypes()[i];

            argumentValues[i] =
                XSharp::TypeAdapter::llvmConvert(arg_type, param_type, arg);

            assertWithError(argumentValues[i], helper->error,
                            ErrorFormatString::inconvertible,
                            arg_type->typeName(), param_type->typeName());
        }

        return {builder.CreateCall(symbol.function->getFunctionType(),
                                   symbol.definition, argumentValues),
                symbol.type->returnValueType()};

    } else if (ast->callee()->is<TypeNode>()) {
        auto callee_type =
            asEntityType(ast->callee()->to<TypeNode>()->toType());

        if (!callee_type) {
            helper->error("No type called '{}'", callee_type->typeName());
            return {nullptr, nullptr};
        }

        if (callee_type->isObject()) {
            auto malloc_code = genObjectMalloc(helper, callee_type);
            passErrorIfNot(malloc_code);

            return {malloc_code, callee_type};
        }

        if (callee_type->isArray()) {
            if (ast->args().size() != 1) {
                helper->error("Lack of arguments for array's initialization");
                return {nullptr, nullptr};
            }

            auto [length, length_type] = generator(ast->args()[0]);
            length = TypeAdapter::llvmConvert(length_type, Types::get("i64"),
                                              length);

            if (!length) {
                helper->error("Cannot take '{}' as arg for array constructor",
                              length_type->typeName());
                return {nullptr, nullptr};
            }

            auto malloc_code = genArrayMalloc(helper, callee_type, length);

            if (malloc_code)
                return {malloc_code, callee_type};
            else
                return {nullptr, nullptr};
        }
    }
    // this-call
    else if (ast->callee()->is<MemberExprNode>()) {
        auto [obj, obj_type] = deReference(
            generator(ast->callee()->to<MemberExprNode>()->object()), helper);
        passErrorIfNot(obj_type);

        XString memberName = ast->callee()->to<MemberExprNode>()->memberName();

        assertWithError(obj_type->isObject(), helper->error,
                        "Cannot get the member '{}' of non-object", memberName);

        XString calleeName = obj_type->getClassDecl()->name + ":" + memberName;

        std::vector<llvm::Value*> argumentValues = {obj};
        std::vector<Type*> argumentTypes = {obj_type};
        for (auto ast : ast->args()) {
            auto [arg_val, arg_type] = generator(ast);
            passErrorIfNot(arg_type);
            argumentTypes.push_back(arg_type);
            argumentValues.push_back(arg_val);
        }

        auto symbol =
            helper->currentSymbols->findFunctionFor(calleeName, argumentTypes);

        if (symbol.symbolType == XSharp::SymbolType::NoneSymbol) {
            helper->error("No matching function for '{} (...)'", calleeName);
            return {nullptr, nullptr};
        }

        for (int i = 0; i < argumentValues.size(); ++i) {
            auto arg = argumentValues[i];
            auto arg_type = argumentTypes[i];
            auto param_type = symbol.type->parameterTypes()[i];

            argumentValues[i] =
                XSharp::TypeAdapter::llvmConvert(arg_type, param_type, arg);

            assertWithError(argumentValues[i], helper->error,
                            ErrorFormatString::inconvertible,
                            arg_type->typeName(), param_type->typeName());
        }

        return {builder.CreateCall(symbol.function->getFunctionType(),
                                   symbol.definition, argumentValues),
                symbol.type->returnValueType()};

    }  // TODO: callable
    else {
    }

    return {nullptr, nullptr};
}
