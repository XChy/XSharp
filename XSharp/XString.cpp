#pragma once
#include "XString.h"

StringData::StringData(const StringData& other) {
	allocate(other.allocSize);
	size = other.size;
	str[size] = '\0';
	memcpy(str, other.str, other.size * sizeof(char16_t));
}

StringData::StringData(uint allocSize)
{
	allocate(allocSize);
	size = 0;
	str[0] = '\0';
}

void StringData::allocate(uint allocSize) {
	str = new char16_t[allocSize];
	this->allocSize = allocSize;
}

void StringData::reallocate(uint allocSize) {
	char16_t* newData = new char16_t[allocSize];
	this->allocSize = allocSize;
    memcpy(newData, str, (size+1) * sizeof(char16_t));
	delete[] str;
	str = newData;
}

StringData::~StringData() {
	delete[] str ;
}

bool XChar::isDigit() const
{
	return ucs >= '0' && ucs <= '9';
}

bool XChar::isSpace() const
{
	return ucs == ' ' || ucs == '\t';
}

bool XChar::isLetter() const
{
	return (ucs >= 'a' && ucs <= 'z') || (ucs >= 'A' && ucs <= 'Z') || (ucs >= 0x4e00 && ucs <= 0x9fa5);
}

bool XChar::isLetterOrDigit() const {
	return isLetter() || isDigit();
}

int XChar::digitValue() const
{
	return ucs - '0';
}

XChar XChar::operator-(const XChar& other) const
{
	return XChar(ucs - other.ucs);
}

bool XChar::operator==(XChar other) const
{
	return ucs == other.ucs;
}

bool XChar::operator!=(XChar other) const
{
	return ucs != other.ucs;
}

bool XChar::operator>(XChar other) const
{
	return ucs > other.ucs;
}

bool XChar::operator<(XChar other) const
{
	return ucs < other.ucs;
}

bool XChar::operator>=(XChar other)const
{
	return ucs >= other.ucs;
}

bool XChar::operator<=(XChar other)const
{
	return ucs <= other.ucs;
}


int ucs2len(const char16_t* str) {
	const char16_t* it = str;
	while (*it)++it;
	return it - str;
}

void xstrcpy(char16_t* dst, const char* src, int size)
{
	while (size--)
		*dst++ = *src++;
}

void xstrcpy(char16_t* dst, const wchar_t* src, int size)
{
	memcpy(dst, src, size * sizeof(char16_t));
}

void xstrcpy(char16_t* dst, const char16_t* src, int size)
{
	memcpy(dst, src, size * sizeof(char16_t));
}

XString::XString()
	:d(new StringData)
{}

XString::XString(const XString& other)
	: d(other.d)
{}

XString::XString(const char* utf8)
	: XString(XString::fromUtf8(utf8))
{
}

XString::XString(const wchar_t* wstr)
{
	int len = wcslen(wstr);
	d = new StringData(len + 1);
	d.data()->size = len;
	xstrcpy(d.data()->str, wstr, len + 1);
}

XString::XString(const char16_t* ustr)
{
	int len = ucs2len(ustr);
	d = new StringData(len + 1);
	d.data()->size = len;
	xstrcpy(d.data()->str, ustr, len + 1);
}

XString::XString(uint allocSize, Initialization init)
	:d(new StringData(allocSize))
{
}

const XChar* XString::data() const
{
	return (const XChar*)d.data()->str;
}

XChar* XString::data()
{
	return (XChar*)d.data()->str;
}

int XString::size() const
{
	return d.data()->size;
}

XString& XString::operator=(const char16_t* ustr) {
	int len = ucs2len(ustr);
	if (d.data()->allocSize < len + 1) {
		d = new StringData((len + 1)*2);
	}
	d.data()->size = len;
	xstrcpy(d.data()->str, ustr, len + 1);
	return *this;
}

XString& XString::operator=(const char* utf8) {
	*this = fromUtf8(utf8);
	return *this;
}

XString& XString::operator=(const wchar_t* wstr) {
	int len = wcslen(wstr);
	if (d.data()->allocSize < len + 1) {
		d = new StringData((len + 1) * 2);
	}
	d.data()->size = len;
	xstrcpy(d.data()->str, wstr, len + 1);
	return *this;
}

XString& XString::append(const XString& other) {
	int oldSize = d.data()->size;
	int otherSize = other.d.data()->size;
	int totalSize = d.data()->size + otherSize;

	if (d.ref->refCount() == 1) {
		if (d.data()->allocSize < totalSize + 1) {
			d.data()->reallocate((totalSize + 1) * 2);
		}
		xstrcpy(d.data()->str + d.data()->size, other.d.data()->str, otherSize + 1);
		d.data()->size = totalSize;
	}
	else {
		char16_t* oldData = d.data()->str;
		d.ref->unref();
		d.mData = new StringData((totalSize + 1) * 2);
		d.data()->size = totalSize;
		xstrcpy(d.data()->str, oldData, oldSize);
		xstrcpy(d.data()->str + oldSize, other.d.data()->str, otherSize + 1);
	}
	
	return *this;
}

