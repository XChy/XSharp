#include "Type.h"
#include <variant>
#include "XSharp/XString.h"
using namespace XSharp;

TypeNode::TypeNode() {}

TypeNode::TypeNode(const TypeNode& other)
{
    typeID = other.typeID;
    baseName = other.baseName;
    category = other.category;
    isConst = other.isConst;
    switch (category) {
        case Basic:
            typeSpecifiedInfo = std::get<BasicType>(other.typeSpecifiedInfo);
            break;
        case Array: {
            ArrayType array = std::get<ArrayType>(other.typeSpecifiedInfo);
            array.elementType = new TypeNode(*other.elementType());
            typeSpecifiedInfo = array;
        } break;
        case Function: {
            FunctionType function;
            function.returnValueType = new TypeNode(*other.returnValueType());
            for (TypeNode* param : other.parameterTypes())
                function.paramTypes.push_back(new TypeNode(*param));
            typeSpecifiedInfo = function;
        } break;
        case Class:
            // TODO class-related
            break;
        case Closure:
            break;
    }
}

TypeNode* TypeNode::innerType() const
{
    return std::get<ReferenceType>(typeSpecifiedInfo).innerType;
}
bool TypeNode::equals(const TypeNode& other) const
{
    if (this == &other) return true;
    if (this->category != other.category) return false;

    switch (category) {
        case Basic:
            return this->basicType() == other.basicType();
        case Reference:
            return this->innerType()->equals(other.innerType());
        case Array:
            return arrayDimension() == other.arrayDimension() &&
                   elementType()->equals(*other.elementType());
        case Function:
            if (!returnValueType()->equals(*other.returnValueType()))
                return false;
            if (parameterTypes().size() != other.parameterTypes().size())
                return false;
            for (int i = 0; i < parameterTypes().size(); ++i) {
                if (!parameterTypes()[i]->equals(*other.parameterTypes()[i])) {
                    return false;
                }
            }
            return true;
        case Class:
            // TODO Class related
            return true;
        case Closure:
            // TODO Closure related
            return true;
    }
    return false;
}

TypeNode* TypeNode::returnValueType() const
{
    return std::get<FunctionType>(typeSpecifiedInfo).returnValueType;
}
std::vector<TypeNode*> TypeNode::parameterTypes() const
{
    return std::get<FunctionType>(typeSpecifiedInfo).paramTypes;
}

// Array type, TODO complete below
uint TypeNode::arrayDimension() const
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
            return elementType()->typeName() + "[]*" +
                   XString::fromInterger(arrayDimension());
        case Function:
            XString name = returnValueType()->typeName();
            name.append('(');
            for (TypeNode* param : parameterTypes())
                name.append(param->typeName());
            name.append(')');
            return name;
    }
    return "";
}

BasicType TypeNode::basicType() const
{
    return std::get<BasicType>(typeSpecifiedInfo);
}

bool TypeNode::isInteger() const
{
    return category == Basic &&
           (basicType() == BasicType::I32 || basicType() == BasicType::I64 ||
            basicType() == BasicType::UI32 || basicType() == BasicType::UI64);
}

bool TypeNode::isSigned() const
{
    return category == Basic &&
           (basicType() == BasicType::I32 || basicType() == BasicType::I64);
}

bool TypeNode::isUnsigned() const
{
    return category == Basic &&
           (basicType() == BasicType::UI32 || basicType() == BasicType::UI64);
}
