#pragma once

#include "LibPEBase.h"

LIBPE_NAMESPACE_BEGIN

struct PE32 {};
struct PE64 {};

struct PETraitBase {
    typedef IMAGE_DOS_HEADER                RawDosHeader;
    typedef IMAGE_FILE_HEADER               RawFileHeader;
    typedef IMAGE_DATA_DIRECTORY            RawDataDirectory;
    typedef IMAGE_SECTION_HEADER            RawSectionHeader;
};

template <class T> struct PETrait {};

template <>
struct PETrait<PE32> : PETraitBase {
    static const bool_t Is32Bit = true;
    typedef uint32_t                        Address;
    typedef IMAGE_NT_HEADERS32              RawNtHeaders;
    typedef IMAGE_OPTIONAL_HEADER32         RawOptionalHeader;
};

template <>
struct PETrait<PE64> : PETraitBase {
    static const bool_t Is32Bit = false;
    typedef uint64_t                        Address;
    typedef IMAGE_NT_HEADERS64              RawNtHeaders;
    typedef IMAGE_OPTIONAL_HEADER64         RawOptionalHeader;
};

#define LibPEAddressT(T)                    typename PETrait<T>::Address
#define LibPERawDosHeaderT(T)               typename PETrait<T>::RawDosHeader
#define LibPERawFileHeaderT(T)              typename PETrait<T>::RawFileHeader
#define LibPERawNtHeadersT(T)               typename PETrait<T>::RawNtHeaders
#define LibPERawOptionalHeaderT(T)          typename PETrait<T>::RawOptionalHeader
#define LibPERawDataDirectoryT(T)           typename PETrait<T>::RawDataDirectory
#define LibPERawSectionHeaderT(T)           typename PETrait<T>::RawSectionHeader

typedef PETraitBase::RawDosHeader           PERawDosHeader;
typedef PETraitBase::RawFileHeader          PERawFileHeader;
typedef PETraitBase::RawDataDirectory       PERawDataDirectory;
typedef PETraitBase::RawSectionHeader       PERawSectionHeader;

typedef PETrait<PE32>::Address              PEAddress32;
typedef PETrait<PE32>::RawNtHeaders         PERawNtHeaders32;
typedef PETrait<PE32>::RawOptionalHeader    PERawOptionalHeader32;

typedef PETrait<PE64>::Address              PEAddress64;
typedef PETrait<PE64>::RawNtHeaders         PERawNtHeaders64;
typedef PETrait<PE64>::RawOptionalHeader    PERawOptionalHeader64;

LIBPE_NAMESPACE_END