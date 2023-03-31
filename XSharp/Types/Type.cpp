#include "Type.h"
#include <cstdint>
#include <variant>
#include "XSharp/XString.h"
using namespace XSharp;

Type::Type() {}

Type::Type(const Type& other)
{
    typeID = other.typeID;
    baseName = other.baseName;
    category = other.category;
    isConst = other.isConst;
    typeinfo = other.typeinfo;
    // switch (category) {
    // case Basic:
    // typeinfo = std::get<BasicType>(other.typeinfo);
    // break;

    // case Array: {
    // typeinfo = std::get<ArrayType>(other.typeinfo);
    //} break;

    // case Function: {
    // FunctionType function;
    // function.returnValueType = new Type(*other.returnValueType());
    // for (Type* param : other.parameterTypes())
    // function.paramTypes.push_back(new Type(*param));
    // typeinfo = function;
    //} break;

    // case Class:
    // typeinfo = std::get<ClassType>(other.typeinfo);
    // break;

    // case Closure:
    //// TODO: Closure
    // break;
    //}
}

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

Type* Type::returnValueType() const
{
    return std::get<FunctionType>(typeinfo).returnValueType;
}
std::vector<Type*> Type::parameterTypes() const
{
    return std::get<FunctionType>(typeinfo).paramTypes;
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
        if (basicType() == BasicType::I32 || basicType() == BasicType::I64 ||
            basicType() == BasicType::UI32 || basicType() == BasicType::UI64 ||
            basicType() == BasicType::Float ||
            basicType() == BasicType::Double) {
            return true;
        }
    }
    return false;
}

bool Type::isObjectRef() const
{
    return category == Reference && derefType()->category == Class;
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
            uint size = sizeof(uintptr_t) * 4;
            for (auto fieid : getObjectClass()->dataFields) {
                if (fieid.type->category != Function)
                    size += fieid.type->size();
            }
            return size;
        }

        case Reference:
        case Array:
            return sizeof(uintptr_t) * 4;
    }
    return 0;
}
