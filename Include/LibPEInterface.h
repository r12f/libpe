#pragma once

#include "LibPEBase.h"
#include "LibPETrait.h"

LIBPE_NAMESPACE_BEGIN

#define LIBPE_CALLTYPE      __stdcall

class ILibPEInterface
{
public:
    virtual UINT32 LIBPE_CALLTYPE AddRef() = 0;
    virtual UINT32 LIBPE_CALLTYPE Release() = 0;
};

#define LIBPE_METHOD_(ret_type, f)      virtual ret_type LIBPE_CALLTYPE f
#define LIBPE_METHOD(f)                 LIBPE_METHOD_(HRESULT, f)

class IPEFile;
class IPEElement;
class IPESectionHeader;
class IPESection;
class IPEOverlay;
class IPEExportTable;
class IPEExportFunction;
class IPEImportTable;
class IPEImportModule;
class IPEImportFunction;
class IPEResourceTable;
class IPEResourceDirectory;
class IPEResourceDirectoryEntry;
class IPEResourceDataEntry;
class IPEResource;
class IPEExceptionTable;
class IPECertificateTable;
class IPERelocationTable;
class IPERelocationPage;
class IPERelocationItem;
class IPEDebugInfoTable;
class IPEGlobalRegister;
class IPETlsTable;
class IPEBoundImportTable;
class IPEImportAddressTable;
class IPEImportAddressBlock;
class IPEImportAddressItem;
class IPEDelayImportTable;
class IPECLRHeader;

class IPEFile : public ILibPEInterface
{
public:
    // Get raw PE header now. But it is not a final solution.
    // Dos header
    virtual PERawDosHeader * LIBPE_CALLTYPE GetDosHeader() = 0;
    virtual BOOL LIBPE_CALLTYPE IsDosFile() = 0;

    // PE Header
    virtual void * LIBPE_CALLTYPE GetNtHeaders() = 0;
    virtual PERawNtHeaders32 * LIBPE_CALLTYPE GetNtHeaders32() = 0;
    virtual PERawNtHeaders64 * LIBPE_CALLTYPE GetNtHeaders64() = 0;
    virtual PERawFileHeader * LIBPE_CALLTYPE GetFileHeader() = 0;
    virtual void * LIBPE_CALLTYPE GetOptionalHeader() = 0;
    virtual PERawOptionalHeader32 * LIBPE_CALLTYPE GetOptionalHeader32() = 0;
    virtual PERawOptionalHeader64 * LIBPE_CALLTYPE GetOptionalHeader64() = 0;
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

    // PEAddress<T> convert tools
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
    virtual HRESULT LIBPE_CALLTYPE GetGlobalRegister(IPEGlobalRegister **ppGlobalRegister) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetTlsTable(IPETlsTable **ppTlsTable) = 0;
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
    virtual HRESULT LIBPE_CALLTYPE RemoveGlobalRegister() = 0;
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

class IPESectionHeader : public IPEElement
{
public:
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
    virtual UINT32 LIBPE_CALLTYPE GetFunctionCount() = 0;
    virtual HRESULT LIBPE_CALLTYPE GetFunctionByIndex(UINT32 nIndex, IPEExportFunction **ppFunction) = 0;
    virtual HRESULT LIBPE_CALLTYPE GetFunctionByName(const char *pFunctionName, IPEExportFunction **ppFunction) = 0;
};

class IPEExportFunction: public IPEElement
{
public:
    virtual const char * LIBPE_CALLTYPE GetName() = 0;
    virtual UINT16 LIBPE_CALLTYPE GetHint() = 0;
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
    //virtual LibPERawThunkData(T) * LIBPE_CALLTYPE GetRawThunkData() = 0;
    virtual const char * LIBPE_CALLTYPE GetName() = 0;
    virtual UINT16 LIBPE_CALLTYPE GetHint() = 0;
    virtual PEAddress LIBPE_CALLTYPE GetEntry() = 0;
};

class IPEResourceTable : public IPEElement
{
public:
    virtual HRESULT LIBPE_CALLTYPE GetRootDirectory(IPEResourceDirectory **ppDirectory) = 0;
};

class IPEResourceDirectory : public IPEElement
{
public:
    virtual UINT32 LIBPE_CALLTYPE GetEntryCount() = 0;
    virtual HRESULT LIBPE_CALLTYPE GetEntryByIndex(UINT32 nIndex, IPEResourceDirectoryEntry **ppEntry) = 0;
};

class IPEResourceDirectoryEntry : public IPEElement
{
public:
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
    virtual HRESULT LIBPE_CALLTYPE GetResource(IPEResource **ppResource) = 0;
};

class IPEResource : public IPEElement
{
public:
};

class IPEExceptionTable : public IPEElement {};

class IPECertificateTable : public IPEElement {};

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

class IPEDebugInfoTable : public IPEElement {};

class IPEGlobalRegister : public IPEElement {};

class IPETlsTable : public IPEElement {};

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
    virtual PEAddress LIBPE_CALLTYPE GetRawAddress() = 0;
};

class IPEDelayImportTable : public IPEElement {};

class IPECLRHeader : public IPEElement {};

LIBPE_NAMESPACE_END