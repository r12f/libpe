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
    virtual void * GetRawMemory(uint64_t nOffset, uint64_t nSize);

    virtual LibPEAddressT(T) GetRVAFromVA(LibPEAddressT(T) nVA);
    virtual LibPEAddressT(T) GetVAFromRVA(LibPEAddressT(T) nRVA);
    virtual LibPEAddressT(T) GetRVAFromFOA(LibPEAddressT(T) nFOA);
    virtual LibPEAddressT(T) GetFOAFromRVA(LibPEAddressT(T) nRVA);
    virtual LibPEAddressT(T) GetVAFromFOA(LibPEAddressT(T) nFOA);
    virtual LibPEAddressT(T) GetFOAFromVA(LibPEAddressT(T) nVA);

    virtual error_t ParseBasicInfo(LibPERawDosHeaderT(T) **ppDosHeader, LibPERawNtHeadersT(T) **ppNtHeaders, SectionHeaderList *pSectionHeaders, IPEOverlayT<T> **ppOverlay);
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
    virtual error_t ParseImportAddressTableContent(LibPERawThunkData(T) *pRawTable, IPEImportAddressTableT<T> *pImportAddressTable);
    virtual error_t ParseImportAddressBlock(LibPERawThunkData(T) *pRawBlock, LibPEAddressT(T) nBlockRVA, LibPEAddressT(T) nBlockFOA, IPEImportAddressBlockT<T> **ppBlock);
    virtual error_t ParseImportAddressItem(LibPERawThunkData(T) *pRawItem, LibPEAddressT(T) nItemRVA, LibPEAddressT(T) nItemFOA, IPEImportAddressItemT<T> **ppItem);
    virtual error_t ParseDelayImportTable(IPEDelayImportTableT<T> **ppDelayImportTable);
    virtual error_t ParseCLRHeader(IPECLRHeaderT<T> **ppCLRHeader);

protected:
    virtual LibPEAddressT(T) GetRVAFromAddressField(LibPEAddressT(T) nRVA) = 0;
    virtual LibPEAddressT(T) GetRawOffsetFromAddressField(LibPEAddressT(T) nAddress) = 0;

    error_t GetDataDirectoryEntry(int32_t nDataDirectoryEntryIndex, LibPEAddressT(T) &nRVA, LibPEAddressT(T) &nFOA, LibPEAddressT(T) &nSize)
    {
        LIBPE_ASSERT_RET(NULL != m_pFile, NULL);
        LibPERawOptionalHeaderT(T) *pOptionalHeader = m_pFile->GetOptionalHeader();
        if(NULL == pOptionalHeader) {
            return NULL;
        }

        LibPERawDataDirectoryT(T) *pDataDirectory = &(pOptionalHeader->DataDirectory[nDataDirectoryEntryIndex]);
        if(NULL == pDataDirectory || 0 == pDataDirectory->VirtualAddress || 0 == pDataDirectory->Size) {
            return ERR_FAIL;
        }

        nRVA = GetRVAFromAddressField(pDataDirectory->VirtualAddress);
        nSize = pDataDirectory->Size;
        nFOA = GetFOAFromRVA(nRVA);
        if(0 == nFOA) {
            return ERR_FAIL;
        }

        return ERR_OK;
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