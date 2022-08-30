#include "Tokens.h"

Token::Token(TokenType type, const XString& value):type(type),value(value)
{}

XString Token::dump() const
{
    XString result;
    switch (type)
    {
    case TokenType::Integer:
        result.append("Integer:");
        break;
    case TokenType::DecimalFraction:
        result.append("DecimalFraction:");
        break;
    case TokenType::String:
        result.append("String:");
        break;
    case TokenType::Boolean:
        result.append("Boolean:");
        break;
    case TokenType::Null:
        result.append("Null:");
        break;
    case TokenType::Operator:
        result.append("Operator:");
        break;
    case TokenType::Comma:
        result.append("Comma:");
        break;
    case TokenType::Dot:
        result.append("Dot:");
        break;
    case TokenType::OpenParenthesis:
        result.append("OpenParenthesis:");
        break;
    case TokenType::CloseParenthesis:
        result.append("CloseParenthesis:");
        break;
    case TokenType::OpenBracket:
        result.append("OpenBracket:");
        break;
    case TokenType::CloseBracket:
        result.append("CloseBracket:");
        break;
    case TokenType::OpenBrace:
        result.append("OpenBrace:");
        break;
    case TokenType::CloseBrace:
        result.append("CloseBrace:");
        break;
    case TokenType::SentenceEnd:
        result.append("SentenceEnd:");
        break;
    case TokenType::Identifier:
        result.append("Identifier:");
        break;
    case TokenType::Keyword:
        result.append("Keyword:");
        break;
    default:
        result.append("Unknown");
        break;
    }
    result.append(value);
    return result;
}

bool XSharp::isKeyword(const XString& keyword)
{
    if (keywords.find(keyword) == keywords.end())
        return false;
    else {
        return keywords[keyword];
    }
}

bool XSharp::isOperator(const XString& oper)
{
    if (operators.find(oper) == operators.end())
        return false;
    else
        return operators[oper];
}

bool XSharp::isOperator(XChar oper)
{
    for (const std::pair<XString, bool>& a : operators) {
        if (a.first.contains(oper))return true;
    }
    return false;
}

bool XSharp::operatorContains(const XString& part)
{
    for (const std::pair<XString, bool>& oper : operators) {
        if (oper.first.subStringIndex(part))
            return true;
    }
    return false;
}
