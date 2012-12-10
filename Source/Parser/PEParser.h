#pragma once

#include "Parser/PEParserCommon.h"
#include "Parser/DataLoader.h"

LIBPE_NAMESPACE_BEGIN

template <class T> class PEFileT;

template <class T>
class PEParserT :
    public ILibPEInterface
{
public:
    typedef std::vector<LibPEPtr<IPESectionHeaderT<T>>> SectionHeaderList;

public:
    static PEParserT<T> * Create(PEParserType nType);
    static PEParserT<T> * CreateForDiskFile(const file_char_t *pFilePath, PEFileT<T> *pFile);
    static PEParserT<T> * CreateForMappedFile(void *pMemory, PEFileT<T> *pFile);
#ifdef LIBPE_WINOS
    static PEParserT<T> * CreateForMappedResource(HMODULE hModule, PEFileT<T> *pFile);
    static PEParserT<T> * CreateForLoadedModule(HMODULE hModule, PEFileT<T> *pFile);
#endif

public:
    PEParserT() : m_pFile(NULL) {}
    virtual ~PEParserT() {}

    LIBPE_SINGLE_THREAD_OBJECT();

    virtual PEParserType GetType() = 0;
    virtual bool_t IsRawAddressVA() = 0;
    virtual int8_t * GetRawMemory(uint64_t nOffset, uint64_t nSize);

    virtual LibPEAddressT(T) GetRVAFromVA(LibPEAddressT(T) nVA);
    virtual LibPEAddressT(T) GetVAFromRVA(LibPEAddressT(T) nRVA);
    virtual LibPEAddressT(T) GetRVAFromFOA(LibPEAddressT(T) nFOA);
    virtual LibPEAddressT(T) GetFOAFromRVA(LibPEAddressT(T) nRVA);
    virtual LibPEAddressT(T) GetVAFromFOA(LibPEAddressT(T) nFOA);
    virtual LibPEAddressT(T) GetFOAFromVA(LibPEAddressT(T) nVA);

    virtual error_t ParseBasicInfo(LibPERawDosHeaderT(T) **ppDosHeader, LibPERawNtHeadersT(T) **ppNtHeaders, SectionHeaderList *pSectionHeaders) = 0;
    virtual error_t ParseSection(LibPERawSectionHeaderT(T) *pSectionHeader, IPESectionT<T> **ppSection) = 0;
    virtual error_t ParseExportTable(IPEExportTableT<T> **ppExportTable) = 0;
    virtual error_t ParseExportFunction(IPEExportTableT<T> *pExportTable, uint32_t nIndex, IPEExportFunctionT<T> **ppFunction) = 0;
    virtual error_t ParseImportTable(IPEImportTableT<T> **ppImportTable) = 0;
    virtual error_t ParseImportModule(LibPEAddressT(T) nImportDescRVA, LibPEAddressT(T) nImportDescFOA, LibPERawImportDescriptor(T) *pImportDescriptor, IPEImportModuleT<T> **ppDll) = 0;
    virtual error_t ParseImportFunction(LibPERawImportDescriptor(T) *pImportDescriptor, LibPERawThunkData(T) *pThunkData, IPEImportFunctionT<T> **ppFunction) = 0;
    virtual error_t ParseResourceTable(IPEResourceTableT<T> **ppResourceTable) = 0;
    virtual error_t ParseExceptionTable(IPEExceptionTableT<T> **ppExceptionTable) = 0;
    virtual error_t ParseCertificateTable(IPECertificateTableT<T> **ppCertificateTable) = 0;
    virtual error_t ParseRelocationTable(IPERelocationTableT<T> **ppRelocationTable) = 0;
    virtual error_t ParseDebugInfoTable(IPEDebugInfoTableT<T> **ppDebugInfoTable) = 0;
    virtual error_t ParseGlobalRegister(IPEGlobalRegisterT<T> **ppGlobalRegister) = 0;
    virtual error_t ParseTlsTable(IPETlsTableT<T> **ppTlsTable) = 0;
    virtual error_t ParseBoundImportTable(IPEBoundImportTableT<T> **ppBoundImportTable) = 0;
    virtual error_t ParseImportAddressTable(IPEImportAddressTableT<T> **ppImportAddressTable) = 0;
    virtual error_t ParseDelayImportTable(IPEDelayImportTableT<T> **ppDelayImportTable) = 0;
    virtual error_t ParseCLRHeader(IPECLRHeaderT<T> **ppCLRHeader) = 0;

protected:
    virtual LibPEAddressT(T) GetAddressFromRVA(LibPEAddressT(T) nRVA) = 0;
    virtual LibPEAddressT(T) GetAddressFromVA(LibPEAddressT(T) nVA) = 0;
    virtual LibPEAddressT(T) GetAddressFromFOA(LibPEAddressT(T) nFOA) = 0;

    LibPERawDataDirectoryT(T) * GetDataDirectoryEntry(int32_t nDataDirectoryEntryIndex) 
    {
        LIBPE_ASSERT_RET(NULL != m_pFile, NULL);
        LibPERawOptionalHeaderT(T) *pOptionalHeader = m_pFile->GetOptionalHeader();
        if(NULL == pOptionalHeader) {
            return NULL;
        }
        return &(pOptionalHeader->DataDirectory[nDataDirectoryEntryIndex]);
    }

protected:
    void SetPEFile(PEFileT<T> *pFile) { m_pFile = pFile; }
    void SetDataLoader(DataLoader *pLoader) { m_pLoader = pLoader; }

protected:
    LibPEPtr<DataLoader>    m_pLoader;
    PEFileT<T>              *m_pFile;
};

typedef PEParserT<PE32> PEParser32;
typedef PEParserT<PE64> PEParser64;

LIBPE_NAMESPACE_END