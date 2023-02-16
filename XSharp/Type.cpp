#include "Type.h"
#include <variant>
#include "XSharp/XString.h"
using namespace XSharp;

TypeNode::TypeNode() {}

TypeNode::~TypeNode()
{
    switch (category) {
        case Array:
            delete elementType();
            break;
        case Function:
            for (TypeNode* param : paramsType()) delete param;
            delete returnValueType();
            break;
        case Class:
            // TODO Class-related
            break;
        case Closure:
            // TODO Closure-related
            break;
    }
}

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
            for (TypeNode* param : other.paramsType())
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

TypeNode* TypeNode::returnValueType() const
{
    return std::get<FunctionType>(typeSpecifiedInfo).returnValueType;
}
std::vector<TypeNode*> TypeNode::paramsType() const
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
            for (TypeNode* param : paramsType()) name.append(param->typeName());
            name.append(')');
            return name;
    }
}

BasicType TypeNode::basicType() const
{
    return std::get<BasicType>(typeSpecifiedInfo);
}

TypeNode* createBasicType(BasicType type)
{
    TypeNode* node = new TypeNode;
    node->typeSpecifiedInfo = type;
    switch (type) {
        case BasicType::I32:
            node->baseName = "i32";
        case BasicType::I64:
            node->baseName = "i64";
        case BasicType::UI32:
            node->baseName = "ui32";
        case BasicType::UI64:
            node->baseName = "ui64";
        case BasicType::Float:
            node->baseName = "float";
        case BasicType::Double:
            node->baseName = "double";
        case BasicType::Boolean:
            node->baseName = "boolean";
        case BasicType::Char:
            node->baseName = "char";
    }
}

TypeNode* createFunctionType(TypeNode* returnValueType,
                             std::vector<TypeNode*> paramsType)
{
    TypeNode* node = new TypeNode;
    node->typeSpecifiedInfo = FunctionType{.paramTypes = paramsType,
                                           .returnValueType = returnValueType};
    return node;
}

TypeNode* createArrayType(TypeNode* elementType, uint dimension)
{
    TypeNode* node = new TypeNode;
    node->typeSpecifiedInfo =
        ArrayType{.arrayDimension = dimension, .elementType = elementType};
    return node;
}

TypeNode* createClassType()
{
    TypeNode* node = new TypeNode;
    node->typeSpecifiedInfo = ClassType{};
    return node;
}

TypeNode* createClosureType()
{
    TypeNode* node = new TypeNode;
    node->typeSpecifiedInfo = ClosureType{};
    return node;
}
