#pragma once

#include "Parser/PEParser.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEParserDiskFileT :
    public PEParserT<T>
{
public:
    PEParserDiskFileT() {}
    virtual ~PEParserDiskFileT() {}

    virtual PEParserType GetType() { return PE_PARSER_TYPE_DISK_FILE; }
    virtual bool_t IsRawAddressVA() { return false; }
    virtual error_t ParseBasicInfo(LibPERawDosHeaderT(T) **ppDosHeader, LibPERawNtHeadersT(T) **ppNtHeaders, SectionHeaderList *pSectionHeaders);
    virtual error_t ParseSection(LibPERawSectionHeaderT(T) *pSectionHeader, IPESectionT<T> **ppSection);
    virtual error_t ParseExportTable(IPEExportTableT<T> **ppExportTable);
    virtual error_t ParseExportFunction(IPEExportTableT<T> *pExportTable, uint32_t nIndex, IPEExportFunctionT<T> **ppFunction);
    virtual error_t ParseImportTable(IPEImportTableT<T> **ppImportTable);
    virtual error_t ParseImportModule(LibPEAddressT(T) nImportDescRVA, LibPEAddressT(T) nImportDescFOA, LibPERawImportDescriptor(T) *pImportDescriptor, IPEImportModuleT<T> **ppImportModule);
    virtual error_t ParseImportFunction(LibPERawImportDescriptor(T) *pImportDescriptor, LibPERawThunkData(T) *pThunkData, IPEImportFunctionT<T> **ppFunction);
    virtual error_t ParseResourceTable(IPEResourceTableT<T> **ppResourceTable);
    virtual error_t ParseExceptionTable(IPEExceptionTableT<T> **ppExceptionTable);
    virtual error_t ParseCertificateTable(IPECertificateTableT<T> **ppCertificateTable);
    virtual error_t ParseRelocationTable(IPERelocationTableT<T> **ppRelocationTable);
    virtual error_t ParseDebugInfoTable(IPEDebugInfoTableT<T> **ppDebugInfoTable);
    virtual error_t ParseGlobalRegister(IPEGlobalRegisterT<T> **ppGlobalRegister);
    virtual error_t ParseTlsTable(IPETlsTableT<T> **ppTlsTable);
    virtual error_t ParseBoundImportTable(IPEBoundImportTableT<T> **ppBoundImportTable);
    virtual error_t ParseImportAddressTable(IPEImportAddressTableT<T> **ppImportAddressTable);
    virtual error_t ParseDelayImportTable(IPEDelayImportTableT<T> **ppDelayImportTable);
    virtual error_t ParseCLRHeader(IPECLRHeaderT<T> **ppCLRHeader);

protected:
    error_t GetDataDirectoryEntryInfo(int32_t nDataDirectoryEntryIndex, LibPEAddressT(T) &nRVA, LibPEAddressT(T) &nFOA, LibPEAddressT(T) &nSize)
    {
        LibPERawDataDirectoryT(T) *pDataDirectory = GetDataDirectoryEntry(nDataDirectoryEntryIndex);
        if(NULL == pDataDirectory || 0 == pDataDirectory->VirtualAddress || 0 == pDataDirectory->Size) {
            return ERR_FAIL;
        }

        nRVA = pDataDirectory->VirtualAddress;
        nFOA = GetFOAFromRVA(nRVA);
        if(0 == nFOA) {
            return ERR_FAIL;
        }

        return ERR_OK;
    }

protected:
    virtual LibPEAddressT(T) GetAddressFromRVA(LibPEAddressT(T) nRVA);
    virtual LibPEAddressT(T) GetAddressFromVA(LibPEAddressT(T) nVA);
    virtual LibPEAddressT(T) GetAddressFromFOA(LibPEAddressT(T) nFOA);
};
typedef PEParserDiskFileT<PE32> PEParserDiskFile32;
typedef PEParserDiskFileT<PE64> PEParserDiskFile64;

template <class T>
class PEParserMappedFileT :
    public PEParserT<T>
{
public:
};

#ifdef LIBPE_WINOS

template <class T>
class PEParserMappedResourceT :
    public PEParserT<T>
{
public:
};

template <class T>
class PEParserLoadedModuleT :
    public PEParserT<T>
{
public:
};

#endif


LIBPE_NAMESPACE_END