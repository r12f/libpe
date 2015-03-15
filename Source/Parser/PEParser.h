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
    PEAddress GetRVAFromVA(PEAddress nVA);
    PEAddress GetVAFromRVA(PEAddress nRVA);
    PEAddress GetRVAFromFOA(PEAddress nFOA);
    PEAddress GetFOAFromRVA(PEAddress nRVA);
    PEAddress GetVAFromFOA(PEAddress nFOA);
    PEAddress GetFOAFromVA(PEAddress nVA);

    // Raw memory getter
    void * GetRawMemory(UINT64 nOffset, UINT64 nSize);

    // String parser
    const char * ParseAnsiString(PEAddress nRVA, PEAddress nFOA, UINT64 &nSize);
    const wchar_t * ParseUnicodeString(PEAddress nRVA, PEAddress nFOA, UINT64 &nSize);

    // Basic info related functions
    HRESULT ParseBasicInfo(IPEDosHeader **ppDosHeader, IPENtHeaders **ppNtHeaders, SectionHeaderList *pSectionHeaders, IPEOverlay **ppOverlay);
    HRESULT ParseSection(LibPERawSectionHeaderT(T) *pSectionHeader, IPESection **ppSection);

    // Export table related functions
    HRESULT ParseExportTable(IPEExportTable **ppExportTable);
    HRESULT ParseExportFunction(IPEExportTable *pExportTable, UINT32 nIndex, IPEExportFunction **ppFunction);

    // Import table related functions
    HRESULT ParseImportTable(IPEImportTable **ppImportTable);
    HRESULT ParseImportModule(PEAddress nImportDescRVA, PEAddress nImportDescFOA, LibPERawImportDescriptor(T) *pImportDescriptor, IPEImportModule **ppImportModule);
    HRESULT ParseImportFunctionsInModule(IPEImportModule *pImportModule);
    HRESULT ParseImportFunction(IPEImportFunction *pFunction);

    // Resource table related functions
    HRESULT ParseResourceTable(IPEResourceTable **ppResourceTable);
    HRESULT ParseResourceDirectory(PEAddress nRVA, PEAddress nFOA, IPEResourceDirectory **ppDirectory);
    HRESULT ParseResourceDirectoryEntry(IPEResourceDirectory *pDirectory, UINT32 nEntryIndex, IPEResourceDirectoryEntry **ppEntry);
    HRESULT ParseResourceDataEntry(PEAddress nRVA, PEAddress nFOA, IPEResourceDataEntry **ppDataEntry);
    HRESULT ParseResource(IPEResourceDataEntry *pDataEntry, IPEResource **ppResource);
    LibPERawResourceString(T) * ParseResourceString(PEAddress nRVA, PEAddress nFOA, UINT64 &nSize);
    LibPERawResourceStringU(T) * ParseResourceStringU(PEAddress nRVA, PEAddress nFOA, UINT64 &nSize);

    // Exception table related functions
    HRESULT ParseExceptionTable(IPEExceptionTable **ppExceptionTable);
    HRESULT ParseExceptionHandlerEntry(IPEExceptionTable *pExceptionTable, UINT32 nHandlerIndex, IPEExceptionHandlerEntry **ppExceptionHandlerEntry);
    HRESULT ParseExceptionHandler(IPEExceptionHandlerEntry *pExceptionHandlerEntry, IPEExceptionHandler **ppExceptionHandler);

    // Certificate table related functions
    HRESULT ParseCertificateTable(IPECertificateTable **ppCertificateTable);
    HRESULT ParseCertificates(IPECertificateTable *pCertificateTable);

    // Relocation table related functions
    HRESULT ParseRelocationTable(IPERelocationTable **ppRelocationTable);

    // Debug info table related functions
    HRESULT ParseDebugInfoTable(IPEDebugInfoTable **ppDebugInfoTable);

    // Global register table related functions
    HRESULT ParseGlobalPointerTable(IPEGlobalPointerTable **ppGlobalPointerTable);

    // Tls table related functions
    HRESULT ParseTlsTable(IPETlsTable **ppTlsTable);
    HRESULT ParseTlsCallbacks(IPETlsTable *pTlsTable);

    // Load config related functions
    HRESULT ParseLoadConfigTable(IPELoadConfigTable **ppLoadConfigTable);

    // Bound import table related functions
    HRESULT ParseBoundImportTable(IPEBoundImportTable **ppBoundImportTable);
    HRESULT ParseBoundImportModules(IPEBoundImportTable *pBoundImportTable);

    // Import Address table related functions
    HRESULT ParseImportAddressTable(IPEImportAddressTable **ppImportAddressTable);
    HRESULT ParseImportAddressTableContent(IPEImportAddressTable *pImportAddressTable);
    HRESULT ParseImportAddressBlock(LibPERawThunkData(T) *pRawBlock, PEAddress nBlockRVA, PEAddress nBlockFOA, IPEImportAddressBlock **ppBlock);
    HRESULT ParseImportAddressItem(LibPERawThunkData(T) *pRawItem, PEAddress nItemRVA, PEAddress nItemFOA, IPEImportAddressItem **ppItem);

    // Delay import table related functions
    HRESULT ParseDelayImportTable(IPEDelayImportTable **ppDelayImportTable);

    // Clr info table related functions
    HRESULT ParseClrTable(IPEClrTable **ppClrTable);

