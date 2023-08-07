#include "Lexer.h"
#include <cstdint>
#include "XSharp/Tokens.h"
#include "XSharp/XString.h"

using namespace XSharp;

Lexer::Lexer() {}

std::vector<Token> Lexer::tokenize(const XString& source)
{
    std::vector<Token> tokens;
    auto beginIter = source.begin();
    Token t;
    Span span{.row = 1, .col = 1};
    while ((t = fetchFrom(beginIter, span)).type != Eof) tokens.push_back(t);
    tokens.push_back({Eof, "Eof", span});
    return tokens;
}

Token Lexer::fetchFrom(XString::const_iterator& iter, Span& span)
{
    auto cur = [&]() { return *iter; };
    auto peek = [&]() { return *(iter + 1); };
    auto next = [&]() {
        span.col++;
        return *(++iter);
    };

    while (cur().isSpace() || cur() == '\n') {
        if (cur() == '\n') {
            span.row++;
        }
        next();
    }
    if (cur() == '\0') return {Eof};

    Span spanTemp = span;

    if (iter->isDigit()) {  // number
        XString value;

        if (*iter == '0') {
            if (*(iter + 1) == 'x' || peek() == 'X') {
                next();
                next();
                // needn't analyze '0x' part
                return Token(Integer, hex(iter, span), spanTemp);
            } else if (peek() == 'b' || peek() == 'B') {
                next();
                next();
                return Token(Integer, binary(iter, span), spanTemp);
            } else {
                bool isFP;
                XString result = floatPoint(iter, span, isFP);
                if (isFP)
                    return Token(FloatingPoint, result, spanTemp);
                else
                    return Token(Integer, result, spanTemp);
            }
        } else {
            bool isFP;
            XString result = floatPoint(iter, span, isFP);
            if (isFP)
                return Token(FloatingPoint, result, spanTemp);
            else
                return Token(Integer, result, spanTemp);
        }
    } else if (XSharp::isInOp(cur())) {
        XString value;
        value.append(cur());
        next();
        while (XSharp::isInOp(cur()) && XSharp::isParialOp(value + cur())) {
            value.append(cur());
            next();
        }
        return Token(Operator, value, spanTemp);
    } else if (cur().isLetter() || cur() == '_') {
        XString value;
        value.append(cur());
        next();
        while (cur().isLetterOrDigit() || cur() == '_') {
            value.append(cur());
            next();
        }
        if (value == "true")
            return Token(Boolean, value, spanTemp);
        else if (value == "false")
            return Token(Boolean, value, spanTemp);
        else if (value == "null")
            return Token(Null, value, spanTemp);
        else if (XSharp::isKeyword(value))
            return Token(Keyword, value, spanTemp);
        else
            return Token(Identifier, value, spanTemp);

    } else if (cur() == ';') {
        next();
        return Token(SentenceEnd, ";", spanTemp);
    } else if (cur() == '(') {
        next();
        return Token(OpenParen, "(", spanTemp);
    } else if (cur() == ')') {
        next();
        return Token(CloseParen, ")", spanTemp);
    } else if (cur() == '[') {
        next();
        return Token(OpenBracket, "[", spanTemp);
    } else if (cur() == ']') {
        next();
        return Token(CloseBracket, "]", spanTemp);
    } else if (cur() == '{') {
        next();
        return Token(OpenBrace, "{", spanTemp);
    } else if (cur() == '}') {
        next();
        return Token(CloseBrace, "}", spanTemp);
    } else if (cur() == ',') {
        next();
        return Token(Comma, ",", spanTemp);
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
        return (Token(Char, value, spanTemp));
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

        return Token(String, value, spanTemp);
    } else if (cur() == '.') {
        next();
        return Token(Dot, ".", spanTemp);
    } else if (cur() == ':') {
        next();
        return Token(Colon, ":", spanTemp);
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
    auto cur = [&]() { return *iter; };
    auto peek = [&]() { return *(iter + 1); };
    auto next = [&]() {
        span.col++;
        return *(++iter);
    };

    XString result;
    while (cur().isDigit()) {
        result.append(cur());
        next();
    }
    return XString::fromInterger(result.toInteger<int64_t>(2), 10);
}

XString Lexer::floatPoint(CharIter& iter, Span& span, bool& isFP)
{
    auto cur = [&]() { return *iter; };
    auto peek = [&]() { return *(iter + 1); };
    auto next = [&]() {
        span.col++;
        return *(++iter);
    };

    XString result;
    isFP = false;

    while (cur().isDigit() || cur() == '.') {
        if (cur() == '.') isFP = true;

        result.append(cur());
        next();
    }

    return result;
}
