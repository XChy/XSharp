#include "Type.h"
#include <variant>
#include "XSharp/XString.h"
using namespace XSharp;

TypeNode::TypeNode()
{  // TODO complete constructor
}

TypeNode::TypeNode(const TypeNode& other)
{
    // TODO complete constructor
}

bool TypeNode::equals(const TypeNode& other) const
{
    if (this->category != other.category) {
        return false;
    }
    switch (category) {
        case Basic:
            return this->basicType() == other.basicType();
        case Array:
            return arrayDimension() == other.arrayDimension() &&
                   arraySize() == other.arrayDimension() &&
                   elementType()->equals(*other.elementType());
        case Function:
            if (!returnValueType()->equals(*other.returnValueType()))
                return false;
            if (paramsType().size() != other.paramsType().size()) return false;
            for (int i = 0; i < paramsType().size(); ++i) {
                if (!paramsType()[i]->equals(*other.paramsType()[i])) {
                    return false;
                }
            }
            return true;
        case Class:
            // TODO Class related
            return true;
    }
}

TypeNode* TypeNode::returnValueType() const {}
std::vector<TypeNode*> paramsType() const {}

// Array type, TODO complete below
uint TypeNode::arrayDimension() const
{
    return std::get<ArrayType>(typeSpecifiedInfo).arrayDimension;
}

uint TypeNode::arraySize() const
{
    return std::get<ArrayType>(typeSpecifiedInfo).arrayDimension;
}

TypeNode* TypeNode::elementType() const
{
    return std::get<ArrayType>(typeSpecifiedInfo).elementType;
}

// Class type,  TODO complete below

// generate a unique name for a type
XString TypeNode::typeName() const
{
    switch (category) {
        case Basic:
            return baseName;
        case Class:
            return "Class";
        case Array:
            return elementType()->typeName() + "[" + arraySize() + "]";
        case Function:
            XString name = returnValueType()->typeName();
            name.append('(');
            for (TypeNode* param : paramsType()) name.append(param->typeName());
            name.append(')');
            return name;
    }
}

BasicType TypeNode::basicType() const
{
    return std::get<BasicType>(typeSpecifiedInfo);
}

TypeNode::~TypeNode()
{
    // TODO complete deconstructor
}
