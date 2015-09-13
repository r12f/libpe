#pragma once

#include "Parser/PEParserCommon.h"
#include "Parser/DataLoader.h"

LIBPE_NAMESPACE_BEGIN

template <class T> class PEFileT;
template <class T> class PEElementT;

template <class T>
class PEParserT :
    public ILibPEInterface
{
    template <class T>
    class PEElementParsingScopeT
    {
    public:
        PEElementParsingScopeT(_In_ PEElementT<T> *pElement)
            : m_pElement(pElement)
        {}

        ~PEElementParsingScopeT()
        {
            m_pElement->OnParsingFinished();
        }

    private:
        PEElementT<T> *m_pElement;
    };

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
    PEAddress GetRVAFromVA(_In_ PEAddress nVA);
    PEAddress GetVAFromRVA(_In_ PEAddress nRVA);
    PEAddress GetRVAFromFOA(_In_ PEAddress nFOA);
    PEAddress GetFOAFromRVA(_In_ PEAddress nRVA);
    PEAddress GetVAFromFOA(_In_ PEAddress nFOA);
    PEAddress GetFOAFromVA(_In_ PEAddress nVA);

    // Raw memory getter
    void * GetRawMemory(_In_ UINT64 nOffset, _In_ UINT64 nSize);

    // String parser
    const char * ParseAnsiString(_In_ PEAddress nRVA, _In_ PEAddress nFOA, UINT64 &nSize);
    const wchar_t * ParseUnicodeString(_In_ PEAddress nRVA, _In_ PEAddress nFOA, UINT64 &nSize);

    // Basic info related functions
    HRESULT ParseBasicInfo(_Outptr_ IPEDosHeader **ppDosHeader, _Outptr_ IPENtHeaders **ppNtHeaders, _Out_ SectionHeaderList *pSectionHeaders, _Outptr_ IPEOverlay **ppOverlay);
    HRESULT ParseSection(_In_ LibPERawSectionHeaderT(T) *pSectionHeader, _Outptr_ IPESection **ppSection);

    // Export table related functions
    HRESULT ParseExportTable(_Outptr_ IPEExportTable **ppExportTable);
    HRESULT ParseExportFunction(_In_ IPEExportTable *pExportTable, UINT32 nIndex, _Outptr_ IPEExportFunction **ppFunction);

    // Import table related functions
    HRESULT ParseImportTable(_Outptr_ IPEImportTable **ppImportTable);
    HRESULT ParseAllImportModules(_In_ IPEImportTable *pImportTable);
    HRESULT ParseImportModule(_In_ PEAddress nImportDescRVA, _In_ PEAddress nImportDescFOA, LibPERawImportDescriptor(T) *pImportDescriptor, _Outptr_ IPEImportModule **ppImportModule);
    HRESULT ParseAllImportFunctions(_In_ IPEImportModule *pImportModule);
    HRESULT ParseImportFunction(_In_ PEAddress nThunkDataRVA, _In_ PEAddress nThunkDataFOA, _Outptr_ IPEImportFunction **ppImportFunction);
    HRESULT ParseImportFunction(_In_ IPEImportFunction *pFunction);

    // Resource table related functions
    HRESULT ParseResourceTable(_Outptr_ IPEResourceTable **ppResourceTable);
    HRESULT ParseResourceDirectory(_In_ PEAddress nRVA, _In_ PEAddress nFOA, _Outptr_ IPEResourceDirectory **ppDirectory);
    HRESULT ParseResourceDirectoryEntry(_In_ IPEResourceDirectory *pDirectory, UINT32 nEntryIndex, _Outptr_ IPEResourceDirectoryEntry **ppEntry);
    HRESULT ParseResourceDataEntry(_In_ PEAddress nRVA, _In_ PEAddress nFOA, _Outptr_ IPEResourceDataEntry **ppDataEntry);
    HRESULT ParseResource(_In_ IPEResourceDataEntry *pDataEntry, _Outptr_ IPEResource **ppResource);
    LibPERawResourceString(T) * ParseResourceString(_In_ PEAddress nRVA, _In_ PEAddress nFOA, UINT64 &nSize);
    LibPERawResourceStringU(T) * ParseResourceStringU(_In_ PEAddress nRVA, _In_ PEAddress nFOA, UINT64 &nSize);

    // Exception table related functions
    HRESULT ParseExceptionTable(_Outptr_ IPEExceptionTable **ppExceptionTable);
    HRESULT ParseExceptionHandlerEntry(_In_ IPEExceptionTable *pExceptionTable, _In_ UINT32 nHandlerIndex, _Outptr_ IPEExceptionHandlerEntry **ppExceptionHandlerEntry);
    HRESULT ParseExceptionHandler(_In_ IPEExceptionHandlerEntry *pExceptionHandlerEntry, _Outptr_ IPEExceptionHandler **ppExceptionHandler);

    // Certificate table related functions
    HRESULT ParseCertificateTable(_Outptr_ IPECertificateTable **ppCertificateTable);
    HRESULT ParseCertificates(_Inout_ IPECertificateTable *pCertificateTable);

    // Relocation table related functions
    HRESULT ParseRelocationTable(_Outptr_ IPERelocationTable **ppRelocationTable);

    // Debug info table related functions
    HRESULT ParseDebugInfoTable(_Outptr_ IPEDebugInfoTable **ppDebugInfoTable);

    // Global register table related functions
    HRESULT ParseGlobalPointerTable(_Outptr_ IPEGlobalPointerTable **ppGlobalPointerTable);

    // Tls table related functions
    HRESULT ParseTlsTable(_Outptr_ IPETlsTable **ppTlsTable);
    HRESULT ParseTlsCallbacks(_Inout_ IPETlsTable *pTlsTable);

    // Load config related functions
    HRESULT ParseLoadConfigTable(_Outptr_ IPELoadConfigTable **ppLoadConfigTable);

    // Bound import table related functions
    HRESULT ParseBoundImportTable(_Outptr_ IPEBoundImportTable **ppBoundImportTable);
    HRESULT ParseBoundImportModules(_In_ IPEBoundImportTable *pBoundImportTable);

    // Import Address table related functions
    HRESULT ParseImportAddressTable(_Outptr_ IPEImportAddressTable **ppImportAddressTable);
    HRESULT ParseImportAddressTableContent(_Inout_ IPEImportAddressTable *pImportAddressTable);
    HRESULT ParseImportAddressBlock(LibPERawThunkData(T) *pRawBlock, _In_ PEAddress nBlockRVA, _In_ PEAddress nBlockFOA, _Outptr_ IPEImportAddressBlock **ppBlock);
    HRESULT ParseImportAddressItem(LibPERawThunkData(T) *pRawItem, _In_ PEAddress nItemRVA, _In_ PEAddress nItemFOA, _Outptr_ IPEImportAddressItem **ppItem);

    // Delay import table related functions
    HRESULT ParseDelayImportTable(_Outptr_ IPEDelayImportTable **ppDelayImportTable);

    // Clr info table related functions
    HRESULT ParseClrTable(_Outptr_ IPEClrTable **ppClrTable);

protected:
    virtual PEAddress GetRawOffsetFromAddressField(_In_ PEAddress nAddress) = 0;
    virtual PEAddress GetRVAFromAddressField(_In_ PEAddress nRVA) = 0;
    virtual PEAddress GetFOAFromAddressField(_In_ PEAddress nRVA) = 0;
    virtual PEAddress GetRawOffsetFromRVA(_In_ PEAddress nRVA) = 0;
    virtual PEAddress GetRawOffsetFromFOA(_In_ PEAddress nFOA) = 0;
    virtual PEAddress GetRVAFromRawOffset(_In_ PEAddress nRawOffset) = 0;
    virtual PEAddress GetFOAFromRawOffset(_In_ PEAddress nRawOffset) = 0;

    PEAddress GetRawOffset(_In_ PEAddress nRVA, _In_ PEAddress nFOA);

    void InitPEElement(_Inout_ PEElementT<T> *pElement, _In_ PEAddress nRVA, _In_ PEAddress nFOA, _In_ PEAddress nSize);
    void InitPEElement(_Inout_ PEElementT<T> *pElement, _In_ PEAddress nRVA, _In_ PEAddress nMemSize, _In_ PEAddress nFOA, _In_ PEAddress nFileSize);

    HRESULT GetDataDirectoryEntry(_In_ INT32 nDataDirectoryEntryIndex, _In_ PEAddress &nRVA, _In_ PEAddress &nFOA, _In_ PEAddress &nSize);
    
    template <class TableClass>
    HRESULT ParseDataDirectory(_Inout_ TableClass *pTable, _In_ INT32 nDataDirectoryEntryIndex)
    {
        LIBPE_CHK(NULL != m_pLoader, E_UNEXPECTED);
        LIBPE_CHK(NULL != m_pFile, E_FAIL);;

        PEAddress nTableRVA = LIBPE_INVALID_ADDRESS, nTableFOA = LIBPE_INVALID_ADDRESS, nTableSize = LIBPE_INVALID_SIZE;
        LIBPE_CHK_HR(GetDataDirectoryEntry(nDataDirectoryEntryIndex, nTableRVA, nTableFOA, nTableSize));

        InitPEElement(pTable, nTableRVA, nTableFOA, nTableSize);

        return S_OK;
    }

    template <class ITable, class TableClass>
    HRESULT ParseDataDirectoryToInterface(_In_ INT32 nDataDirectoryEntryIndex, _Outptr_ ITable **ppTable)
    {
        LIBPE_CHK(NULL != m_pLoader, E_UNEXPECTED);
        LIBPE_CHK(NULL != m_pFile, E_FAIL);

        *ppTable = NULL;

        LibPEPtr<TableClass> pTable = new TableClass();
        PEElementParsingScopeT<T> oTableParsingScope(pTable);
        LIBPE_CHK_HR(ParseDataDirectory<TableClass>(pTable, nDataDirectoryEntryIndex));

        *ppTable = pTable.Detach();

        return S_OK;
    }

protected:
    LibPEPtr<DataLoader>    m_pLoader;
    PEFileT<T>              *m_pFile;
};

typedef PEParserT<PE32> PEParser32;
typedef PEParserT<PE64> PEParser64;


LIBPE_NAMESPACE_END