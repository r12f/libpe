#pragma once

#include "LibPEBase.h"
#include "LibPETrait.h"

LIBPE_NAMESPACE_BEGIN

#ifdef LIBPE_WINOS
#define LIBPE_CALLTYPE      __stdcall
#else
#define LIBPE_CALLTYPE
#endif

#if defined(LIBPE_WINOS) && defined(LIBPE_DLL)
#ifdef LIBPEDLL_EXPORTS
#define LIBPE_API           __declspec(dllexport) LIBPE_CALLTYPE
#else
#define LIBPE_API           __declspec(dllimport) LIBPE_CALLTYPE
#endif
#else
#define LIBPE_API           LIBPE_CALLTYPE
#endif


class ILibPEInterface
{
public:
    virtual UINT32 LIBPE_CALLTYPE AddRef() = 0;
    virtual UINT32 LIBPE_CALLTYPE Release() = 0;
};

#define LIBPE_METHOD_(ret_type, f)      virtual ret_type LIBPE_CALLTYPE f
#define LIBPE_METHOD(f)                 LIBPE_METHOD_(HRESULT, f)

// PE file
class IPEFile;

// Base class for all the PE elements
class IPEElement;

// Header
class IPEDosHeader;
class IPENtHeaders;
class IPEFileHeader;
class IPEOptionalHeader;

// Sections
class IPESectionHeader;
class IPESection;
class IPEOverlay;

// DD0: Export table
class IPEExportTable;
class IPEExportFunction;

// DD1: Import table
class IPEImportTable;
class IPEImportModule;
class IPEImportFunction;

// DD2: Resource table
class IPEResourceTable;
class IPEResourceDirectory;
class IPEResourceDirectoryEntry;
class IPEResourceDataEntry;
class IPEResource;

// DD3: Exception table
class IPEExceptionTable;
class IPEExceptionHandlerEntry;
class IPEExceptionHandler;

// DD4: Security table
class IPECertificateTable;
class IPECertificate;

// DD5: Base relocation table
class IPERelocationTable;
class IPERelocationPage;
class IPERelocationItem;

// DD6: Debug info table
class IPEDebugInfoTable;

// DD7: Architecture Specific Data
class IPEArchitectureDataTable;

// DD8: Global pointer
class IPEGlobalPointerTable;

// DD9: TLS table
class IPETlsTable;

// DD10: Load config
class IPELoadConfigTable;

// DD11: Bound import table
class IPEBoundImportTable;

// DD12: IAT
class IPEImportAddressTable;
class IPEImportAddressBlock;
class IPEImportAddressItem;

// DD13: Delay import table
class IPEDelayImportTable;

// DD14: CLR table
class IPECLRHeader;

#define LIBPE_DEFINE_FIELD_ACCESSOR(FieldType, FuncName)                                    \
    virtual FieldType LIBPE_CALLTYPE GetField ## FuncName() = 0

#define LIBPE_DEFINE_ARRAY_FIELD_ACCESSOR(FieldType, FuncName)                              \
    virtual FieldType * LIBPE_CALLTYPE GetField ## FuncName ## Buffer()  = 0;               \
    virtual UINT32 LIBPE_CALLTYPE GetField ## FuncName ## ElementCount()  = 0;              \
    virtual FieldType LIBPE_CALLTYPE GetField ## FuncName(UINT32 nIndex)  = 0

class IPEFile : public ILibPEInterface
{
public:
    // Raw PE Header
    virtual PERawDosHeader * LIBPE_CALLTYPE GetRawDosHeader() = 0;
    virtual void * LIBPE_CALLTYPE GetRawNtHeaders() = 0;
    virtual PERawNtHeaders32 * LIBPE_CALLTYPE GetRawNtHeaders32() = 0;
    virtual PERawNtHeaders64 * LIBPE_CALLTYPE GetRawNtHeaders64() = 0;
    virtual PERawFileHeader * LIBPE_CALLTYPE GetRawFileHeader() = 0;
    virtual void * LIBPE_CALLTYPE GetRawOptionalHeader() = 0;
    virtual PERawOptionalHeader32 * LIBPE_CALLTYPE GetRawOptionalHeader32() = 0;
    virtual PERawOptionalHeader64 * LIBPE_CALLTYPE GetRawOptionalHeader64() = 0;

