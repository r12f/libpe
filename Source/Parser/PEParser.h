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
    virtual BOOL IsRawAddressVA() = 0;
    virtual PEAddress GetRVAFromVA(PEAddress nVA);
    virtual PEAddress GetVAFromRVA(PEAddress nRVA);
    virtual PEAddress GetRVAFromFOA(PEAddress nFOA);
    virtual PEAddress GetFOAFromRVA(PEAddress nRVA);
    virtual PEAddress GetVAFromFOA(PEAddress nFOA);
    virtual PEAddress GetFOAFromVA(PEAddress nVA);

    // Raw memory getter
    virtual void * GetRawMemory(UINT64 nOffset, UINT64 nSize);

    // String parser
    virtual const char * ParseAnsiString(PEAddress nRVA, PEAddress nFOA, UINT64 &nSize);
    virtual const wchar_t * ParseUnicodeString(PEAddress nRVA, PEAddress nFOA, UINT64 &nSize);

    // Basic info related functions
    virtual HRESULT ParseBasicInfo(IPEDosHeader **ppDosHeader, IPENtHeaders **ppNtHeaders, SectionHeaderList *pSectionHeaders, IPEOverlay **ppOverlay);
    virtual HRESULT ParseSection(LibPERawSectionHeaderT(T) *pSectionHeader, IPESection **ppSection);

    // Export table related functions
    virtual HRESULT ParseExportTable(IPEExportTable **ppExportTable);
    virtual HRESULT ParseExportFunction(IPEExportTable *pExportTable, UINT32 nIndex, IPEExportFunction **ppFunction);

    // Import table related functions
    virtual HRESULT ParseImportTable(IPEImportTable **ppImportTable);
    virtual HRESULT ParseImportModule(PEAddress nImportDescRVA, PEAddress nImportDescFOA, LibPERawImportDescriptor(T) *pImportDescriptor, IPEImportModule **ppImportModule);
    virtual HRESULT ParseImportFunctionsInModule(IPEImportModule *pImportModule);
    virtual HRESULT ParseImportFunction(IPEImportFunction *pFunction);

    // Resource table related functions
    virtual HRESULT ParseResourceTable(IPEResourceTable **ppResourceTable);
    virtual HRESULT ParseResourceDirectory(PEAddress nRVA, PEAddress nFOA, IPEResourceDirectory **ppDirectory);
    virtual HRESULT ParseResourceDirectoryEntry(IPEResourceDirectory *pDirectory, UINT32 nEntryIndex, IPEResourceDirectoryEntry **ppEntry);
    virtual HRESULT ParseResourceDataEntry(PEAddress nRVA, PEAddress nFOA, IPEResourceDataEntry **ppDataEntry);
    virtual HRESULT ParseResource(IPEResourceDataEntry *pDataEntry, IPEResource **ppResource);
    virtual LibPERawResourceString(T) * ParseResourceString(PEAddress nRVA, PEAddress nFOA, UINT64 &nSize);
    virtual LibPERawResourceStringU(T) * ParseResourceStringU(PEAddress nRVA, PEAddress nFOA, UINT64 &nSize);

    virtual HRESULT ParseExceptionTable(IPEExceptionTable **ppExceptionTable);
    virtual HRESULT ParseCertificateTable(IPECertificateTable **ppCertificateTable);

    // Relocation table related functions.
    virtual HRESULT ParseRelocationTable(IPERelocationTable **ppRelocationTable);

    virtual HRESULT ParseDebugInfoTable(IPEDebugInfoTable **ppDebugInfoTable);
    virtual HRESULT ParseGlobalRegister(IPEGlobalRegister **ppGlobalRegister);
    virtual HRESULT ParseTlsTable(IPETlsTable **ppTlsTable);
    virtual HRESULT ParseBoundImportTable(IPEBoundImportTable **ppBoundImportTable);

    // Import Address table related functions.
    virtual HRESULT ParseImportAddressTable(IPEImportAddressTable **ppImportAddressTable);
    virtual HRESULT ParseImportAddressTableContent(IPEImportAddressTable *pImportAddressTable);
    virtual HRESULT ParseImportAddressBlock(LibPERawThunkData(T) *pRawBlock, PEAddress nBlockRVA, PEAddress nBlockFOA, IPEImportAddressBlock **ppBlock);
    virtual HRESULT ParseImportAddressItem(LibPERawThunkData(T) *pRawItem, PEAddress nItemRVA, PEAddress nItemFOA, IPEImportAddressItem **ppItem);

    virtual HRESULT ParseDelayImportTable(IPEDelayImportTable **ppDelayImportTable);
    virtual HRESULT ParseCLRHeader(IPECLRHeader **ppCLRHeader);

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

    HRESULT GetDataDirectoryEntry(INT32 nDataDirectoryEntryIndex, PEAddress &nRVA, PEAddress &nFOA, PEAddress &nSize)
    {
        LIBPE_ASSERT_RET(NULL != m_pFile, NULL);
        LibPERawOptionalHeaderT(T) *pOptionalHeader = (LibPERawOptionalHeaderT(T) *)m_pFile->GetRawOptionalHeader();
        if(NULL == pOptionalHeader) {
            return NULL;
        }

        LibPERawDataDirectoryT(T) *pDataDirectory = &(pOptionalHeader->DataDirectory[nDataDirectoryEntryIndex]);
        if(NULL == pDataDirectory || 0 == pDataDirectory->VirtualAddress || 0 == pDataDirectory->Size) {
            return E_FAIL;
        }

        nRVA = GetRVAFromAddressField(pDataDirectory->VirtualAddress);
        nSize = pDataDirectory->Size;
        nFOA = GetFOAFromRVA(nRVA);
        if(0 == nFOA) {
            return E_FAIL;
        }

        return S_OK;
    }

protected:
    LibPEPtr<DataLoader>    m_pLoader;
    PEFileT<T>              *m_pFile;
};

typedef PEParserT<PE32> PEParser32;
typedef PEParserT<PE64> PEParser64;

LIBPE_NAMESPACE_END