XString& XString::append(XChar xc)
{
	int totalSize = d.data()->size+1;
	int oldSize = d.data()->size;
	if (d.ref->refCount() == 1) {
		if (d.data()->allocSize < totalSize + 1) {
			d.data()->reallocate((totalSize + 1) * 2);
		}
		d.data()->str[totalSize - 1] = xc.ucs;
		d.data()->str[totalSize] = '\0';
		d.data()->size = totalSize;
	}
	else {
		char16_t* oldData = d.data()->str;
		d.ref->unref();
		d.mData = new StringData((totalSize + 1) * 2);
		d.data()->size = totalSize;
		xstrcpy(d.data()->str, oldData, oldSize);
		d.data()->str[totalSize - 1] = xc.ucs;
		d.data()->str[totalSize] = '\0';
	}

	return *this;
}

XString& XString::reverse()
{
	std::reverse(begin(), end());
	return *this;
}

XString XString::operator+(const char* ascii) const
{
	return XString(*this).append(ascii);
}

XString XString::operator+(const wchar_t* wstr) const
{
	return XString(*this).append(wstr);
}

XString XString::operator+(const XString& other) const
{
	return XString(*this).append(other);
}

XString XString::operator+(const char16_t* ustr) const
{
	return XString(*this).append(ustr);
}

XString XString::operator+(XChar xc) const
{
	return XString(*this).append(xc);
}

XChar XString::operator[](int index) const
{
	return d.data()->str[index];
}

XChar& XString::operator[](int index)
{
	return data()[index];
}

bool XString::operator==(const XString& other) const
{
	if (d.ref == other.d.ref) {
		return true;
	}
	else if (size() == other.size()) {
		return memcmp(data(), other.data(), size()*2) == 0;
	}
	else {
		return false;
	}
}

bool XString::contains(XChar xc) const
{
	for (auto it = begin(); it != end(); ++it) {
		if (*it == xc) {
			return true;
		}
	}
	return false;
}

int XString::subStringIndex(const XString& sub) const
{
	if (this->size() < sub.size()) {
		return -1;
	}

	for (int i = 0; i <= this->size() - sub.size(); ++i) {
		if (memcmp(data(), sub.data(), sub.size() * 2) == 0) {
			return i;
		}
	}
	return -1;
}

void XString::removeAt(int pos) {
	if (!isDetach()) {
		detach();
	}
	xstrcpy(d.data()->str + pos, d.data()->str + pos + 1, d.data()->size - pos - 1);
	d.data()->str[--d.data()->size] = '\0';
}

void XString::remove(int pos, int len) {
	if (!isDetach()) {
		detach();
	}
	xstrcpy(d.data()->str + pos, d.data()->str + pos + len, d.data()->size - pos - len);
	d.data()->str[d.data()->size -= len] = '\0';
}

void XString::removeLast() {
	if (!isDetach()) {
		detach();
	}
	d.data()->str[--d.data()->size] = '\0';
}

std::string XString::toStdString() const
{
	std::string result;

	for (char16_t* p = (char16_t*)data(); p != (char16_t*)data() + size(); ++p) {
		result.push_back(*p);
	}
	return result;
}

std::u16string XString::toUtf16String() const
{
	std::u16string result;
	for (char16_t* p = (char16_t*)data(); p != (char16_t*)data() + size(); ++p) {
		result.push_back(*p);
	}
	return result;
}

XString XString::fromStdString(std::string str)
{
	return XString(str.data());
}

const XChar* XString::unicode() const
{
	return data();
}

template<typename Interger>
Interger XString::toInteger(int base) const
{
	Interger result = 0;
	switch (base) {
	case 2:
		for (auto it = begin(); it != end(); ++it) {
			result <<= 1;
			if (*it == '1') {
				result += 1;
			}
		}
		break;
	case 10:
		for (auto it = begin(); it != end(); ++it) {
			if (*it == 'e') {
				++it;
				bool isMinus = false;
				if (*it == '+') {
					++it;
				}
				else if (*it == '-')
				{
					++it;
					isMinus = true;
				}
				Interger ePart = 0;
				while (it != end()) {
					ePart *= 10;
					ePart += it->digitValue();
					++it;
				}
				if (isMinus) {
					while (ePart--) {
						result /= 10;
					}
				}
				else {
					while (ePart--) {
						result *= 10;
					}
				}
				break;
			}
			result *= 10;
			result += it->digitValue();
		}
		break;
	case 16:
		for (auto it = begin(); it != end(); ++it) {
			result <<= 4;
			if (it->isDigit()) {
				result += it->digitValue();
			}
			else if ((*it >= 'a' && *it <= 'f') || (*it >= 'A' && *it <= 'F')) {
				result += 10 + it->digitValue();
			}
		}
		break;
	}
	return result;
}

