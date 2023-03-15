#include "UnaryOpImpl.h"
using namespace XSharp;

ValueAndType XSharp::PositiveImpl(UnaryOperatorNode* op,
                                  CodeGenContextHelper* helper,
                                  const Generator& generator)
{
    auto [operand, operand_type] =
        deReference(generator(op->operand()), helper);

    if (!operand_type) return {nullptr, nullptr};

    if (!operand_type->isNumber()) {
        // TODO: Support customed operator
        helper->error("Cannot get the positive value of non-numbers");
        return {nullptr, nullptr};
    }

    return {operand, operand_type};
}

ValueAndType XSharp::NegativeImpl(UnaryOperatorNode* op,
                                  CodeGenContextHelper* helper,
                                  const Generator& generator)
{
    auto [operand, operand_type] =
        deReference(generator(op->operand()), helper);

    if (!operand_type) return {nullptr, nullptr};

    if (!operand_type->isNumber()) {
        // TODO: Support customed operator
        helper->error("Cannot get the positive value of non-numbers");
        return {nullptr, nullptr};
    }
    // TODO: impliment the minus LLVMIR
    return {operand, operand_type};
}

ValueAndType XSharp::NotImpl(UnaryOperatorNode* op,
                             CodeGenContextHelper* helper,
                             const Generator& generator)
{
    auto [operand, operand_type] =
        deReference(generator(op->operand()), helper);

    if (!operand_type) return {nullptr, nullptr};
    if (operand_type->basicType() == BasicType::Boolean) {
        // TODO: Support customed operator
        helper->error("Cannot get the positive value of non-numbers");
        return {nullptr, nullptr};
    }
    // TODO: impliment the minus LLVMIR
    return {operand, operand_type};
}
