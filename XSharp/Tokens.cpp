#include "Tokens.h"
#include "XSharp/XString.h"
#include "fmt/core.h"

XString Span::dump() const { return fmt::format("{}:{}", row, col); }

XString Span::wholeDump() const
{
    return fmt::format("'{}' {}", filename, dump());
}

Token::Token(TokenType type, const XString &value) : type(type), value(value) {}

XString Token::dump() const
{
    XString result = span.dump();
    result.append(" ");
    switch (type) {
        case TokenType::Integer:
            result.append("Integer:");
            break;
        case TokenType::DecimalFraction:
            result.append("DecimalFraction:");
            break;
        case TokenType::Char:
            result.append("Char:");
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
        case TokenType::Colon:
            result.append("Colon:");
            break;
        case TokenType::OpenParen:
            result.append("OpenParenthesis:");
            break;
        case TokenType::CloseParen:
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

bool Token::is(TokenType type) const { return this->type == type; }

bool Token::isOperator(const XString &op) const
{
    return type == Operator && value == op;
}
bool Token::isKeyword(const XString &keyword) const
{
    return type == Keyword && value == keyword;
}

bool XSharp::isKeyword(const XString &keyword)
{
    if (keywords.find(keyword) == keywords.end())
        return false;
    else {
        return keywords.contains(keyword);
    }
}

bool XSharp::isOperator(const XString &oper)
{
    if (operators.find(oper) == operators.end())
        return false;
    else
        return operators.contains(oper);
}

bool XSharp::isOperator(XChar oper)
{
    for (auto operStr : operators) {
        if (operStr.contains(oper)) return true;
    }
    return false;
}

bool XSharp::operatorContains(const XString &part)
{
    for (auto operStr : operators) {
        if (operStr.subStringIndex(part) != -1) return true;
    }
    return false;
}
