#pragma once

// Macro List:
// LIBPE_POSIX
// LIBPE_NO_NAMESPACE
#ifdef WIN32
#define LIBPE_WINOS
#endif

#ifndef LIBPE_NO_NAMESPACE
#define LIBPE_NAMESPACE_BEGIN       namespace LibPE {
#define LIBPE_NAMESPACE_END         }
#else
#define LIBPE_NAMESPACE_BEGIN    
#define LIBPE_NAMESPACE_END
#endif

#include <string>

LIBPE_NAMESPACE_BEGIN

// Basic types
typedef char                bool_t;

#ifdef LIBPE_WINOS
typedef char                int8_t;
typedef unsigned char       uint8_t;
typedef short               int16_t;
typedef unsigned short      uint16_t;
typedef int                 int32_t;
typedef unsigned            uint32_t;
typedef __int64             int64_t;
typedef unsigned __int64    uint64_t;
typedef wchar_t             file_char_t;
typedef std::wstring        file_t;
#else
#include <inttype.h>
typedef char                file_char_t;
typedef std::string         file_t;
#endif

#define LIBPE_UNUSED_PARAM(x)       (x)

LIBPE_NAMESPACE_END
