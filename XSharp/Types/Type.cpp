#include "Type.h"
#include <cstdint>
#include <variant>
#include "XSharp/XString.h"
using namespace XSharp;

Type::Type() {}

bool Type::isRef() const { return category == Reference; }

Type* Type::derefType() const
{
    return std::get<ReferenceType>(typeinfo).derefType;
}

bool Type::equals(const Type& other) const
{
    if (this == &other) return true;
    if (this->category != other.category) return false;

    switch (category) {
        case Basic:
            return this->basicType() == other.basicType();
        case Reference:
            return this->derefType()->equals(other.derefType());
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

            return this->isInitializer() == other.isInitializer() &&
                   this->isMethod() == other.isMethod() &&
                   this->isVarArgs() == other.isVarArgs();

        case Class:
            // TODO: how to handle the generics
            return getObjectClass()->name == other.getObjectClass()->name;
        case Closure:
            // TODO: Closure related
            return true;
    }

    return false;
}

Type* Type::returnValueType() const
{
    return std::get<FunctionType>(typeinfo).returnValueType;
}
std::vector<Type*> Type::parameterTypes() const
{
    return std::get<FunctionType>(typeinfo).paramTypes;
}

bool Type::isInitializer() const
{
    return std::get<FunctionType>(typeinfo).isInitializer;
}
bool Type::isMethod() const
{
    return std::get<FunctionType>(typeinfo).isMethod;
}
bool Type::isVarArgs() const
{
    return std::get<FunctionType>(typeinfo).isVarArgs;
}

// Array type, TODO complete below
uint Type::arrayDimension() const
{
    return std::get<ArrayType>(typeinfo).arrayDimension;
}

Type* Type::elementType() const
{
    return std::get<ArrayType>(typeinfo).elementType;
}

// Class type,  TODO complete below

// generate a unique name for a type
XString Type::typeName() const
{
    switch (category) {
        case Basic:
            return baseName;
        case Reference:
            return fmt::format("Ref<{}>", derefType()->typeName());
        case Class:
            return baseName;
        case Array:
            return elementType()->typeName() + "[]*" +
                   XString::fromInterger(arrayDimension());
        case Function:
            XString name = returnValueType()->typeName();
            name.append('(');
            for (Type* param : parameterTypes()) name.append(param->typeName());
            name.append(')');
            return name;
    }
    return "";
}

bool Type::isBasic() const { return category == Basic; }

BasicType Type::basicType() const { return std::get<BasicType>(typeinfo); }

bool Type::isInteger() const
{
    return category == Basic &&
           (basicType() == BasicType::I8 || basicType() == BasicType::UI8 ||
            basicType() == BasicType::I16 || basicType() == BasicType::I16 ||
            basicType() == BasicType::I32 || basicType() == BasicType::UI32 ||
            basicType() == BasicType::I64 || basicType() == BasicType::UI64);
}

bool Type::isSigned() const
{
    return category == Basic &&
           (basicType() == BasicType::I8 || basicType() == BasicType::I16 ||
            basicType() == BasicType::I32 || basicType() == BasicType::I64);
}

bool Type::isUnsigned() const
{
    return category == Basic &&
           (basicType() == BasicType::UI8 || basicType() == BasicType::UI16 ||
            basicType() == BasicType::UI32 || basicType() == BasicType::UI64);
}

bool Type::isNumber() const
{
    if (category == Type::Basic) {
        if (isInteger() || basicType() == BasicType::Float ||
            basicType() == BasicType::Double) {
            return true;
        }
    }
    return false;
}

bool Type::isObject() const
{
    return category == Reference && derefType()->category == Class;
}

bool Type::isObjectRef() const
{
    return category == Reference && derefType()->isObject();
}

XClass* Type::getObjectClass() const
{
    if (category == Reference && derefType()->category == Class) {
        // Easily get classDecl
        return derefType()->getObjectClass();
    } else if (category == Class) {
        return std::get<ClassType>(typeinfo).classDecl;
    } else {
        return nullptr;
    }
}

uint Type::size() const { return bits() / 8; }

uint Type::bits() const
{
    switch (category) {
        case Basic:
            switch (basicType()) {
                case BasicType::Void:
                case BasicType::Boolean:
                    return 1;
                case BasicType::I8:
                case BasicType::UI8:
                case BasicType::Char:
                    return 8;
                case BasicType::I16:
                case BasicType::UI16:
                    return 16;
                case BasicType::I32:
                case BasicType::UI32:
                case BasicType::Float:
                    return 32;
                case BasicType::I64:
                case BasicType::UI64:
                case BasicType::Double:
                    return 64;
                default:
                    return 0;
            }
            break;

        case Class: {
            uint bits = sizeof(uintptr_t) * 8;
            for (auto fieid : getObjectClass()->dataFields) {
                if (fieid.type->category != Function)
                    bits += fieid.type->bits();
            }
            return bits;
        }

        case Reference:
        case Array:
            return sizeof(uintptr_t) * 8;
    }
    return 0;
}
