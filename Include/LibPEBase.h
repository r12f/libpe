#pragma once

// Macro List:
// LIBPE_POSIX
// LIBPE_NO_NAMESPACE
// LIBPE_DLL

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

// Basic types
#ifdef LIBPE_WINOS
#include <basetsd.h>
typedef wchar_t             file_char_t;
typedef std::wstring        file_t;
#else
typedef bool                BOOL;
typedef char                INT8;
typedef unsigned char       UINT8;
typedef short               INT16;
typedef unsigned short      UINT16;
typedef int                 INT32;
typedef unsigned            UINT32;
typedef __int64             INT64;
typedef unsigned __int64    UINT64;
typedef char                file_char_t;
typedef std::string         file_t;
#endif

#define LIBPE_UNUSED_PARAM(x)       (x)
