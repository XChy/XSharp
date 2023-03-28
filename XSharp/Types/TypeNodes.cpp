#include "TypeNodes.h"
#include "XSharp/Types/Type.h"
#include "XSharp/Types/TypeSystem.h"
#include "XSharp/XString.h"
#include "fmt/format.h"

using namespace XSharp;

XString TypeNode::dump() const
{
    if (toType())
        return toType()->typeName();
    else
        return "";
}

Type* IdentifierNode::toType() const { return Types::get(name); }
XString IdentifierNode::dump() const { return name; }

ArrayNode::ArrayNode() : element(nullptr) {}

ArrayNode::ArrayNode(TypeNode* element, int dimension)
    : element(element), dimension(dimension)
{
}

ArrayNode::~ArrayNode() { delete element; }

Type* ArrayNode::toType() const
{
    auto elementType = element->toType();

    if (elementType) return getArrayType(element->toType(), dimension);

    return nullptr;
}

XString ArrayNode::dump() const
{
    return fmt::format("Array<{}, {}>", element->dump(), dimension);
}

GenericsNode::GenericsNode() {}
GenericsNode::~GenericsNode()
{
    for (auto p : args) {
        delete p;
    }
}

Type* GenericsNode::toType() const
{
    // TODO: GenericsNode
    return nullptr;
}

XString GenericsNode::dump() const
{
    std::vector<std::string> args_format;

    for (auto arg : args) args_format.push_back(arg->dump().toStdString());

    return fmt::format("{}<{}>", prototype->dump(),
                       fmt::join(args_format, ","));
}
