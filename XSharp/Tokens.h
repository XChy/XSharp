#pragma once

#include <memory>
#include <unordered_set>
#include "XString.h"

namespace XSharp {

static std::unordered_set<XString> keywords = {
    "for",    "while",   "break", "continue", "if",    "else",   "return",
    "switch", "default", "case",  "throw",    "catch", "try",    "class",
    "const",  "static",  "enum",  "false",    "true",  "public", "private",
    "new",    "match",   "as",    "extends"};

bool isKeyword(const XString& keyword);

static std::unordered_set<XString> operators = {
    "+",  "-",  "*",  "/",  "^",  "&",  "<<", ">>", "<<=", ">>=", "%",
    "!",  "!=", "=",  "==", "+=", "-=", "*=", "/=", "%=",  "&=",  "|=",
    "^=", "++", "--", "<",  ">",  "<=", ">=", "&&", "||",  "?",   ":",
};

bool isOperator(const XString& oper);
bool isOp(XChar oper);

bool opContains(const XString& part);

}  // namespace XSharp

enum TokenType {
    Integer,
    Decimal,
    Boolean,
    Char,
    String,
    Null,
    Operator,
    Comma,
    Dot,
    Colon,
    OpenParen,
    CloseParen,
    OpenBracket,
    CloseBracket,
    OpenBrace,
    CloseBrace,
    SentenceEnd,
    Identifier,
    Keyword,
    Eof,
    LexerError
};

struct Span {
    XString filename;
    uint row;
    uint col;
    XString dump() const;
    XString wholeDump() const;
};

class XSharp_EXPORT Token
{
   public:
    Token() = default;
    Token(TokenType type);
    Token(TokenType type, const XString& value);
    Token(TokenType type, const XString& value, const Span& span);

    XString dump() const;

    bool is(TokenType type) const;
    bool isOperator(const XString& op) const;
    bool isKeyword(const XString& keyword) const;

    TokenType type;
    XString value;
    Span span;
};
