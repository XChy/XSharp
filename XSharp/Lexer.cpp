#include "Lexer.h"

Lexer::Lexer()
{
}

std::vector<Token> Lexer::tokenize(const XString& source) const
{
    std::vector<Token> result;
	XString::const_iterator it = source.begin();
	while (it != source.end()) {
		if (it->isDigit()) {//number
			XString value;
			auto tokenBegin = it;

			if (*it == '0') {
				++it;
				if (*it == 'x' || *it == 'X') {
					++it;//needn't analyze '0x' part
					result.push_back(Token(Integer,hex(it)));
				}
				else if (*it == 'b' || *it == 'B') {
					++it;//needn't analyze '0b' part
					result.push_back(Token(Integer, bin(it)));
				}
				else {
					it = tokenBegin;
					result.push_back(dec(it));
				}
			}
			else {
				result.push_back(dec(it));
			}
		}
		else if (XSharp::isOperator(*it)) {
			XString value;
			value.append(*it); ++it;
			while (XSharp::isOperator(*it)&&XSharp::operatorContains(value+*it)) {
				value.append(*it);
				++it;
			}
			result.push_back(Token(Operator, value));
		}
		else if (it->isLetter() || *it == '_') {
			XString value;
			value.append(*it); ++it;;
			while (it->isLetterOrDigit() || *it == '_') {
				value.append(*it); ++it;;
			}
			if (value == "true") {
				result.push_back(Token(Boolean, value));
			}
			else if (value == "false") {
				result.push_back(Token(Boolean, value));
			}
			else if (value == "null") {
				result.push_back(Token(Null, value));
			}
			else if(XSharp::isKeyword(value))
			{
				result.push_back(Token(Keyword, value));
			}
			else {
				result.push_back(Token(Identifier, value));
			}
		}
		else if (*it == ';') {
			result.push_back(Token(SentenceEnd, ";")); ++it;
		}
		else if (*it == '(') {
			result.push_back(Token(OpenParenthesis,"(")); ++it;
		}
		else if (*it == ')') {
			result.push_back(Token(CloseParenthesis,")")); ++it;
		}
		else if (*it == '[') {
			result.push_back(Token(OpenBracket, "[")); ++it;
		}
		else if (*it == ']') {
			result.push_back(Token(CloseBracket, "]")); ++it;
		}
		else if (*it == '{') {
			result.push_back(Token(OpenBrace, "{")); ++it;
		}
		else if (*it == '}') {
			result.push_back(Token(CloseBrace, "}")); ++it;
		}
		else if (*it == ',') {
			result.push_back(Token(Comma,",")); ++it;;
		}
		else if (*it == '\"') {
			++it;
			XString value;
			while (*it != '\"') {
				value.append(*it); ++it;
			}
			++it;
			result.push_back(Token(String, value));
		}
		else if (*it == '.') {
			++it;
			result.push_back(Token(Dot,"."));
		}
		else if (it->isSpace()) {
			++it;
		}
		else {
			throw XSharpError(XString("Unknown char:").append(*it));
		}
	}
    return result;
}

XString Lexer::hex(XString::const_iterator& it) const
{
	XString result;
	while (it->isDigit()|| (*it >= 'a' && *it <= 'f') || (*it >= 'A' && *it <= 'F'))
	{
		result.append(*it);
		++it;
	}
	return XString::fromInterger(result.toInteger<long long>(16),10);
}

XString Lexer::bin(XString::const_iterator& it) const
{
	XString result;
	while (it->isDigit())
	{
		result.append(*it);
		++it;
	}
	return XString::fromInterger(result.toInteger<long long>(2), 10);
}

Token Lexer::dec(XString::const_iterator& it) const
{
	XString result;
	bool isDecimalFraction=false;

	while (it->isDigit()||*it=='.')
	{
		if (*it == '.') {
			isDecimalFraction = true;
		}
		result.append(*it);
		++it;
	}

	if (isDecimalFraction)
		return Token(DecimalFraction, result);
	else
		return Token(Integer, result);
}