    // PE header
    virtual HRESULT LIBPE_CALLTYPE GetDosHeader(IPEDosHeader **ppDosHeader) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetNtHeaders(IPENtHeaders **ppNtHeaders) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetFileHeader(IPEFileHeader **ppFileHeader) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetOptionalHeader(IPEOptionalHeader **ppOptionalHeader) = 0;

    virtual BOOL LIBPE_CALLTYPE IsDosFile() = 0;
    virtual BOOL LIBPE_CALLTYPE Is32Bit() = 0;
    virtual PEAddress LIBPE_CALLTYPE GetImageBase() = 0;
    virtual UINT32 LIBPE_CALLTYPE GetImageSize() = 0;
    virtual UINT32 LIBPE_CALLTYPE GetEntryPoint() = 0;

    // Section & Extra data
    virtual UINT32 LIBPE_CALLTYPE GetSectionCount() = 0;
    virtual HRESULT LIBPE_CALLTYPE GetSectionHeader(UINT32 nIndex, IPESectionHeader **ppSectionHeader) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetSection(UINT32 nIndex, IPESection **ppSection) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetSectionByRVA(PEAddress nRVA, IPESection **ppSection) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetSectionByVA(PEAddress nVA, IPESection **ppSection) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetSectionByFOA(PEAddress nFOA, IPESection **ppSection) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetOverlay(IPEOverlay **ppOverlay) = 0;

    // PEAddress convert tools
    virtual PEAddress LIBPE_CALLTYPE GetRVAFromVA(PEAddress nVA) = 0;
    virtual PEAddress LIBPE_CALLTYPE GetVAFromRVA(PEAddress nRVA) = 0;
    virtual PEAddress LIBPE_CALLTYPE GetRVAFromFOA(PEAddress nFOA) = 0;
    virtual PEAddress LIBPE_CALLTYPE GetFOAFromRVA(PEAddress nRVA) = 0;
    virtual PEAddress LIBPE_CALLTYPE GetVAFromFOA(PEAddress nFOA) = 0;
    virtual PEAddress LIBPE_CALLTYPE GetFOAFromVA(PEAddress nVA) = 0;

    // Data directory entries
    virtual HRESULT LIBPE_CALLTYPE GetExportTable(IPEExportTable **ppExportTable) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetImportTable(IPEImportTable **ppImportTable) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetResourceTable(IPEResourceTable **ppResourceTable) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetExceptionTable(IPEExceptionTable **ppExceptionTable) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetCertificateTable(IPECertificateTable **ppCertificateTable) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetRelocationTable(IPERelocationTable **ppRelocationTable) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetDebugInfoTable(IPEDebugInfoTable **ppDebugInfoTable) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetArchitectureDataTable(IPEArchitectureDataTable **ppArchitectureDataTable) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetGlobalPointerTable(IPEGlobalPointerTable **ppGlobalPointerTable) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetTlsTable(IPETlsTable **ppTlsTable) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetLoadConfigTable(IPELoadConfigTable **ppLoadConfigTable) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetBoundImportTable(IPEBoundImportTable **ppBoundImportTable) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetImportAddressTable(IPEImportAddressTable **ppImportAddressTable) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetDelayImportTable(IPEDelayImportTable **ppDelayImportTable) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetCLRHeader(IPECLRHeader **ppCLRHeader) = 0;

