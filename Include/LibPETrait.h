#pragma once

#include "LibPEBase.h"

LIBPE_NAMESPACE_BEGIN

struct PE32 {};
struct PE64 {};

struct PETraitBase {
    typedef IMAGE_DOS_HEADER                    RawDosHeader;
    typedef IMAGE_FILE_HEADER                   RawFileHeader;
    typedef IMAGE_DATA_DIRECTORY                RawDataDirectory;
    typedef IMAGE_SECTION_HEADER                RawSectionHeader;
    typedef IMAGE_EXPORT_DIRECTORY              RawExportDirectory;
    typedef IMAGE_IMPORT_DESCRIPTOR             RawImportDescriptor;
    typedef IMAGE_IMPORT_BY_NAME                RawImportByName;
    typedef IMAGE_BASE_RELOCATION               RawBaseRelocation;
    typedef IMAGE_RESOURCE_DIRECTORY            RawResourceDirectory;
    typedef IMAGE_RESOURCE_DIRECTORY_ENTRY      RawResourceDirectoryEntry;
    typedef IMAGE_RESOURCE_DATA_ENTRY           RawResourceDataEntry;
    typedef IMAGE_RESOURCE_DIRECTORY_STRING     RawResourceString;
    typedef IMAGE_RESOURCE_DIR_STRING_U         RawResourceStringU;
    typedef WIN_CERTIFICATE                     RawWinCertificate;
};

template <class T> struct PETrait {};

template <>
struct PETrait<PE32> : PETraitBase {
    static const BOOL                           Is32Bit = true;
    typedef UINT32                              RawAddress;
    typedef IMAGE_NT_HEADERS32                  RawNtHeaders;
    typedef IMAGE_OPTIONAL_HEADER32             RawOptionalHeader;
    typedef IMAGE_THUNK_DATA32                  RawThunkData;
    static const UINT32                         ImageOrdinalFlag = 0x80000000;
    typedef IMAGE_LOAD_CONFIG_DIRECTORY32       RawLoadConfigDirectory;
};

template <>
struct PETrait<PE64> : PETraitBase {
    static const BOOL                           Is32Bit = false;
    typedef UINT64                              RawAddress;
    typedef IMAGE_NT_HEADERS64                  RawNtHeaders;
    typedef IMAGE_OPTIONAL_HEADER64             RawOptionalHeader;
    typedef IMAGE_THUNK_DATA64                  RawThunkData;
    static const UINT64                         ImageOrdinalFlag = 0x8000000000000000;
    typedef IMAGE_LOAD_CONFIG_DIRECTORY64       RawLoadConfigDirectory;
};

#define LibPERawAddressT(T)                     typename PETrait<T>::RawAddress
#define LibPERawDosHeaderT(T)                   typename PETrait<T>::RawDosHeader
#define LibPERawFileHeaderT(T)                  typename PETrait<T>::RawFileHeader
#define LibPERawNtHeadersT(T)                   typename PETrait<T>::RawNtHeaders
#define LibPERawOptionalHeaderT(T)              typename PETrait<T>::RawOptionalHeader
#define LibPERawDataDirectoryT(T)               typename PETrait<T>::RawDataDirectory
#define LibPERawSectionHeaderT(T)               typename PETrait<T>::RawSectionHeader
#define LibPERawExportDirectory(T)              typename PETrait<T>::RawExportDirectory
#define LibPERawImportDescriptor(T)             typename PETrait<T>::RawImportDescriptor
#define LibPERawThunkData(T)                    typename PETrait<T>::RawThunkData
#define LibPERawImportByName(T)                 typename PETrait<T>::RawImportByName
#define LibPERawBaseRelocation(T)               typename PETrait<T>::RawBaseRelocation
#define LibPERawResourceDirectory(T)            typename PETrait<T>::RawResourceDirectory
#define LibPERawResourceDirectoryEntry(T)       typename PETrait<T>::RawResourceDirectoryEntry
#define LibPERawResourceDataEntry(T)            typename PETrait<T>::RawResourceDataEntry
#define LibPERawResourceString(T)               typename PETrait<T>::RawResourceString
#define LibPERawResourceStringU(T)              typename PETrait<T>::RawResourceStringU
#define LibPERawLoadConfigDirectory(T)          typename PETrait<T>::RawLoadConfigDirectory
#define LibPERawWinCertificate(T)               typename PETrait<T>::RawWinCertificate

typedef UINT64                                  PEAddress;
#define LIBPE_INVALID_ADDRESS                   ((UINT64)(-1))
#define LIBPE_INVALID_SIZE                      0

typedef PETraitBase::RawDosHeader               PERawDosHeader;
typedef PETraitBase::RawFileHeader              PERawFileHeader;
typedef PETraitBase::RawDataDirectory           PERawDataDirectory;
typedef PETraitBase::RawSectionHeader           PERawSectionHeader;
typedef PETraitBase::RawExportDirectory         PERawExportDirectory;
typedef PETraitBase::RawImportDescriptor        PERawImportDescriptor;
typedef PETraitBase::RawImportByName            PERawImportByName;
typedef PETraitBase::RawBaseRelocation          PERawBaseRelocation;
typedef PETraitBase::RawResourceDirectory       PERawResourceDirectory;
typedef PETraitBase::RawResourceDirectoryEntry  PERawResourceDirectoryEntry;
typedef PETraitBase::RawResourceDataEntry       PERawResourceDataEntry;
typedef PETraitBase::RawResourceString          PERawResourceString;
typedef PETraitBase::RawResourceStringU         PERawResourceStringU;
typedef PETraitBase::RawWinCertificate          PERawWinCertificate;

typedef PETrait<PE32>::RawNtHeaders             PERawNtHeaders32;
typedef PETrait<PE32>::RawOptionalHeader        PERawOptionalHeader32;
typedef PETrait<PE32>::RawThunkData             PERawThunkData32;
typedef PETrait<PE32>::RawLoadConfigDirectory   PERawLoadConfigDirectory32;

typedef PETrait<PE64>::RawNtHeaders             PERawNtHeaders64;
typedef PETrait<PE64>::RawOptionalHeader        PERawOptionalHeader64;
typedef PETrait<PE64>::RawThunkData             PERawThunkData64;
typedef PETrait<PE64>::RawLoadConfigDirectory   PERawLoadConfigDirectory64;

LIBPE_NAMESPACE_END