int XString::toInt(int base) const
{
	return toInteger<int>(base);
}

long long XString::toLongLong(int base)
{
	return toInteger<long long>(base);
}

double XString::toDouble() const
{
	double result = 0;
	for (auto it = begin(); it != end(); ++it) {
		if (*it == '.') {
			++it;
			double decMul = 1;
			while (it != end()) {
				if (*it == 'e') {
					goto E;
				}
				decMul *= 10;
				result += (double(it->digitValue()) / decMul);
				++it;
			}
			break;
		}
		else if (*it == 'e') {
		E:
			++it;
			bool isMinus = false;
			if (*it == '+') {
				++it;
			}
			else if (*it == '-')
			{
				++it;
				isMinus = true;
			}
			double ePart = 0;
			while (it != end()) {
				ePart *= 10;
				ePart += it->digitValue();
				++it;
			}
			if (isMinus) {
				while (ePart--) {
					result /= 10;
				}
			}
			else {
				while (ePart--) {
					result *= 10;
				}
			}
			break;
		}
		else {
			result *= 10;
			result += it->digitValue();
		}

	}

	return result;
}

XString XString::fromAscii(const char* asciiStr)
{
	int len = strlen(asciiStr);
	XString result(len + 1, unInitialization);
	result.d.data()->size = len;
	xstrcpy(result.d.data()->str, asciiStr, len + 1);
	return result;
}

XString XString::fromUtf8(const char* utf8Str)
{
	XString result;
	for (uchar* it = (uchar*)utf8Str; (*it) != '\0'; ++it) {
		if (*it < 192) {
			result.append(*it);
		}
		else if (*it < 224) {
			ushort ucs;
			uint utf8=0;
			uchar utf8_1 = (*it) ^ 0xc0;
			uchar utf8_2 = (*++it) ^ 0x80;
			utf8 += utf8_1;
			utf8 <<= 6;
			utf8 += utf8_2;
			ucs = utf8;
			result.append(ucs);
		}
		else if (*it < 240) {
			ushort ucs;
			uint utf8 = 0;
			uchar utf8_1 = (*it) ^ 0xe0;
			uchar utf8_2 = (*++it) ^ 0x80;
			uchar utf8_3 = (*++it) ^ 0x80;
			utf8 += utf8_1;
			utf8 <<= 6;
			utf8 += utf8_2;
			utf8 <<= 6;
			utf8 += utf8_3;
			ucs = utf8;
			result.append(ucs);
		}
	}
	return result;
}

template<typename Interger>
XString XString::fromInterger(Interger v, int base)
{
	XString result;
	bool isMinus = v < 0;
	v = llabs(v);
	switch (base) {
	case 2:
		if (isMinus) {
			result.append('-');
		}
		while (v != 0) {
			result.append((v & 1) ? '1' : '0');
			v >>= 1;
		}
		break;
	case 10:
		if (v == 0) {
			result.append('0');
			break;
		}
		while (v != 0) {
			result.append(XChar(char(v % 10 + '0')));
			v /= 10;
		}
		if (isMinus) {
			result.append('-');
		}
		result.reverse();
		break;
	case 16:
		if (isMinus) {
			result.append('-');
		}
		while (v != 0) {
			int base16 = v & 0xf;
			if (base16 >= 0 && base16 <= 9) {
				result.append(char(base16 + '0'));
			}
			else {
				result.append(char(base16 - 10 + 'a'));
			}
			v >>= 4;
		}
		break;
	}
	return result;
}

XString XString::number(int v, int base)
{
	return fromInterger<int>(v, base);
}

XString XString::number(long long v, int base)
{
	return fromInterger<long long>(v, base);
}

XString XString::number(double v)
{
	char buffer[256];
	snprintf(buffer, sizeof(buffer), "%f", v);
	return XString::fromAscii(buffer);
}

XString::iterator XString::begin() {
	return (iterator)d.data()->str;
}

XString::iterator XString::end() {
	return (iterator)(d.data()->str + d.data()->size);
}

XString::const_iterator XString::begin() const
{
	return (iterator)d.data()->str;
}

XString::const_iterator XString::end() const
{
	return (iterator)(d.data()->str + d.data()->size);
}

bool XString::isDetach() {
	return d.ref->refCount() == 1;
}

void XString::detach() {
	d.ref->unref();
	d = new StringData(*d.data());
}

XString operator+(const char* utf8, const XString& xstr)
{
	return XString(utf8).append(xstr);
}
