#pragma once

#include "LibPEBase.h"
#include "LibPETrait.h"

LIBPE_NAMESPACE_BEGIN

#define LIBPE_CALLTYPE      __stdcall

class ILibPEInterface
{
public:
    virtual uint32_t LIBPE_CALLTYPE AddRef() = 0;
    virtual uint32_t LIBPE_CALLTYPE Release() = 0;
};

#define LIBPE_METHOD_(ret_type, f)      virtual ret_type LIBPE_CALLTYPE f
#define LIBPE_METHOD(f)                 LIBPE_METHOD_(error_t, f)

template <class T> class IPEFileT;
template <class T> class IPEElementT;
template <class T> class IPESectionHeaderT;
template <class T> class IPESectionT;
template <class T> class IPEOverlayT;
template <class T> class IPEExportTableT;
template <class T> class IPEExportFunctionT;
template <class T> class IPEImportTableT;
template <class T> class IPEImportModuleT;
template <class T> class IPEImportFunctionT;
template <class T> class IPEResourceTableT;
template <class T> class IPEResourceTableItemT;
template <class T> class IPEExceptionTableT;
template <class T> class IPECertificateTableT;
template <class T> class IPERelocationTableT;
template <class T> class IPERelocationPageT;
template <class T> class IPERelocationItemT;
template <class T> class IPEDebugInfoTableT;
template <class T> class IPEGlobalRegisterT;
template <class T> class IPETlsTableT;
template <class T> class IPEBoundImportTableT;
template <class T> class IPEImportAddressTableT;
template <class T> class IPEImportAddressBlockT;
template <class T> class IPEImportAddressItemT;
template <class T> class IPEDelayImportTableT;
template <class T> class IPECLRHeaderT;

template <class T>
class IPEFileT : public ILibPEInterface
{
public:
    // Basic info
    virtual LibPERawDosHeaderT(T) * LIBPE_CALLTYPE GetDosHeader() = 0;
    virtual LibPERawNtHeadersT(T) * LIBPE_CALLTYPE GetNtHeaders() = 0;
    virtual LibPERawFileHeaderT(T) * LIBPE_CALLTYPE GetFileHeader() = 0;
    virtual LibPERawOptionalHeaderT(T) * LIBPE_CALLTYPE GetOptionalHeader() = 0;
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetImageBase() = 0;
    virtual uint32_t LIBPE_CALLTYPE GetImageSize() = 0;
    virtual uint32_t LIBPE_CALLTYPE GetEntryPoint() = 0;

    // Section & Extra data
    virtual uint32_t LIBPE_CALLTYPE GetSectionCount() = 0;
    virtual error_t LIBPE_CALLTYPE GetSectionHeader(uint32_t nIndex, IPESectionHeaderT<T> **ppSectionHeader) = 0;
    virtual error_t LIBPE_CALLTYPE GetSection(uint32_t nIndex, IPESectionT<T> **ppSection) = 0;
    virtual error_t LIBPE_CALLTYPE GetSectionByRVA(LibPEAddressT(T) nRVA, IPESectionT<T> **ppSection) = 0;
    virtual error_t LIBPE_CALLTYPE GetSectionByVA(LibPEAddressT(T) nVA, IPESectionT<T> **ppSection) = 0;
    virtual error_t LIBPE_CALLTYPE GetSectionByFOA(LibPEAddressT(T) nFOA, IPESectionT<T> **ppSection) = 0;
    virtual error_t LIBPE_CALLTYPE GetOverlay(IPEOverlayT<T> **ppOverlay) = 0;

    // PEAddress<T> convert tools
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetRVAFromVA(LibPEAddressT(T) nVA) = 0;
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetVAFromRVA(LibPEAddressT(T) nRVA) = 0;
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetRVAFromFOA(LibPEAddressT(T) nFOA) = 0;
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetFOAFromRVA(LibPEAddressT(T) nRVA) = 0;
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetVAFromFOA(LibPEAddressT(T) nFOA) = 0;
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetFOAFromVA(LibPEAddressT(T) nVA) = 0;

