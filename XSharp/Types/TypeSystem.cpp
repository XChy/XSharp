#include "TypeSystem.h"
#include "XSharp/Types/Type.h"
#include "XSharp/Class/XClass.h"

using namespace XSharp;

TypeContext::TypeContext()
{
    std::unordered_map<BasicType, XString> basicTypeToName = {
        {BasicType::Void, "void"},       {BasicType::I32, "i32"},
        {BasicType::I64, "i64"},         {BasicType::UI32, "ui32"},
        {BasicType::UI64, "ui64"},       {BasicType::Float, "float"},
        {BasicType::Double, "double"},   {BasicType::Char, "char"},
        {BasicType::Boolean, "boolean"},
    };

    // fill a useless node so that typeID start with 1
    for (int i = 0; i < int(BasicType::ENDTYPE); ++i) {
        TypeNode* node = new TypeNode;
        node->category = TypeNode::Basic;
        node->typeSpecifiedInfo = BasicType(i);
        BasicType type = BasicType(i);
        node->baseName = basicTypeToName[type];
        registerType(node);
    }
}

TypeContext::~TypeContext()
{
    for (TypeNode* node : typeList) delete node;
}
TypeNode* TypeContext::registerType(XSharp::TypeNode* type)
{
    if (type->category == TypeNode::Class) {
        classes[type->typeName()] = type;
    }

    typeList.push_back(type);
    typeList[registerNum]->typeID = registerNum;
    return typeList[registerNum++];
}

TypeNode* TypeContext::registerClass(XClass* classDecl)
{
    TypeNode* type = new TypeNode;
    type->typeSpecifiedInfo = ClassType{.classDecl = classDecl};
    type->baseName = classDecl->name;
    registerType(type);
    return type;
}
TypeNode* TypeContext::typeOf(int typeId) { return typeList[typeId]; }

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
                                  const std::vector<TypeNode*>& paramsType)
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
    node->category = TypeNode::Reference;
    node->typeSpecifiedInfo = ReferenceType{.innerType = innerType};
    GlobalTypeContext.registerType(node);
    return node;
}

TypeNode* XSharp::getArrayType(TypeNode* elementType, uint dimension)
{
    TypeNode* node = new TypeNode;
    node->category = TypeNode::Array;
    node->typeSpecifiedInfo =
        ArrayType{.arrayDimension = dimension, .elementType = elementType};
    GlobalTypeContext.registerType(node);
    return node;
}

TypeNode* XSharp::getClassType(const XString& baseName)
{
    // TODO: support generics
    return GlobalTypeContext.classes[baseName];
}

TypeNode* XSharp::getClosureType()
{
    TypeNode* node = new TypeNode;
    node->category = TypeNode::Closure;
    node->typeSpecifiedInfo = ClosureType{};
    GlobalTypeContext.registerType(node);
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

TypeNode* XSharp::getMergedType(TypeNode* lhs_type, TypeNode* rhs_type)
{
    TypeNode* merged_type;
    if (lhs_type->isInteger() && rhs_type->isInteger()) {
        if ((lhs_type->isSigned() && rhs_type->isSigned()) ||
            (lhs_type->isUnsigned() && rhs_type->isUnsigned())) {
            merged_type =
                lhs_type->size() > rhs_type->size() ? lhs_type : rhs_type;
        } else {
            merged_type = lhs_type->isSigned() ? lhs_type : rhs_type;
        }
    } else if (lhs_type->isInteger() || rhs_type->isInteger()) {
        merged_type = lhs_type->isInteger() ? rhs_type : lhs_type;
    } else {
        merged_type = lhs_type->size() > rhs_type->size() ? lhs_type : rhs_type;
    }
    return merged_type;
}