    virtual HRESULT LIBPE_CALLTYPE RemoveExportTable() = 0;
    virtual HRESULT LIBPE_CALLTYPE RemoveImportTable() = 0;
    virtual HRESULT LIBPE_CALLTYPE RemoveResourceTable() = 0;
    virtual HRESULT LIBPE_CALLTYPE RemoveExceptionTable() = 0;
    virtual HRESULT LIBPE_CALLTYPE RemoveCertificateTable() = 0;
    virtual HRESULT LIBPE_CALLTYPE RemoveRelocationTable() = 0;
    virtual HRESULT LIBPE_CALLTYPE RemoveDebugInfoTable() = 0;
    virtual HRESULT LIBPE_CALLTYPE RemoveGlobalPointerTable() = 0;
    virtual HRESULT LIBPE_CALLTYPE RemoveTlsTable() = 0;
    virtual HRESULT LIBPE_CALLTYPE RemoveBoundImportTable() = 0;
    virtual HRESULT LIBPE_CALLTYPE RemoveImportAddressTable() = 0;
    virtual HRESULT LIBPE_CALLTYPE RemoveDelayImportTable() = 0;
    virtual HRESULT LIBPE_CALLTYPE RemoveCLRHeader() = 0;

    // PE Verification
    virtual BOOL LIBPE_CALLTYPE ValidatePEHeader() = 0;

    // Rebuild
    virtual HRESULT LIBPE_CALLTYPE Rebuild(const file_char_t *pFilePath) = 0;
};

class IPEElement : public ILibPEInterface
{
public:
    // Using raw memory directly may be dangerous. Because some related data may not be loaded.
    // We can just guarantee only this particular element is loaded.
    virtual void * LIBPE_CALLTYPE GetRawMemory() = 0;
    virtual PEAddress LIBPE_CALLTYPE GetRawOffset() = 0;
    virtual PEAddress LIBPE_CALLTYPE GetRawSize() = 0;

    virtual PEAddress LIBPE_CALLTYPE GetRVA() = 0;
    virtual PEAddress LIBPE_CALLTYPE GetVA() = 0;
    virtual PEAddress LIBPE_CALLTYPE GetSizeInMemory() = 0;

    virtual PEAddress LIBPE_CALLTYPE GetFOA() = 0;
    virtual PEAddress LIBPE_CALLTYPE GetSizeInFile() = 0;
};

class IPEDosHeader : public IPEElement
{
public:
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, Magic);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, Cblp);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, Cp);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, Crlc);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, Cparhdr);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, Minalloc);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, Maxalloc);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, Ss);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, Sp);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, Csum);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, Ip);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, Cs);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, Lfarlc);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, Ovno);
    LIBPE_DEFINE_ARRAY_FIELD_ACCESSOR(UINT16, Res);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, Oemid);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, Oeminfo);
    LIBPE_DEFINE_ARRAY_FIELD_ACCESSOR(UINT16, Res2);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, Lfanew);
};

class IPENtHeaders : public IPEElement
{
public:
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, Signature);
    virtual HRESULT LIBPE_CALLTYPE GetFileHeader(IPEFileHeader **ppFileHeader) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetOptionalHeader(IPEOptionalHeader **ppOptionalHeader) = 0;
};

class IPEFileHeader : public IPEElement
{
public:
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, Machine);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, NumberOfSections);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, TimeDateStamp);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, PointerToSymbolTable);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, NumberOfSymbols);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, SizeOfOptionalHeader);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, Characteristics);
};

class IPEOptionalHeader : public IPEElement
{
public:
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, Magic);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT8, MajorLinkerVersion);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT8, MinorLinkerVersion);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, SizeOfCode);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, SizeOfInitializedData);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, SizeOfUninitializedData);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, AddressOfEntryPoint);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, BaseOfCode);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT64, ImageBase);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, SectionAlignment);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, FileAlignment);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, MajorOperatingSystemVersion);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, MinorOperatingSystemVersion);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, MajorImageVersion);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, MinorImageVersion);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, MajorSubsystemVersion);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, MinorSubsystemVersion);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, Win32VersionValue);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, SizeOfImage);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, SizeOfHeaders);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, CheckSum);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, Subsystem);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, DllCharacteristics);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT64, SizeOfStackReserve);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT64, SizeOfStackCommit);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT64, SizeOfHeapReserve);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT64, SizeOfHeapCommit);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, LoaderFlags);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, NumberOfRvaAndSizes);
};

