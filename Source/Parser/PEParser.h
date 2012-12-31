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
    typedef std::vector<LibPEPtr<IPESectionHeader>> SectionHeaderList;

public:
    static LibPEPtr<PEParserT<T>> Create(PEParserType nType);

public:
    PEParserT() : m_pFile(NULL) {}
    virtual ~PEParserT() {}

    LIBPE_SINGLE_THREAD_OBJECT();

    virtual PEParserType GetType() = 0;

    void SetPEFile(PEFileT<T> *pFile) { m_pFile = pFile; }
    void SetDataLoader(DataLoader *pLoader) { m_pLoader = pLoader; }

    // Address converter
    virtual bool_t IsRawAddressVA() = 0;
    virtual PEAddress GetRVAFromVA(PEAddress nVA);
    virtual PEAddress GetVAFromRVA(PEAddress nRVA);
    virtual PEAddress GetRVAFromFOA(PEAddress nFOA);
    virtual PEAddress GetFOAFromRVA(PEAddress nRVA);
    virtual PEAddress GetVAFromFOA(PEAddress nFOA);
    virtual PEAddress GetFOAFromVA(PEAddress nVA);

    // Raw memory getter
    virtual void * GetRawMemory(uint64_t nOffset, uint64_t nSize);

    // String parser
    virtual const char * ParseAnsiString(PEAddress nRVA, PEAddress nFOA, uint64_t &nSize);
    virtual const wchar_t * ParseUnicodeString(PEAddress nRVA, PEAddress nFOA, uint64_t &nSize);

    // Basic info related functions
    virtual error_t ParseBasicInfo(LibPERawDosHeaderT(T) **ppDosHeader, void **ppNtHeaders, SectionHeaderList *pSectionHeaders, IPEOverlay **ppOverlay);
    virtual error_t ParseSection(LibPERawSectionHeaderT(T) *pSectionHeader, IPESection **ppSection);

    // Export table related functions
    virtual error_t ParseExportTable(IPEExportTable **ppExportTable);
    virtual error_t ParseExportFunction(IPEExportTable *pExportTable, uint32_t nIndex, IPEExportFunction **ppFunction);

    // Import table related functions
    virtual error_t ParseImportTable(IPEImportTable **ppImportTable);
    virtual error_t ParseImportModule(PEAddress nImportDescRVA, PEAddress nImportDescFOA, LibPERawImportDescriptor(T) *pImportDescriptor, IPEImportModule **ppImportModule);
    virtual error_t ParseImportFunction(LibPERawImportDescriptor(T) *pImportDescriptor, LibPERawThunkData(T) *pThunkData, IPEImportFunction **ppFunction);

    // Resource table related functions
    virtual error_t ParseResourceTable(IPEResourceTable **ppResourceTable);
    virtual error_t ParseResourceDirectory(PEAddress nRVA, PEAddress nFOA, IPEResourceDirectory **ppDirectory);
    virtual error_t ParseResourceDirectoryEntry(IPEResourceDirectory *pDirectory, uint32_t nEntryIndex, IPEResourceDirectoryEntry **ppEntry);
    virtual error_t ParseResourceDataEntry(PEAddress nRVA, PEAddress nFOA, IPEResourceDataEntry **ppDataEntry);
    virtual error_t ParseResource(IPEResourceDataEntry *pDataEntry, IPEResource **ppResource);
    virtual LibPERawResourceString(T) * ParseResourceString(PEAddress nRVA, PEAddress nFOA, uint64_t &nSize);
    virtual LibPERawResourceStringU(T) * ParseResourceStringU(PEAddress nRVA, PEAddress nFOA, uint64_t &nSize);

    virtual error_t ParseExceptionTable(IPEExceptionTable **ppExceptionTable);
    virtual error_t ParseCertificateTable(IPECertificateTable **ppCertificateTable);

    // Relocation table related functions.
    virtual error_t ParseRelocationTable(IPERelocationTable **ppRelocationTable);

    virtual error_t ParseDebugInfoTable(IPEDebugInfoTable **ppDebugInfoTable);
    virtual error_t ParseGlobalRegister(IPEGlobalRegister **ppGlobalRegister);
    virtual error_t ParseTlsTable(IPETlsTable **ppTlsTable);
    virtual error_t ParseBoundImportTable(IPEBoundImportTable **ppBoundImportTable);

    // Import Address table related functions.
    virtual error_t ParseImportAddressTable(IPEImportAddressTable **ppImportAddressTable);
    virtual error_t ParseImportAddressTableContent(IPEImportAddressTable *pImportAddressTable);
    virtual error_t ParseImportAddressBlock(LibPERawThunkData(T) *pRawBlock, PEAddress nBlockRVA, PEAddress nBlockFOA, IPEImportAddressBlock **ppBlock);
    virtual error_t ParseImportAddressItem(LibPERawThunkData(T) *pRawItem, PEAddress nItemRVA, PEAddress nItemFOA, IPEImportAddressItem **ppItem);

    virtual error_t ParseDelayImportTable(IPEDelayImportTable **ppDelayImportTable);
    virtual error_t ParseCLRHeader(IPECLRHeader **ppCLRHeader);

protected:
    virtual PEAddress GetRawOffsetFromAddressField(PEAddress nAddress) = 0;
    virtual PEAddress GetRVAFromAddressField(PEAddress nRVA) = 0;
    virtual PEAddress GetFOAFromAddressField(PEAddress nRVA) = 0;
    virtual PEAddress GetRawOffsetFromRVA(PEAddress nRVA) = 0;
    virtual PEAddress GetRawOffsetFromFOA(PEAddress nFOA) = 0;
    virtual PEAddress GetRVAFromRawOffset(PEAddress nRawOffset) = 0;
    virtual PEAddress GetFOAFromRawOffset(PEAddress nRawOffset) = 0;

    PEAddress GetRawOffset(PEAddress nRVA, PEAddress nFOA)
    {
        LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, NULL);
        return (0 != nRVA) ? GetRawOffsetFromRVA(nRVA) : GetRawOffsetFromFOA(nFOA);
    }

    error_t GetDataDirectoryEntry(int32_t nDataDirectoryEntryIndex, PEAddress &nRVA, PEAddress &nFOA, PEAddress &nSize)
    {
        LIBPE_ASSERT_RET(NULL != m_pFile, NULL);
        LibPERawOptionalHeaderT(T) *pOptionalHeader = (LibPERawOptionalHeaderT(T) *)m_pFile->GetOptionalHeader();
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
    LibPEPtr<DataLoader>    m_pLoader;
    PEFileT<T>              *m_pFile;
};

typedef PEParserT<PE32> PEParser32;
typedef PEParserT<PE64> PEParser64;

LIBPE_NAMESPACE_END