protected:
    virtual PEAddress GetRawOffsetFromAddressField(PEAddress nAddress) = 0;
    virtual PEAddress GetRVAFromAddressField(PEAddress nRVA) = 0;
    virtual PEAddress GetFOAFromAddressField(PEAddress nRVA) = 0;
    virtual PEAddress GetRawOffsetFromRVA(PEAddress nRVA) = 0;
    virtual PEAddress GetRawOffsetFromFOA(PEAddress nFOA) = 0;
    virtual PEAddress GetRVAFromRawOffset(PEAddress nRawOffset) = 0;
    virtual PEAddress GetFOAFromRawOffset(PEAddress nRawOffset) = 0;

    PEAddress GetRawOffset(PEAddress nRVA, PEAddress nFOA);
    HRESULT GetDataDirectoryEntry(INT32 nDataDirectoryEntryIndex, PEAddress &nRVA, PEAddress &nFOA, PEAddress &nSize);
    
    template <class TableClass>
    HRESULT ParseDataDirectory(INT32 nDataDirectoryEntryIndex, TableClass **ppTable)
    {
        LIBPE_CHK(NULL != ppTable, E_POINTER);
        LIBPE_CHK(NULL != m_pLoader && NULL != m_pFile, E_FAIL);

        *ppTable = NULL;

        PEAddress nTableRVA = LIBPE_INVALID_ADDRESS, nTableFOA = LIBPE_INVALID_ADDRESS, nTableSize = LIBPE_INVALID_SIZE;
        LIBPE_CHK_HR(GetDataDirectoryEntry(nDataDirectoryEntryIndex, nTableRVA, nTableFOA, nTableSize));

        LibPEPtr<TableClass> pTable = CreatePEElement<TableClass>(nTableRVA, nTableFOA, nTableSize);
        LIBPE_CHK(NULL != pTable, E_OUTOFMEMORY);

        *ppTable = pTable.Detach();

        return S_OK;
    }

    template <class ITable, class TableClass>
    HRESULT ParseDataDirectoryToInterface(INT32 nDataDirectoryEntryIndex, ITable **ppTable)
    {
        LIBPE_CHK(NULL != ppTable, E_POINTER);

        LibPEPtr<TableClass> pTable;
        LIBPE_CHK_HR(ParseDataDirectory<TableClass>(nDataDirectoryEntryIndex, &pTable));

        *ppTable = pTable.Detach();

        return S_OK;
    }

    template <class PEElementType>
    LibPEPtr<PEElementType> CreatePEElementEx(PEAddress nRVA, PEAddress nMemSize, PEAddress nFOA, PEAddress nFileSize)
    {
        LibPEPtr<PEElementType> pElement = NULL;

        HRESULT hr = S_OK;

        LIBPE_HR_TRY_BEGIN(hr)
        {
            pElement = new PEElementType();
            pElement->InnerSetBase(m_pFile, this);
            pElement->InnerSetMemoryInfo(nRVA, LIBPE_INVALID_ADDRESS, nMemSize);
            pElement->InnerSetFileInfo(nFOA, nFileSize);
        }
        LIBPE_HR_TRY_END();

        if (FAILED(hr)) {
            pElement = NULL;
        }

        return pElement;
    }

    template <class PEElementType>
    LibPEPtr<PEElementType> CreatePEElement(PEAddress nRVA, PEAddress nFOA, PEAddress nSize)
    {
        return CreatePEElementEx<PEElementType>(nRVA, nSize, nFOA, nSize);
    }

protected:
    LibPEPtr<DataLoader>    m_pLoader;
    PEFileT<T>              *m_pFile;
};

typedef PEParserT<PE32> PEParser32;
typedef PEParserT<PE64> PEParser64;

LIBPE_NAMESPACE_END