class IPESectionHeader : public IPEElement
{
public:
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, PhysicalAddress);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, VirtualSize);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, VirtualAddress);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, SizeOfRawData);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, PointerToRawData);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, PointerToRelocations);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, PointerToLinenumbers);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, NumberOfRelocations);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, NumberOfLinenumbers);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, Characteristics);

    virtual HRESULT LIBPE_CALLTYPE GetSection(IPESection **ppSection) = 0;
};

class IPESection : public IPEElement
{
public:
    virtual const char * LIBPE_CALLTYPE GetName() = 0;
    virtual HRESULT LIBPE_CALLTYPE GetRelocations() = 0;
    virtual HRESULT LIBPE_CALLTYPE GetLineNumbers() = 0;
    virtual UINT32 LIBPE_CALLTYPE GetCharacteristics() = 0;

    virtual HRESULT LIBPE_CALLTYPE SetName(const char *pName) = 0;
};

class IPEOverlay : public IPEElement
{
public:
};

class IPEExportTable : public IPEElement
{
public:
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, Characteristics);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, TimeDateStamp);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, MajorVersion);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, MinorVersion);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, Name);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, Base);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, NumberOfFunctions);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, NumberOfNames);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, AddressOfFunctions);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, AddressOfNames);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, AddressOfNameOrdinals);

    virtual UINT32 LIBPE_CALLTYPE GetFunctionCount() = 0;
    virtual HRESULT LIBPE_CALLTYPE GetFunctionByIndex(UINT32 nIndex, IPEExportFunction **ppFunction) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetFunctionByName(const char *pFunctionName, IPEExportFunction **ppFunction) = 0;
};

class IPEExportFunction: public IPEElement
{
public:
    virtual const char * LIBPE_CALLTYPE GetName() = 0;
    virtual UINT16 LIBPE_CALLTYPE GetOrdinal() = 0;
};

class IPEImportTable : public IPEElement
{
public:
    virtual UINT32 LIBPE_CALLTYPE GetModuleCount() = 0;
    virtual HRESULT LIBPE_CALLTYPE GetModuleByIndex(UINT32 nIndex, IPEImportModule **ppImportModule) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetModuleByName(const char *pModuleName, IPEImportModule **ppImportModule) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetFunctionByName(const char *pModuleName, const char *pFunctionName, IPEImportFunction **ppImportFunction) = 0;
};

class IPEImportModule: public IPEElement
{
public:
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, Characteristics);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, OriginalFirstThunk);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, TimeDateStamp);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, ForwarderChain);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, Name);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, FirstThunk);

    virtual BOOL LIBPE_CALLTYPE IsBound() = 0;
    virtual const char * LIBPE_CALLTYPE GetName() = 0;
    virtual UINT32 LIBPE_CALLTYPE GetFunctionCount() = 0;
    virtual HRESULT LIBPE_CALLTYPE GetFunctionByIndex(UINT32 nFunctionId, IPEImportFunction **ppFunction) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetFunctionByName(const char *pFunctionName, IPEImportFunction **ppFunction) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetRelatedImportAddressBlock(IPEImportAddressBlock **ppBlock) = 0;
};

class IPEImportFunction: public IPEElement
{
public:
    LIBPE_DEFINE_FIELD_ACCESSOR(PEAddress, ForwarderString);
    LIBPE_DEFINE_FIELD_ACCESSOR(PEAddress, Function);
    LIBPE_DEFINE_FIELD_ACCESSOR(PEAddress, Ordinal);
    LIBPE_DEFINE_FIELD_ACCESSOR(PEAddress, AddressOfData);

    virtual PERawImportByName * GetRawImportByName() = 0;
    virtual const char * GetName() = 0;
    virtual UINT16 GetOrdinal() = 0;
};

class IPEResourceTable : public IPEElement
{
public:
    virtual HRESULT LIBPE_CALLTYPE GetRootDirectory(IPEResourceDirectory **ppDirectory) = 0;
};

