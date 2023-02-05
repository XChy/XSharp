#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <stack>

typedef unsigned short ushort;
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ulonglong;

#ifdef _WIN32
#define DECL_EXPORT __declspec(dllexport)
#define DECL_IMPORT __declspec(dllimport)
#elif defined(QT_VISIBILITY_AVAILABLE)
#define DECL_EXPORT __attribute__((visibility("default")))
#define DECL_IMPORT __attribute__((visibility("default")))
#define DECL_HIDDEN __attribute__((visibility("hidden")))
#else
#define DECL_EXPORT
#define DECL_IMPORT
#define DECL_HIDDEN
#endif

#if defined(XSharp_SHARED_LIBRARY)
#define XSharp_EXPORT DECL_EXPORT
#elif defined(XSharp_STATIC_LIBRARY)
#define XSharp_EXPORT
#else
#define XSharp_EXPORT DECL_IMPORT
#endif
