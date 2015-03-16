#pragma once

#include "Parser/PEParser.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEFileT :
    public IPEFile
{
    // Table parsed flags
    enum {
        TPF_IsExportTableParsed             = (1 << 0),
        TPF_IsImportTableParsed             = (1 << 1),
        TPF_IsResourceTableParsed           = (1 << 2),
        TPF_IsExceptionTableParsed          = (1 << 3),
        TPF_IsCertificateTableParsed        = (1 << 4),
        TPF_IsRelocationTableParsed         = (1 << 5),
        TPF_IsDebugInfoTableParsed          = (1 << 6),
        TPF_IsGlobalPointerTableParsed      = (1 << 7),
        TPF_IsTlsTableParsed                = (1 << 8),
        TPF_IsLoadConfigTableParsed         = (1 << 9),
        TPF_IsBoundImportTableParsed        = (1 << 10),
        TPF_IsImportAddressTableParsed      = (1 << 11),
        TPF_IsDelayImportTableParsed        = (1 << 12),
        TPF_IsClrTableParsed                = (1 << 13),
    };

    typedef std::vector<LibPEPtr<IPESectionHeader>> SectionHeaderList;

public:
    static HRESULT Create(DataLoader *pLoader, IPEFile **ppFile);

public:
    PEFileT() {}
    virtual ~PEFileT() {}

    LIBPE_SINGLE_THREAD_OBJECT();

    HRESULT Init(PEParserT<T> *pParser);

    // Override IPEFile
    // Raw PE Header
    virtual PERawDosHeader * LIBPE_CALLTYPE GetRawDosHeader();
    virtual void * LIBPE_CALLTYPE GetRawNtHeaders();
    virtual PERawNtHeaders32 * LIBPE_CALLTYPE GetRawNtHeaders32();
    virtual PERawNtHeaders64 * LIBPE_CALLTYPE GetRawNtHeaders64();
    virtual PERawFileHeader * LIBPE_CALLTYPE GetRawFileHeader();
    virtual void * LIBPE_CALLTYPE GetRawOptionalHeader();
    virtual PERawOptionalHeader32 * LIBPE_CALLTYPE GetRawOptionalHeader32();
    virtual PERawOptionalHeader64 * LIBPE_CALLTYPE GetRawOptionalHeader64();

    // PE Header
    virtual HRESULT LIBPE_CALLTYPE GetDosHeader(IPEDosHeader **ppDosHeader);
    virtual HRESULT LIBPE_CALLTYPE GetNtHeaders(IPENtHeaders **ppNtHeaders);
    virtual HRESULT LIBPE_CALLTYPE GetFileHeader(IPEFileHeader **ppFileHeader);
    virtual HRESULT LIBPE_CALLTYPE GetOptionalHeader(IPEOptionalHeader **ppOptionalHeader);

    // PE Header utilities
    virtual BOOL LIBPE_CALLTYPE IsDosFile();
    virtual BOOL LIBPE_CALLTYPE Is32Bit();
    virtual PEAddress LIBPE_CALLTYPE GetImageBase();
    virtual UINT32 LIBPE_CALLTYPE GetImageSize();
    virtual UINT32 LIBPE_CALLTYPE GetEntryPoint();

    // Section
    virtual UINT32 LIBPE_CALLTYPE GetSectionCount();
    virtual HRESULT LIBPE_CALLTYPE GetSectionHeader(UINT32 nIndex, IPESectionHeader **ppSectionHeader);
    virtual HRESULT LIBPE_CALLTYPE GetSection(UINT32 nIndex, IPESection **ppSection);
    virtual HRESULT LIBPE_CALLTYPE GetSectionByRVA(PEAddress nRVA, IPESection **ppSection);
    virtual HRESULT LIBPE_CALLTYPE GetSectionByVA(PEAddress nVA, IPESection **ppSection);
    virtual HRESULT LIBPE_CALLTYPE GetSectionByFOA(PEAddress nFOA, IPESection **ppSection);
    virtual HRESULT LIBPE_CALLTYPE GetOverlay(IPEOverlay **ppOverlay);

    // PEAddress convert tools
    virtual PEAddress LIBPE_CALLTYPE GetRVAFromVA(PEAddress nVA);
    virtual PEAddress LIBPE_CALLTYPE GetVAFromRVA(PEAddress nRVA);
    virtual PEAddress LIBPE_CALLTYPE GetRVAFromFOA(PEAddress nFOA);
    virtual PEAddress LIBPE_CALLTYPE GetFOAFromRVA(PEAddress nRVA);
    virtual PEAddress LIBPE_CALLTYPE GetVAFromFOA(PEAddress nFOA);
    virtual PEAddress LIBPE_CALLTYPE GetFOAFromVA(PEAddress nVA);

    // Data directory entries operations
    virtual HRESULT LIBPE_CALLTYPE GetExportTable(IPEExportTable **ppExportTable);
    virtual HRESULT LIBPE_CALLTYPE GetImportTable(IPEImportTable **ppImportTable);
    virtual HRESULT LIBPE_CALLTYPE GetResourceTable(IPEResourceTable **ppResourceTable);
    virtual HRESULT LIBPE_CALLTYPE GetExceptionTable(IPEExceptionTable **ppExceptionTable);
    virtual HRESULT LIBPE_CALLTYPE GetCertificateTable(IPECertificateTable **ppCertificateTable);
    virtual HRESULT LIBPE_CALLTYPE GetRelocationTable(IPERelocationTable **ppRelocationTable);
    virtual HRESULT LIBPE_CALLTYPE GetDebugInfoTable(IPEDebugInfoTable **ppDebugInfoTable);
    virtual HRESULT LIBPE_CALLTYPE GetArchitectureDataTable(IPEArchitectureDataTable **ppArchitectureDataTable);
    virtual HRESULT LIBPE_CALLTYPE GetGlobalPointerTable(IPEGlobalPointerTable **ppGlobalPointerTable);
    virtual HRESULT LIBPE_CALLTYPE GetTlsTable(IPETlsTable **ppTlsTable);
    virtual HRESULT LIBPE_CALLTYPE GetLoadConfigTable(IPELoadConfigTable **ppLoadConfigTable);
    virtual HRESULT LIBPE_CALLTYPE GetBoundImportTable(IPEBoundImportTable **ppBoundImportTable);
    virtual HRESULT LIBPE_CALLTYPE GetImportAddressTable(IPEImportAddressTable **ppImportAddressTable);
    virtual HRESULT LIBPE_CALLTYPE GetDelayImportTable(IPEDelayImportTable **ppDelayImportTable);
    virtual HRESULT LIBPE_CALLTYPE GetClrTable(IPEClrTable **ppClrTable);

    virtual HRESULT LIBPE_CALLTYPE RemoveExportTable() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveImportTable() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveResourceTable() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveExceptionTable() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveCertificateTable() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveRelocationTable() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveDebugInfoTable() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveGlobalPointerTable() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveTlsTable() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveBoundImportTable() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveImportAddressTable() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveDelayImportTable() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveClrTable() { return E_NOTIMPL; };

    // PE Verification
    virtual BOOL LIBPE_CALLTYPE ValidatePEHeader() { return true; }

    // Rebuild
    virtual HRESULT LIBPE_CALLTYPE Rebuild(const file_char_t *pFilePath) { return S_OK; }

protected:
    template <class ITable, class ParseFunc>
    HRESULT ParsePETable(ParseFunc pParseFunc, UINT32 nTableParsedFlag, LibPEPtr<ITable> &pParsedTable, ITable **ppRetTable)
    {
        HRESULT hr = S_OK;

        *ppRetTable = nullptr;

        if ((m_nTableParsedFlags & nTableParsedFlag) == 0) {
            LIBPE_CHK(NULL != m_pParser, E_FAIL);
            hr = (m_pParser->*pParseFunc)(&pParsedTable);
        } else {
            if(NULL == pParsedTable) {
                hr = E_NOT_SET;
            }
        }

        if (FAILED(hr)) {
            return hr;
        }

        return pParsedTable.CopyTo(ppRetTable);
    }

private:
    // Parser
    LibPEPtr<PEParserT<T>>                  m_pParser;

    // The following structures will always be parsed, otherwise even the most basic functions cannot work,
    // such as RVAToFOA.
    LibPEPtr<IPEDosHeader>                  m_pDosHeader;
    LibPEPtr<IPENtHeaders>                  m_pNtHeaders;
    LibPEPtr<IPEFileHeader>                 m_pFileHeader;
    LibPEPtr<IPEOptionalHeader>             m_pOptionalHeader;
    SectionHeaderList                       m_vSectionHeaders;
    LibPEPtr<IPEOverlay>                    m_pOverlay;

    // The following structures will be parsed on demand.
    LibPEPtr<IPEExportTable>                m_pExportTable;
    LibPEPtr<IPEImportTable>                m_pImportTable;
    LibPEPtr<IPEResourceTable>              m_pResourceTable;
    LibPEPtr<IPEExceptionTable>             m_pExceptionTable;
    LibPEPtr<IPECertificateTable>           m_pCertificateTable;
    LibPEPtr<IPERelocationTable>            m_pRelocationTable;
    LibPEPtr<IPEDebugInfoTable>             m_pDebugInfoTable;
    LibPEPtr<IPEGlobalPointerTable>         m_pGlobalPointerTable;
    LibPEPtr<IPETlsTable>                   m_pTlsTable;
    LibPEPtr<IPELoadConfigTable>            m_pLoadConfigTable;
    LibPEPtr<IPEBoundImportTable>           m_pBoundImportTable;
    LibPEPtr<IPEImportAddressTable>         m_pImportAddressTable;
    LibPEPtr<IPEDelayImportTable>           m_pDelayImportTable;
    LibPEPtr<IPEClrTable>                   m_pClrTable;

    // Table parsed flags
    UINT32                                  m_nTableParsedFlags;
};

typedef PEFileT<PE32> PEFile32;
typedef PEFileT<PE64> PEFile64;

LIBPE_NAMESPACE_END