class IPEResourceDirectory : public IPEElement
{
public:
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, Characteristics);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, TimeDateStamp);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, MajorVersion);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, MinorVersion);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, NumberOfNamedEntries);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, NumberOfIdEntries);

    virtual UINT32 LIBPE_CALLTYPE GetEntryCount() = 0;
    virtual HRESULT LIBPE_CALLTYPE GetEntryByIndex(UINT32 nIndex, IPEResourceDirectoryEntry **ppEntry) = 0;
};

class IPEResourceDirectoryEntry : public IPEElement
{
public:
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, NameOffset);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, NameIsString);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, Name);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, Id);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, OffsetToData);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, OffsetToDirectory);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, DataIsDirectory);

    virtual BOOL LIBPE_CALLTYPE IsNameId() = 0;
    virtual UINT16 LIBPE_CALLTYPE GetId() = 0;

    virtual BOOL LIBPE_CALLTYPE IsNameString() = 0;
    virtual const wchar_t * LIBPE_CALLTYPE GetName() = 0;
    
    virtual BOOL LIBPE_CALLTYPE IsEntryDirectory() = 0;
    virtual HRESULT LIBPE_CALLTYPE GetDirectory(IPEResourceDirectory **ppDirectory) = 0;

    virtual BOOL LIBPE_CALLTYPE IsEntryDataEntry() = 0;
    virtual HRESULT LIBPE_CALLTYPE GetDataEntry(IPEResourceDataEntry **ppDataEntry) = 0;
};

class IPEResourceDataEntry : public IPEElement
{
public:
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, OffsetToData);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, Size);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, CodePage);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, Reserved);

    virtual HRESULT LIBPE_CALLTYPE GetResource(IPEResource **ppResource) = 0;
};

class IPEResource : public IPEElement
{
public:
};

class IPEExceptionTable : public IPEElement
{
public:
    virtual UINT32 LIBPE_CALLTYPE GetExceptionHandlerCount() = 0;
    virtual HRESULT LIBPE_CALLTYPE GetExceptionHandlerEntryByIndex(UINT32 nIndex, IPEExceptionHandlerEntry **ppExceptionHandlerEntry) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetExceptionHandlerByIndex(UINT32 nIndex, IPEExceptionHandler **ppExceptionHandler) = 0;
};

class IPEExceptionHandlerEntry : public IPEElement
{
public:
    virtual HRESULT LIBPE_CALLTYPE GetExceptionHandler(IPEExceptionHandler **ppExceptionHandler) = 0;
};

class IPEExceptionHandler : public IPEElement {};

class IPECertificateTable : public IPEElement
{
public:
    virtual UINT32 LIBPE_CALLTYPE GetCertificateCount() = 0;
    virtual HRESULT LIBPE_CALLTYPE GetCertificateByIndex(UINT32 nIndex, IPECertificate **ppCertificate) = 0;
};

class IPECertificate : public IPEElement
{
public:
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, Length);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, Revision);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, CertificateType);
    LIBPE_DEFINE_FIELD_ACCESSOR(BYTE *, Certificate);
};

class IPERelocationTable : public IPEElement
{
public:
    virtual UINT32 LIBPE_CALLTYPE GetPageCount() = 0;
    virtual HRESULT LIBPE_CALLTYPE GetPageByIndex(UINT32 nIndex, IPERelocationPage **ppRelocationPage) = 0;
    virtual BOOL LIBPE_CALLTYPE IsRVANeedRelocation(PEAddress nRVA) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetItemByRVA(PEAddress nRVA, IPERelocationItem **ppRelocationItem) = 0;
};

class IPERelocationPage : public IPEElement
{
public:
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, VirtualAddress);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, SizeOfBlock);

    virtual PEAddress LIBPE_CALLTYPE GetPageRVA() = 0;
    virtual UINT32 LIBPE_CALLTYPE GetItemCount() = 0;
    virtual HRESULT LIBPE_CALLTYPE GetItemByIndex(UINT32 nIndex, IPERelocationItem **ppRelocationItem) = 0;
    virtual BOOL LIBPE_CALLTYPE IsRVANeedRelocation(PEAddress nRVA) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetItemByRVA(PEAddress nRVA, IPERelocationItem **ppRelocationItem) = 0;
};

