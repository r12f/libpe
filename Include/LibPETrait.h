#pragma once

#include "LibPEBase.h"

LIBPE_NAMESPACE_BEGIN

struct PE32 {};
struct PE64 {};

template <class T> struct PETrait {};

template <>
struct PETrait<PE32> {
    static const bool_t Is32Bit = true;
    typedef uint32_t                        Address;
    typedef IMAGE_DOS_HEADER                RawDosHeader;
    typedef IMAGE_NT_HEADERS32              RawNtHeaders;
    typedef IMAGE_FILE_HEADER               RawFileHeader;
    typedef IMAGE_OPTIONAL_HEADER32         RawOptionalHeader;
    typedef IMAGE_SECTION_HEADER            RawSectionHeader;
};

template <>
struct PETrait<PE64> {
    static const bool_t Is32Bit = false;
    typedef uint64_t                        Address;
    typedef IMAGE_DOS_HEADER                RawDosHeader;
    typedef IMAGE_NT_HEADERS64              RawNtHeaders;
    typedef IMAGE_FILE_HEADER               RawFileHeader;
    typedef IMAGE_OPTIONAL_HEADER64         RawOptionalHeader;
    typedef IMAGE_SECTION_HEADER            RawSectionHeader;
};

#define LibPEAddressT(T)                    typename PETrait<T>::Address
#define LibPERawDosHeaderT(T)               typename PETrait<T>::RawDosHeader
#define LibPERawFileHeaderT(T)              typename PETrait<T>::RawFileHeader
#define LibPERawNtHeadersT(T)               typename PETrait<T>::RawNtHeaders
#define LibPERawOptionalHeaderT(T)          typename PETrait<T>::RawOptionalHeader
#define LibPERawSectionHeaderT(T)           typename PETrait<T>::RawSectionHeader

typedef PETrait<PE32>::Address              PEAddress32;
typedef PETrait<PE32>::RawDosHeader         PERawDosHeader32;
typedef PETrait<PE32>::RawFileHeader        PERawFileHeader32;
typedef PETrait<PE32>::RawNtHeaders         PERawNtHeaders32;
typedef PETrait<PE32>::RawOptionalHeader    PERawOptionalHeader32;
typedef PETrait<PE32>::RawSectionHeader     PERawSectionHeader32;

typedef PETrait<PE64>::Address              PEAddress64;
typedef PETrait<PE64>::RawDosHeader         PERawDosHeader64;
typedef PETrait<PE64>::RawFileHeader        PERawFileHeader64;
typedef PETrait<PE64>::RawNtHeaders         PERawNtHeaders64;
typedef PETrait<PE64>::RawOptionalHeader    PERawOptionalHeader64;
typedef PETrait<PE64>::RawSectionHeader     PERawSectionHeader64;

LIBPE_NAMESPACE_END