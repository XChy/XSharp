#include "Lexer.h"
#include <cstdint>
#include "XSharp/Tokens.h"
#include "XSharp/XString.h"

using namespace XSharp;

Lexer::Lexer() {}

std::vector<Token> Lexer::tokenize(const XString& source)
{
    std::vector<Token> tokens;
    currentIter = source.begin();
    Token t;
    Span span{.row = 1, .col = 1};
    while ((t = fetchFrom(currentIter, span)).type != Eof) tokens.push_back(t);
    tokens.push_back({Eof, "Eof", span});
    return tokens;
}

Token Lexer::fetchFrom(XString::const_iterator& iter, Span& span)
{
    auto cur = [&iter]() { return *iter; };
    auto peek = [&iter]() { return *(iter + 1); };
    auto next = [&iter]() { return *(++iter); };

    while (cur().isSpace() || cur() == '\n') {
        if (cur() == '\n') {
            span.col = 1;
            span.row++;
        } else {
            span.col++;
        }
        iter++;
    }
    if (cur() == '\0') return {Eof};

    if (iter->isDigit()) {  // number
        XString value;

        if (*iter == '0') {
            if (*(iter + 1) == 'x' || peek() == 'X') {
                next();
                next();
                // needn't analyze '0x' part
                return Token(Integer, hex(iter, span), span);
            } else if (peek() == 'b' || peek() == 'B') {
                next();
                next();
                return Token(Integer, binary(iter, span), span);
            } else {
                return floatPoint(iter, span);
            }
        } else {
            return floatPoint(iter, span);
        }
    } else if (XSharp::isOp(cur())) {
        XString value;
        value.append(cur());
        next();
        while (XSharp::isOp(cur()) && XSharp::opContains(value + cur())) {
            value.append(cur());
            next();
        }
        return Token(Operator, value, span);
    } else if (cur().isLetter() || cur() == '_') {
        XString value;
        value.append(cur());
        next();
        while (cur().isLetterOrDigit() || cur() == '_') {
            value.append(cur());
            next();
        }
        if (value == "true")
            return Token(Boolean, value, span);
        else if (value == "false")
            return Token(Boolean, value, span);
        else if (value == "null")
            return Token(Null, value, span);
        else if (XSharp::isKeyword(value))
            return Token(Keyword, value, span);
        else
            return Token(Identifier, value, span);

    } else if (cur() == ';') {
        next();
        return Token(SentenceEnd, ";", span);
    } else if (cur() == '(') {
        next();
        return Token(OpenParen, "(", span);
    } else if (cur() == ')') {
        next();
        return Token(CloseParen, ")", span);
    } else if (cur() == '[') {
        next();
        return Token(OpenBracket, "[", span);
    } else if (cur() == ']') {
        next();
        return Token(CloseBracket, "]", span);
    } else if (cur() == '{') {
        next();
        return Token(OpenBrace, "{", span);
    } else if (cur() == '}') {
        next();
        return Token(CloseBrace, "}", span);
    } else if (cur() == ',') {
        next();
        return Token(Comma, ",", span);
    } else if (cur() == '\'') {
        next();
        XString value;

        while (cur() != '\'' && cur() != '\n' && cur() != '\0') {
            if (cur() == '\\') {
                next();
                value.append(transferMap[cur()]);
            } else {
                value.append(cur());
            }
            next();
        }

        if (cur() == '\'') {
            next();
        } else {
            // TODO: Error?
        }
        // TODO: check count of char
        return (Token(Char, value, span));
    } else if (cur() == '\"') {
        next();
        XString value;

        while (cur() != '\"') {
            if (cur() == '\\') {
                next();
                value.append(transferMap[cur()]);
            } else {
                value.append(cur());
            }
            next();
        }

        if (cur() == '\"') {
            next();
        } else {
            // TODO: Error?
        }

        return Token(String, value, span);
    } else if (cur() == '.') {
        next();
        return Token(Dot, ".", span);
    } else if (cur() == ':') {
        next();
        return Token(Colon, ":", span);
    } else if (cur() == '\n' || cur() == '\r') {
        next();
        span.row++;
        span.col = 1;
    } else if (cur().isSpace()) {
        next();
    } else {
        throw XSharpError(XString("Unknown char:").append(cur()));
    }
}

XString Lexer::hex(CharIter& iter, Span& span)
{
    auto cur = [&iter]() { return *iter; };
    auto peek = [&iter]() { return *(iter + 1); };
    auto next = [&iter]() { return *(++iter); };

    XString result;
    while (cur().isDigit() || (cur() >= 'a' && cur() <= 'f') ||
           (cur() >= 'A' && cur() <= 'F')) {
        result.append(cur());
        next();
    }
    return XString::fromInterger(result.toInteger<int64_t>(16), 10);
}

XString Lexer::binary(CharIter& iter, Span& span)
{
    auto cur = [&iter]() { return *iter; };
    auto peek = [&iter]() { return *(iter + 1); };
    auto next = [&iter]() { return *(++iter); };

    XString result;
    while (cur().isDigit()) {
        result.append(cur());
        next();
    }
    return XString::fromInterger(result.toInteger<int64_t>(2), 10);
}

Token Lexer::floatPoint(CharIter& iter, Span& span)
{
    auto cur = [&iter]() { return *iter; };
    auto peek = [&iter]() { return *(iter + 1); };
    auto next = [&iter]() { return *(++iter); };

    XString result;
    bool isDecimal = false;

    while (cur().isDigit() || cur() == '.') {
        if (cur() == '.') isDecimal = true;

        result.append(cur());
        next();
    }

    if (isDecimal)
        return Token(Decimal, result, span);
    else
        return Token(Integer, result, span);
}
