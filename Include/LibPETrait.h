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
    typedef IMAGE_EXPORT_DIRECTORY          RawExportDirectory;
    typedef IMAGE_IMPORT_DESCRIPTOR         RawImportDescriptor;
    typedef IMAGE_IMPORT_BY_NAME            RawImportByName;
};

template <class T> struct PETrait {};

template <>
struct PETrait<PE32> : PETraitBase {
    static const bool_t Is32Bit = true;
    typedef uint32_t                        Address;
    typedef IMAGE_NT_HEADERS32              RawNtHeaders;
    typedef IMAGE_OPTIONAL_HEADER32         RawOptionalHeader;
    typedef IMAGE_THUNK_DATA32              RawThunkData;
};

template <>
struct PETrait<PE64> : PETraitBase {
    static const bool_t Is32Bit = false;
    typedef uint64_t                        Address;
    typedef IMAGE_NT_HEADERS64              RawNtHeaders;
    typedef IMAGE_OPTIONAL_HEADER64         RawOptionalHeader;
    typedef IMAGE_THUNK_DATA64              RawThunkData;
};

#define LibPEAddressT(T)                    typename PETrait<T>::Address
#define LibPERawDosHeaderT(T)               typename PETrait<T>::RawDosHeader
#define LibPERawFileHeaderT(T)              typename PETrait<T>::RawFileHeader
#define LibPERawNtHeadersT(T)               typename PETrait<T>::RawNtHeaders
#define LibPERawOptionalHeaderT(T)          typename PETrait<T>::RawOptionalHeader
#define LibPERawDataDirectoryT(T)           typename PETrait<T>::RawDataDirectory
#define LibPERawSectionHeaderT(T)           typename PETrait<T>::RawSectionHeader
#define LibPERawExportDirectory(T)          typename PETrait<T>::RawExportDirectory
#define LibPERawImportDescriptor(T)         typename PETrait<T>::RawImportDescriptor
#define LibPERawThunkData(T)                typename PETrait<T>::RawThunkData
#define LibPERawImportByName(T)             typename PETrait<T>::RawImportByName

typedef PETraitBase::RawDosHeader           PERawDosHeader;
typedef PETraitBase::RawFileHeader          PERawFileHeader;
typedef PETraitBase::RawDataDirectory       PERawDataDirectory;
typedef PETraitBase::RawSectionHeader       PERawSectionHeader;
typedef PETraitBase::RawExportDirectory     PERawExportDirectory;
typedef PETraitBase::RawImportDescriptor    PERawImportDescriptor;
typedef PETraitBase::RawImportByName        PERawImportByName;

typedef PETrait<PE32>::Address              PEAddress32;
typedef PETrait<PE32>::RawNtHeaders         PERawNtHeaders32;
typedef PETrait<PE32>::RawOptionalHeader    PERawOptionalHeader32;
typedef PETrait<PE32>::RawThunkData         PERawThunkData32;

typedef PETrait<PE64>::Address              PEAddress64;
typedef PETrait<PE64>::RawNtHeaders         PERawNtHeaders64;
typedef PETrait<PE64>::RawOptionalHeader    PERawOptionalHeader64;
typedef PETrait<PE64>::RawThunkData         PERawThunkData64;

LIBPE_NAMESPACE_END