    // Data directory entries
    virtual error_t LIBPE_CALLTYPE GetExportTable(IPEExportTableT<T> **ppExportTable) = 0;
    virtual error_t LIBPE_CALLTYPE GetImportTable(IPEImportTableT<T> **ppImportTable) = 0;
    virtual error_t LIBPE_CALLTYPE GetResourceTable(IPEResourceTableT<T> **ppResourceTable) = 0;
    virtual error_t LIBPE_CALLTYPE GetExceptionTable(IPEExceptionTableT<T> **ppExceptionTable) = 0;
    virtual error_t LIBPE_CALLTYPE GetCertificateTable(IPECertificateTableT<T> **ppCertificateTable) = 0;
    virtual error_t LIBPE_CALLTYPE GetRelocationTable(IPERelocationTableT<T> **ppRelocationTable) = 0;
    virtual error_t LIBPE_CALLTYPE GetDebugInfoTable(IPEDebugInfoTableT<T> **ppDebugInfoTable) = 0;
    virtual error_t LIBPE_CALLTYPE GetGlobalRegister(IPEGlobalRegisterT<T> **ppGlobalRegister) = 0;
    virtual error_t LIBPE_CALLTYPE GetTlsTable(IPETlsTableT<T> **ppTlsTable) = 0;
    virtual error_t LIBPE_CALLTYPE GetBoundImportTable(IPEBoundImportTableT<T> **ppBoundImportTable) = 0;
    virtual error_t LIBPE_CALLTYPE GetImportAddressTable(IPEImportAddressTableT<T> **ppImportAddressTable) = 0;
    virtual error_t LIBPE_CALLTYPE GetDelayImportTable(IPEDelayImportTableT<T> **ppDelayImportTable) = 0;
    virtual error_t LIBPE_CALLTYPE GetCLRHeader(IPECLRHeaderT<T> **ppCLRHeader) = 0;

    virtual error_t LIBPE_CALLTYPE RemoveExportTable() = 0;
    virtual error_t LIBPE_CALLTYPE RemoveImportTable() = 0;
    virtual error_t LIBPE_CALLTYPE RemoveResourceTable() = 0;
    virtual error_t LIBPE_CALLTYPE RemoveExceptionTable() = 0;
    virtual error_t LIBPE_CALLTYPE RemoveCertificateTable() = 0;
    virtual error_t LIBPE_CALLTYPE RemoveRelocationTable() = 0;
    virtual error_t LIBPE_CALLTYPE RemoveDebugInfoTable() = 0;
    virtual error_t LIBPE_CALLTYPE RemoveGlobalRegister() = 0;
    virtual error_t LIBPE_CALLTYPE RemoveTlsTable() = 0;
    virtual error_t LIBPE_CALLTYPE RemoveBoundImportTable() = 0;
    virtual error_t LIBPE_CALLTYPE RemoveImportAddressTable() = 0;
    virtual error_t LIBPE_CALLTYPE RemoveDelayImportTable() = 0;
    virtual error_t LIBPE_CALLTYPE RemoveCLRHeader() = 0;

    // PE Verification
    virtual bool_t LIBPE_CALLTYPE ValidatePEHeader() = 0;

    // Rebuild
    virtual error_t LIBPE_CALLTYPE Rebuild(const file_char_t *pFilePath) = 0;
};

template <class T>
class IPEElementT : public ILibPEInterface
{
public:
    // Using raw memory directly may be dangerous. Because some related data may not be loaded.
    // We can just guarantee only this particular element is loaded.
    virtual void * LIBPE_CALLTYPE GetRawMemory() = 0;
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetRawOffset() = 0;
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetRawSize() = 0;

    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetRVA() = 0;
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetVA() = 0;
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetSizeInMemory() = 0;

    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetFOA() = 0;
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetSizeInFile() = 0;
};

template <class T>
class IPESectionHeaderT : public IPEElementT<T>
{
public:
    virtual LibPERawSectionHeaderT(T) * LIBPE_CALLTYPE GetRawStruct() = 0;
    virtual error_t LIBPE_CALLTYPE GetSection(IPESectionT<T> **ppSection) = 0;
};

template <class T>
class IPESectionT : public IPEElementT<T>
{
public:
    virtual void * LIBPE_CALLTYPE GetRawStruct() = 0;
    virtual const char * LIBPE_CALLTYPE GetName() = 0;
    virtual error_t LIBPE_CALLTYPE GetRelocations() = 0;
    virtual error_t LIBPE_CALLTYPE GetLineNumbers() = 0;
    virtual uint32_t LIBPE_CALLTYPE GetCharacteristics() = 0;

