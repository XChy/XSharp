#pragma once

#include "xsharp_global.h"
#include "SharedData.h"
#include <cstring>
#include <string>

class XSharp_EXPORT XChar
{
    friend class XString;

   public:
    XChar(char ucs) : ucs(uchar(ucs)) {}
    XChar(uchar ucs) : ucs(ucs) {}
    XChar(short ucs) : ucs(ushort(ucs)) {}
    XChar(ushort ucs) : ucs(ucs) {}
    XChar(int ucs4) : ucs(ushort(ucs4 | 0xFFFF)) {}
    XChar(uint ucs4) : ucs(ushort(ucs4 | 0xFFFF)) {}
    bool isDigit() const;
    bool isSpace() const;
    bool isLetter() const;
    bool isLetterOrDigit() const;

    char16_t value() const;

    int digitValue() const;
    XChar operator-(const XChar& other) const;
    bool operator==(XChar other) const;
    bool operator!=(XChar other) const;
    bool operator>(XChar other) const;
    bool operator<(XChar other) const;
    bool operator>=(XChar other) const;
    bool operator<=(XChar other) const;

   private:
    char16_t ucs;
};

class XSharp_EXPORT StringData
{
   public:
    StringData(const StringData& other);
    StringData(uint allocSize = 2);
    void allocate(uint allocSize);
    void reallocate(uint allocSize);
    ~StringData();
    char16_t* str;
    uint size;
    uint allocSize;
};

//  XString
//  Encoding:UCS2
//  Copy on write

enum Initialization { unInitialization };

class XSharp_EXPORT XString
{
   public:
    XString();
    XString(const XString& other);
    XString(const char* utf8);
    XString(const wchar_t* wstr);
    XString(const char16_t* ustr);
    XString(uint allocSize, Initialization init);

    const XChar* data() const;
    XChar* data();

    int size() const;

    XString& operator=(const char* utf8);
    XString& operator=(const wchar_t* wstr);
    XString& operator=(const char16_t* ustr);

    XString& append(const XString& other);
    XString& append(XChar xc);

    XString& reverse();

    XString operator+(const char* cstr) const;
    XString operator+(const wchar_t* wstr) const;
    XString operator+(const XString& other) const;
    XString operator+(const char16_t* ustr) const;
    XString operator+(XChar xc) const;

    XChar operator[](int index) const;
    XChar& operator[](int index);

    bool operator==(const XString& other) const;

    bool contains(XChar xc) const;

    // If 'sub' is not the substring of XString ,return -1
    int subStringIndex(const XString& sub) const;

    void removeAt(int pos);
    void remove(int pos, int len);
    void removeLast();

    std::string toStdString() const;
    std::u16string toUtf16String() const;

    static XString fromStdString(std::string str);
    const XChar* unicode() const;

    template <typename Interger>
    Interger toInteger(int base = 10) const
    {
        return std::stoll(this->toStdString(), 0, base);
    }
    int toInt(int base = 10) const;
    long long toLongLong(int base = 10);
    double toDouble() const;

    static XString fromAscii(const char* asciiStr);
    static XString fromUtf8(const char* utf8Str);

    template <typename Interger>
    static XString fromInterger(Interger v, int base = 10)
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
                result = std::to_string(v).data();
                break;
            case 16:
                if (isMinus) {
                    result.append('-');
                }
                while (v != 0) {
                    int base16 = v & 0xf;
                    if (base16 >= 0 && base16 <= 9) {
                        result.append(char(base16 + '0'));
                    } else {
                        result.append(char(base16 - 10 + 'a'));
                    }
                    v >>= 4;
                }
                break;
        }
        return result;
    }

    static XString number(int v, int base = 10);
    static XString number(long long v, int base = 10);
    static XString number(double v);

    typedef XChar* iterator;
    iterator begin();
    iterator end();

    typedef const XChar* const_iterator;
    const_iterator begin() const;
    const_iterator end() const;

    bool isDetach();
    void detach();

   private:
    XSharedData<StringData> d;
};

XString operator+(const char* utf8, const XString& xstr);

namespace std {

template <>
struct hash<XString> {
    size_t operator()(const XString& xstr) const
    {
        int base = 131;
        unsigned long long ans = 0;
        for (auto it = xstr.begin(); it != xstr.begin(); ++it)
            ans = ans * base + (unsigned long long)(*(ushort*)it);
        return ans;
    }
};

}  // namespace std
