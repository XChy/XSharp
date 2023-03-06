#pragma once

#include <memory>
#include "XString.h"

namespace XSharp {

static std::unordered_map<XString, bool> keywords = {
    {"for", true},       {"while", true},   {"break", true},
    {"continue", true},  {"if", true},      {"else", true},
    {"return", true},    {"switch", true},  {"default", true},
    {"case", true},      {"throw", true},   {"catch", true},
    {"try", true},       {"class", true},   {"const", true},
    {"static", true},    {"enum", true},    {"false", true},
    {"true", true},      {"new", true},     {"public", true},
    {"protected", true}, {"private", true},
};
bool isKeyword(const XString& keyword);

static std::unordered_map<XString, bool> operators = {
    {"+", true},  {"-", true},  {"*", true},  {"/", true},   {"^", true},
    {"&", true},  {"<<", true}, {">>", true}, {"<<=", true}, {">>=", true},
    {"%", true},  {"!", true},  {"!=", true}, {"=", true},   {"==", true},
    {"+=", true}, {"-=", true}, {"*=", true}, {"/=", true},  {"%=", true},
    {"&=", true}, {"|=", true}, {"^=", true}, {"++", true},  {"--", true},
    {"<", true},  {">", true},  {"<=", true}, {">=", true},  {"?", true},
    {":", true},
};
bool isOperator(const XString& oper);
bool isOperator(XChar oper);

bool operatorContains(const XString& part);

}  // namespace XSharp

enum TokenType {
    Integer,
    DecimalFraction,
    Boolean,
    String,
    Null,
    Operator,
    Comma,
    Dot,
    Colon,
    OpenParenthesis,
    CloseParenthesis,
    OpenBracket,
    CloseBracket,
    OpenBrace,
    CloseBrace,
    SentenceEnd,
    Identifier,
    Keyword
};

class XSharp_EXPORT Token
{
   public:
    Token() = default;
    Token(TokenType type, const XString& value);

    XString dump() const;

    bool is(TokenType type) const;
    bool isOperator(const XString& op) const;
    bool isKeyword(const XString& keyword) const;

    TokenType type;
    XString value;
};