    virtual error_t LIBPE_CALLTYPE SetName(const char *pName) = 0;
};

template <class T>
class IPEOverlayT : public IPEElementT<T>
{
public:
    virtual void * LIBPE_CALLTYPE GetRawStruct() = 0;
};

template <class T>
class IPEExportTableT : public IPEElementT<T>
{
public:
    virtual LibPERawExportDirectory(T) * LIBPE_CALLTYPE GetRawStruct() = 0;
    virtual uint32_t LIBPE_CALLTYPE GetExportFunctionCount() = 0;
    virtual error_t LIBPE_CALLTYPE GetExportFunctionByIndex(uint32_t nIndex, IPEExportFunctionT<T> **ppFunction) = 0;
    virtual error_t LIBPE_CALLTYPE GetExportFunctionByName(const char *pFunctionName, IPEExportFunctionT<T> **ppFunction) = 0;
};

template <class T>
class IPEExportFunctionT: public IPEElementT<T>
{
public:
    virtual const char * LIBPE_CALLTYPE GetName() = 0;
    virtual uint16_t LIBPE_CALLTYPE GetHint() = 0;
};

template <class T>
class IPEImportTableT : public IPEElementT<T>
{
public:
    virtual LibPERawImportDescriptor(T) * LIBPE_CALLTYPE GetRawStruct() = 0;
    virtual uint32_t LIBPE_CALLTYPE GetImportModuleCount() = 0;
    virtual error_t LIBPE_CALLTYPE GetImportModuleByIndex(uint32_t nIndex, IPEImportModuleT<T> **ppImportModule) = 0;
    virtual error_t LIBPE_CALLTYPE GetImportModuleByName(const char *pModuleName, IPEImportModuleT<T> **ppImportModule) = 0;
    virtual error_t LIBPE_CALLTYPE GetImportFunctionByName(const char *pModuleName, const char *pFunctionName, IPEImportFunctionT<T> **ppImportFunction) = 0;
};

template <class T>
class IPEImportModuleT: public IPEElementT<T>
{
public:
    virtual LibPERawImportDescriptor(T) * LIBPE_CALLTYPE GetRawStruct() = 0;
    virtual bool_t LIBPE_CALLTYPE IsBound() = 0;
    virtual const char * LIBPE_CALLTYPE GetName() = 0;
    virtual uint32_t LIBPE_CALLTYPE GetImportFunctionCount() = 0;
    virtual error_t LIBPE_CALLTYPE GetImportFunctionByIndex(uint32_t nFunctionId, IPEImportFunctionT<T> **ppFunction) = 0;
    virtual error_t LIBPE_CALLTYPE GetImportFunctionByName(const char *pFunctionName, IPEImportFunctionT<T> **ppFunction) = 0;
    virtual error_t LIBPE_CALLTYPE GetRelatedImportAddressBlock(IPEImportAddressBlockT<T> **ppBlock) = 0;
};

template <class T>
class IPEImportFunctionT: public IPEElementT<T>
{
public:
    virtual LibPERawImportByName(T) * LIBPE_CALLTYPE GetRawStruct() = 0;
    virtual LibPERawThunkData(T) * LIBPE_CALLTYPE GetRawThunkData() = 0;
    virtual const char * LIBPE_CALLTYPE GetName() = 0;
    virtual uint16_t LIBPE_CALLTYPE GetHint() = 0;
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetEntry() = 0;
};

template <class T>
class IPEResourceTableT : public IPEElementT<T>
{
public:
    virtual error_t LIBPE_CALLTYPE AddItem(IPEResourceTableItemT<T> *pItem) = 0;
    virtual error_t LIBPE_CALLTYPE RemoveItem(int32_t nIndex) = 0;
    virtual error_t LIBPE_CALLTYPE GetItem(int32_t nIndex, IPEResourceTableItemT<T> **ppItem) = 0;
    virtual error_t LIBPE_CALLTYPE GetItemCount() = 0;
};

template <class T>
class IPEResourceTableItemT : public IPEElementT<T> {};

template <class T>
class IPEExceptionTableT : public IPEElementT<T> {};

