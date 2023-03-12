#pragma once

#include <memory>
#include <unordered_set>
#include "XString.h"

namespace XSharp {

static std::unordered_set<XString> keywords = {
    "for",    "while",  "break",   "continue",  "if",      "else",
    "return", "switch", "default", "case",      "throw",   "catch",
    "try",    "class",  "const",   "static",    "enum",    "false",
    "true",   "new",    "public",  "protected", "private",
};
bool isKeyword(const XString& keyword);

static std::unordered_set<XString> operators = {
    "+",  "-",  "*",  "/",  "^",  "&",  "<<", ">>", "<<=", ">>=", "%",
    "!",  "!=", "=",  "==", "+=", "-=", "*=", "/=", "%=",  "&=",  "|=",
    "^=", "++", "--", "<",  ">",  "<=", ">=", "&&", "||",  "?",   ":",

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
