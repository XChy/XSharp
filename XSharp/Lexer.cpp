#include "Lexer.h"
#include <cstdint>
#include "XSharp/Tokens.h"
#include "XSharp/XString.h"

using namespace XSharp;

Lexer::Lexer() : row(1), col(1) {}

std::vector<Token> Lexer::tokenize(const XString &source)
{
    std::vector<Token> result;
    currentIter = source.begin();
    while (!isEof()) {
        if (cur().isDigit()) {  // number
            XString value;

            if (cur() == '0') {
                if (peek() == 'x' || peek() == 'X') {
                    next();
                    next();
                    // needn't analyze '0x' part
                    result.push_back(Token(Integer, hex(), currentSpan()));
                } else if (peek() == 'b' || peek() == 'B') {
                    next();
                    next();
                    result.push_back(Token(Integer, bin(), currentSpan()));
                } else {
                    result.push_back(dec());
                }
            } else {
                result.push_back(dec());
            }
        } else if (XSharp::isOperator(cur())) {
            XString value;
            value.append(cur());
            next();
            while (XSharp::isOperator(cur()) &&
                   XSharp::operatorContains(value + cur())) {
                value.append(cur());
                next();
            }
            result.push_back(Token(Operator, value, currentSpan()));
        } else if (cur().isLetter() || cur() == '_') {
            XString value;
            value.append(cur());
            next();
            while (cur().isLetterOrDigit() || cur() == '_') {
                value.append(cur());
                next();
            }
            if (value == "true")
                result.push_back(Token(Boolean, value, currentSpan()));
            else if (value == "false")
                result.push_back(Token(Boolean, value, currentSpan()));
            else if (value == "null")
                result.push_back(Token(Null, value, currentSpan()));
            else if (XSharp::isKeyword(value))
                result.push_back(Token(Keyword, value, currentSpan()));
            else
                result.push_back(Token(Identifier, value, currentSpan()));

        } else if (cur() == ';') {
            result.push_back(Token(SentenceEnd, ";", currentSpan()));
            next();
        } else if (cur() == '(') {
            result.push_back(Token(OpenParen, "(", currentSpan()));
            next();
        } else if (cur() == ')') {
            result.push_back(Token(CloseParen, ")", currentSpan()));
            next();
        } else if (cur() == '[') {
            result.push_back(Token(OpenBracket, "[", currentSpan()));
            next();
        } else if (cur() == ']') {
            result.push_back(Token(CloseBracket, "]", currentSpan()));
            next();
        } else if (cur() == '{') {
            result.push_back(Token(OpenBrace, "{", currentSpan()));
            next();
        } else if (cur() == '}') {
            result.push_back(Token(CloseBrace, "}", currentSpan()));
            next();
        } else if (cur() == ',') {
            result.push_back(Token(Comma, ",", currentSpan()));
            next();
        } else if (cur() == '\'') {
            next();
            XString value;

            while (cur() != '\'' && cur() != '\n' && !isEof()) {
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
            result.push_back(Token(Char, value, currentSpan()));
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

            result.push_back(Token(String, value, currentSpan()));
        } else if (cur() == '.') {
            next();
            result.push_back(Token(Dot, ".", currentSpan()));
        } else if (cur() == ':') {
            next();
            result.push_back(Token(Colon, ":", currentSpan()));
        } else if (cur() == '\n' || cur() == '\r') {
            next();
            row++;
            col = 1;
        } else if (cur().isSpace()) {
            next();
        } else {
            throw XSharpError(XString("Unknown char:").append(cur()));
        }
    }
    return result;
}

XString Lexer::hex()
{
    XString result;
    while (cur().isDigit() || (cur() >= 'a' && cur() <= 'f') ||
           (cur() >= 'A' && cur() <= 'F')) {
        result.append(cur());
        next();
    }
    return XString::fromInterger(result.toInteger<int64_t>(16), 10);
}

XString Lexer::bin()
{
    XString result;
    while (cur().isDigit()) {
        result.append(cur());
        next();
    }
    return XString::fromInterger(result.toInteger<int64_t>(2), 10);
}

Token Lexer::dec()
{
    XString result;
    bool isDecimal = false;

    while (cur().isDigit() || cur() == '.') {
        if (cur() == '.') isDecimal = true;

        result.append(cur());
        next();
    }

    if (isDecimal)
        return Token(Decimal, result, currentSpan());
    else
        return Token(Integer, result, currentSpan());
}

bool Lexer::next()
{
    currentIter++;
    col++;
    return *currentIter != '\0';
}

bool Lexer::isEof() { return *currentIter == '\0'; }

XChar Lexer::peek() const { return *(currentIter + 1); }

XChar Lexer::cur() const { return *currentIter; }

Span Lexer::currentSpan() const
{
    return Span{.filename = filename, .row = row, .col = col};
}