template <class T>
class IPECertificateTableT : public IPEElementT<T> {};

template <class T>
class IPERelocationTableT : public IPEElementT<T>
{
public:
    virtual LibPERawBaseRelocation(T) * LIBPE_CALLTYPE GetRawStruct() = 0;
    virtual uint32_t LIBPE_CALLTYPE GetRelocationPageCount() = 0;
    virtual error_t LIBPE_CALLTYPE GetRelocationPageByIndex(uint32_t nIndex, IPERelocationPageT<T> **ppRelocationPage) = 0;
    virtual bool_t LIBPE_CALLTYPE IsRVANeedRelocation(LibPEAddressT(T) nRVA) = 0;
    virtual error_t LIBPE_CALLTYPE GetRelocationItemByRVA(LibPEAddressT(T) nRVA, IPERelocationItemT<T> **ppRelocationItem) = 0;
};

template <class T>
class IPERelocationPageT : public IPEElementT<T>
{
public:
    virtual LibPERawBaseRelocation(T) * LIBPE_CALLTYPE GetRawStruct() = 0;
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetPageRVA() = 0;
    virtual uint32_t LIBPE_CALLTYPE GetRelocationItemCount() = 0;
    virtual error_t LIBPE_CALLTYPE GetRelocationItemByIndex(uint32_t nIndex, IPERelocationItemT<T> **ppRelocationItem) = 0;
    virtual bool_t LIBPE_CALLTYPE IsRVANeedRelocation(LibPEAddressT(T) nRVA) = 0;
    virtual error_t LIBPE_CALLTYPE GetRelocationItemByRVA(LibPEAddressT(T) nRVA, IPERelocationItemT<T> **ppRelocationItem) = 0;
};

template <class T>
class IPERelocationItemT : public IPEElementT<T>
{
public:
    virtual void * LIBPE_CALLTYPE GetRawStruct() = 0;
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetAddressRVA() = 0;
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetAddressContent() = 0;
    virtual LibPEAddressT(T) * LIBPE_CALLTYPE GetRawAddressContent() = 0;
};

template <class T>
class IPEDebugInfoTableT : public IPEElementT<T> {};

template <class T>
class IPEGlobalRegisterT : public IPEElementT<T> {};

template <class T>
class IPETlsTableT : public IPEElementT<T> {};

template <class T>
class IPEBoundImportTableT : public IPEElementT<T> {};

template <class T>
class IPEImportAddressTableT : public IPEElementT<T>
{
public:
    virtual LibPERawThunkData(T) * LIBPE_CALLTYPE GetRawStruct() = 0;
    virtual uint32_t LIBPE_CALLTYPE GetImportAddressBlockCount() = 0;
    virtual error_t LIBPE_CALLTYPE GetImportAddressBlockByIndex(uint32_t nIndex, IPEImportAddressBlockT<T> **ppBlock) = 0;
    virtual bool_t LIBPE_CALLTYPE IsImportAddressBlockInTable(IPEImportAddressBlockT<T> *pBlock) = 0;
    virtual bool_t LIBPE_CALLTYPE IsImportAddressItemInTable(IPEImportAddressItemT<T> *pItem) = 0;
};

template <class T>
class IPEImportAddressBlockT : public IPEElementT<T>
{
public:
    virtual LibPERawThunkData(T) * LIBPE_CALLTYPE GetRawStruct() = 0;
    virtual uint32_t LIBPE_CALLTYPE GetImportAddressItemCount() = 0;
    virtual error_t LIBPE_CALLTYPE GetImportAddressItemByIndex(uint32_t nIndex, IPEImportAddressItemT<T> **ppItem) = 0;
    virtual bool_t LIBPE_CALLTYPE IsImportAddressItemInTable(IPEImportAddressItemT<T> *pItem) = 0;
};

template <class T>
class IPEImportAddressItemT : public IPEElementT<T>
{
public:
    virtual LibPERawThunkData(T) * LIBPE_CALLTYPE GetRawStruct() = 0;
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetRawAddress() = 0;
};

template <class T>
class IPEDelayImportTableT : public IPEElementT<T> {};

template <class T>
class IPECLRHeaderT : public IPEElementT<T> {};

LIBPE_NAMESPACE_END