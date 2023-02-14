#include "LLVMIR/LLVMTypes.h"
static llvm::Type* llvmTypeFor(XSharp::TypeNode type)
{
    // TODO complete XSharp's Type to Variable
    using XSharp::TypeNode;
    switch (type.category) {
        case TypeNode::Basic:
            break;
        case TypeNode::Function:
            break;
        case TypeNode::Variable:
            break;
        case TypeNode::Class:
            break;
        default:
            break;
    }
}
