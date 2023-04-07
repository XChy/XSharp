#include "TypeSystem.h"
#include <cmath>
#include "XSharp/Types/Type.h"
#include "XSharp/Class/XClass.h"

using namespace XSharp;

TypeContext globalTypeContext;

TypeContext::TypeContext()
{
    basicTypeToName = {
        {BasicType::Void, "void"},       {BasicType::I8, "i8"},
        {BasicType::U8, "u8"},           {BasicType::I16, "i16"},
        {BasicType::U16, "u16"},         {BasicType::I32, "i32"},
        {BasicType::U32, "u32"},         {BasicType::I64, "i64"},
        {BasicType::U64, "u64"},         {BasicType::Float, "float"},
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

Type* Types::get(const XString& name)
{
    auto it = globalTypeContext.types.find(name);

    if (it != globalTypeContext.types.end()) return it->second;

    return nullptr;
}

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
        classDecls[type->typeName()] = type->getClassDecl();
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
    node->typeinfo = FunctionType{
        .paramTypes = paramTypes,
        .returnValueType = returnValueType,
        .isInitializer = false,
        .isMethod = false,
        .isVarArgs = false,
    };
    if (!Types::get(node->typeName())) {
        globalTypeContext.registerType(node);
        return node;
    } else {
        auto typeName = node->typeName();
        delete node;
        return Types::get(typeName);
    }
}

Type* XSharp::getReferenceType(Type* innerType)
{
    Type* node = new Type;
    node->category = Type::Reference;
    node->typeinfo = ReferenceType{.derefType = innerType};

    if (!Types::get(node->typeName())) {
        globalTypeContext.registerType(node);
        return node;
    } else {
        auto typeName = node->typeName();
        delete node;
        return Types::get(typeName);
    }

    return node;
}

Type* XSharp::asEntityType(Type* type)
{
    if (type == nullptr) return nullptr;

    // Object
    if (type->isClass()) {
        // TODO: generics
        return getReferenceType(type);
    }

    return type;
}

Type* XSharp::getArrayType(Type* elementType, uint dimension)
{
    Type* node = new Type;
    node->category = Type::Array;
    node->typeinfo =
        ArrayType{.arrayDimension = dimension, .elementType = elementType};

    if (!Types::get(node->typeName())) {
        globalTypeContext.registerType(node);
        return node;
    } else {
        auto typeName = node->typeName();
        delete node;
        return Types::get(typeName);
    }
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
    if (!Types::get(node->typeName())) {
        globalTypeContext.registerType(node);
        return node;
    } else {
        auto typeName = node->typeName();
        delete node;
        return Types::get(typeName);
    }
}

Type* XSharp::getMergedType(Type* lhs_type, Type* rhs_type)
{
    Type* merged_type = nullptr;
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

Type* XSharp::registerClass(XClass* classDecl)
{
    globalTypeContext.classDecls[classDecl->name] = classDecl;

    Type* type = new Type;
    type->category = Type::Class;
    type->typeinfo = ClassType{.classDecl = classDecl};
    type->baseName = classDecl->name;

    globalTypeContext.registerType(type);
    return type;
}

TypeContext* getGlobalTypeContext() { return &globalTypeContext; }
