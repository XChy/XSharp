#include "TypeSystem.h"
#include <algorithm>
#include "XSharp/Type.h"

using namespace XSharp;

TypeContext::TypeContext()
{
    // fill a useless node so that typeID start with 1
    for (int i = 0; i < int(BasicType::ENDTYPE); ++i) {
        TypeNode* node = new TypeNode;
        node->category = TypeNode::Basic;
        node->typeSpecifiedInfo = BasicType(i);
        node->baseName = basicTypeToName[BasicType(i)];
        registerType(node);
    }
}

TypeContext::~TypeContext()
{
    for (TypeNode* node : typesList) delete node;
}
TypeNode* TypeContext::registerType(XSharp::TypeNode* type)
{
    registerNum++;

    typesMap[type->typeName()] = registerNum;

    typesList.push_back(type);
    typesList[registerNum]->typeID = registerNum;
    return typesList[registerNum];
}

TypeNode* TypeContext::typeOf(int typeId) { return typesList[typeId]; }

uint TypeContext::typeIDOf(XString name)
{
    auto iterator_found = typesMap.find(name);

    if (iterator_found != typesMap.end()) {
        return iterator_found->second;
    }

    return 0;
}

TypeNode* XSharp::getBasicType(BasicType type)
{
    return GlobalTypeContext.typeOf(int(type));
}

TypeNode* XSharp::getFunctionType(TypeNode* returnValueType,
                                  std::vector<TypeNode*> paramsType)
{
    TypeNode* node = new TypeNode;
    node->category = TypeNode::Function;
    node->typeSpecifiedInfo = FunctionType{.paramTypes = paramsType,
                                           .returnValueType = returnValueType};
    GlobalTypeContext.registerType(node);
    return node;
}

TypeNode* XSharp::getReferenceType(TypeNode* innerType)
{
    TypeNode* node = new TypeNode;
    node->category = TypeNode::Function;
    node->typeSpecifiedInfo = ReferenceType{.innerType = innerType};
    return node;
}

TypeNode* XSharp::getArrayType(TypeNode* elementType, uint dimension)
{
    TypeNode* node = new TypeNode;
    node->category = TypeNode::Array;
    node->typeSpecifiedInfo =
        ArrayType{.arrayDimension = dimension, .elementType = elementType};
    return node;
}

TypeNode* XSharp::getClassType(const XString& baseName)
{
    TypeNode* node = new TypeNode;
    node->category = TypeNode::Class;
    node->typeSpecifiedInfo = ClassType{};
    return node;
}

TypeNode* XSharp::getClosureType()
{
    TypeNode* node = new TypeNode;
    node->category = TypeNode::Closure;
    node->typeSpecifiedInfo = ClosureType{};
    return node;
}

TypeNode* XSharp::getTypeFor(const XString& baseName)
{
    auto mapIter = nameToBasicType.find(baseName);
    if (mapIter != nameToBasicType.end()) {
        return getBasicType(nameToBasicType[baseName]);
    } else {
        return getClassType(baseName);
    }
}