class IPERelocationItem : public IPEElement
{
public:
    virtual PEAddress LIBPE_CALLTYPE GetAddressRVA() = 0;
    virtual PEAddress LIBPE_CALLTYPE GetAddressContent() = 0;
    virtual PEAddress * LIBPE_CALLTYPE GetRawAddressContent() = 0;
};

class IPEDebugInfoTable : public IPEElement
{
public:
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, Characteristics);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, TimeDateStamp);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, MajorVersion);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, MinorVersion);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, Type);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, SizeOfData);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, AddressOfRawData);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, PointerToRawData);
};

class IPEArchitectureDataTable : public IPEElement {};

class IPEGlobalPointerTable : public IPEElement
{
public:
    LIBPE_DEFINE_FIELD_ACCESSOR(PEAddress, GlobalPointerRVA);

    virtual HRESULT LIBPE_CALLTYPE GetRelatedSection(IPESection **ppSection) = 0;
};

class IPETlsTable : public IPEElement
{
public:
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT64, StartAddressOfRawData);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT64, EndAddressOfRawData);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT64, AddressOfIndex);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT64, AddressOfCallBacks);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, SizeOfZeroFill);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, Characteristics);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, Reserved0);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, Alignment);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, Reserved1);

    virtual UINT32 LIBPE_CALLTYPE GetCallbackCount() = 0;
    virtual PEAddress LIBPE_CALLTYPE GetCallbackRVAByIndex(UINT32 nIndex) = 0;
};

class IPELoadConfigTable : public IPEElement
{
public:
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, Size);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, TimeDateStamp);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, MajorVersion);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, MinorVersion);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, GlobalFlagsClear);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, GlobalFlagsSet);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, CriticalSectionDefaultTimeout);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT64, DeCommitFreeBlockThreshold);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT64, DeCommitTotalFreeThreshold);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT64, LockPrefixTable);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT64, MaximumAllocationSize);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT64, VirtualMemoryThreshold);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT64, ProcessAffinityMask);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, ProcessHeapFlags);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, CSDVersion);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT16, Reserved1);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT64, EditList);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT64, SecurityCookie);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT64, SEHandlerTable);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT64, SEHandlerCount);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT64, GuardCFCheckFunctionPointer);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT64, Reserved2);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT64, GuardCFFunctionTable);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT64, GuardCFFunctionCount);
    LIBPE_DEFINE_FIELD_ACCESSOR(UINT32, GuardFlags);
};

class IPEBoundImportTable : public IPEElement {};

class IPEImportAddressTable : public IPEElement
{
public:
    virtual UINT32 LIBPE_CALLTYPE GetBlockCount() = 0;
    virtual HRESULT LIBPE_CALLTYPE GetBlockByIndex(UINT32 nIndex, IPEImportAddressBlock **ppBlock) = 0;
    virtual BOOL LIBPE_CALLTYPE IsBlockExists(IPEImportAddressBlock *pBlock) = 0;
    virtual BOOL LIBPE_CALLTYPE IsItemExist(IPEImportAddressItem *pItem) = 0;
};

class IPEImportAddressBlock : public IPEElement
{
public:
    virtual UINT32 LIBPE_CALLTYPE GetItemCount() = 0;
    virtual HRESULT LIBPE_CALLTYPE GetItemByIndex(UINT32 nIndex, IPEImportAddressItem **ppItem) = 0;
    virtual BOOL LIBPE_CALLTYPE IsItemExist(IPEImportAddressItem *pItem) = 0;
};

class IPEImportAddressItem : public IPEElement
{
public:
    LIBPE_DEFINE_FIELD_ACCESSOR(PEAddress, ForwarderString);
    LIBPE_DEFINE_FIELD_ACCESSOR(PEAddress, Function);
    LIBPE_DEFINE_FIELD_ACCESSOR(PEAddress, Ordinal);
    LIBPE_DEFINE_FIELD_ACCESSOR(PEAddress, AddressOfData);
};

class IPEDelayImportTable : public IPEElement {};

class IPECLRHeader : public IPEElement {};

LIBPE_NAMESPACE_END