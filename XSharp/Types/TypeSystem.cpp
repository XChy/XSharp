#include "TypeSystem.h"
#include "XSharp/Types/Type.h"
#include "XSharp/Class/XClass.h"

using namespace XSharp;

TypeContext globalTypeContext;

TypeContext::TypeContext()
{
    basicTypeToName = {
        {BasicType::Void, "void"},       {BasicType::I8, "i8"},
        {BasicType::UI8, "ui8"},         {BasicType::I16, "i16"},
        {BasicType::UI16, "ui16"},       {BasicType::I32, "i32"},
        {BasicType::UI32, "ui32"},       {BasicType::I64, "i64"},
        {BasicType::UI64, "ui64"},       {BasicType::Float, "float"},
        {BasicType::Double, "double"},   {BasicType::Char, "char"},
        {BasicType::Boolean, "boolean"},
    };

    for (int i = 0; i < int(BasicType::ENDTYPE); ++i) {
        Type* node = new Type;
        node->category = Type::Basic;
        node->typeinfo = BasicType(i);
        BasicType type = BasicType(i);
        node->baseName = basicTypeToName[type];
        registerType(node);
    }
}

Type* Types::get(const XString& name) { return globalTypeContext.types[name]; }

TypeContext::~TypeContext()
{
    for (auto [name, type] : types) {
        delete type;
    }

    for (auto [_, classDecl] : classDecls) {
        delete classDecl;
    }
}
void TypeContext::registerType(Type* type)
{
    if (type->category == Type::Class) {
        classDecls[type->typeName()] = type->getObjectClass();
    }

    types[type->typeName()] = type;
}

void TypeContext::registerTypeIf(Type* type)
{
    if (types.contains(type->typeName())) return;
    registerType(type);
}

Type* XSharp::getBasicType(BasicType type)
{
    auto name = globalTypeContext.basicTypeToName[type];
    return Types::get(name);
}

Type* XSharp::getFunctionType(Type* returnValueType,
                              const std::vector<Type*>& paramTypes)
{
    Type* node = new Type;
    node->category = Type::Function;
    node->typeinfo = FunctionType{.paramTypes = paramTypes,
                                  .returnValueType = returnValueType};
    globalTypeContext.registerType(node);
    return node;
}

Type* XSharp::getReferenceType(Type* innerType)
{
    Type* node = new Type;
    node->category = Type::Reference;
    node->typeinfo = ReferenceType{.derefType = innerType};
    globalTypeContext.registerType(node);
    return node;
}

Type* XSharp::getArrayType(Type* elementType, uint dimension)
{
    Type* node = new Type;
    node->category = Type::Array;
    node->typeinfo =
        ArrayType{.arrayDimension = dimension, .elementType = elementType};
    globalTypeContext.registerType(node);
    return node;
}

Type* XSharp::getClassType(const XString& baseName)
{
    // TODO: support generics
    return Types::get(baseName);
}

Type* XSharp::getClosureType()
{
    Type* node = new Type;
    node->category = Type::Closure;
    node->typeinfo = ClosureType{};
    globalTypeContext.registerType(node);
    return node;
}

Type* XSharp::getTypeFor(const XString& baseName)
{
    auto mapIter = nameToBasicType.find(baseName);
    if (mapIter != nameToBasicType.end()) {
        return getBasicType(nameToBasicType[baseName]);
    } else {
        return getClassType(baseName);
    }
}

Type* XSharp::getMergedType(Type* lhs_type, Type* rhs_type)
{
    Type* merged_type;
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

TypeContext* getGlobalTypeContext() { return &globalTypeContext; }
