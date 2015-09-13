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
    PERawDosHeader * LIBPE_CALLTYPE GetRawDosHeader() override;
    void * LIBPE_CALLTYPE GetRawNtHeaders() override;
    PERawNtHeaders32 * LIBPE_CALLTYPE GetRawNtHeaders32() override;
    PERawNtHeaders64 * LIBPE_CALLTYPE GetRawNtHeaders64() override;
    PERawFileHeader * LIBPE_CALLTYPE GetRawFileHeader() override;
    void * LIBPE_CALLTYPE GetRawOptionalHeader() override;
    PERawOptionalHeader32 * LIBPE_CALLTYPE GetRawOptionalHeader32() override;
    PERawOptionalHeader64 * LIBPE_CALLTYPE GetRawOptionalHeader64() override;

    // PE Header
    HRESULT LIBPE_CALLTYPE GetDosHeader(IPEDosHeader **ppDosHeader) override;
    HRESULT LIBPE_CALLTYPE GetNtHeaders(IPENtHeaders **ppNtHeaders) override;
    HRESULT LIBPE_CALLTYPE GetFileHeader(IPEFileHeader **ppFileHeader) override;
    HRESULT LIBPE_CALLTYPE GetOptionalHeader(IPEOptionalHeader **ppOptionalHeader) override;

    // PE Header utilities
    BOOL LIBPE_CALLTYPE IsDosFile() override;
    BOOL LIBPE_CALLTYPE Is32Bit() override;
    PEAddress LIBPE_CALLTYPE GetImageBase() override;
    UINT32 LIBPE_CALLTYPE GetImageSize() override;
    UINT32 LIBPE_CALLTYPE GetEntryPoint() override;

    // Section
    UINT32 LIBPE_CALLTYPE GetSectionCount() override;
    HRESULT LIBPE_CALLTYPE GetSectionHeader(UINT32 nIndex, IPESectionHeader **ppSectionHeader) override;
    HRESULT LIBPE_CALLTYPE GetSection(UINT32 nIndex, IPESection **ppSection) override;
    HRESULT LIBPE_CALLTYPE GetSectionByRVA(PEAddress nRVA, IPESection **ppSection) override;
    HRESULT LIBPE_CALLTYPE GetSectionByVA(PEAddress nVA, IPESection **ppSection) override;
    HRESULT LIBPE_CALLTYPE GetSectionByFOA(PEAddress nFOA, IPESection **ppSection) override;
    HRESULT LIBPE_CALLTYPE GetOverlay(IPEOverlay **ppOverlay) override;

    // PEAddress convert tools
    PEAddress LIBPE_CALLTYPE GetRVAFromVA(PEAddress nVA) override;
    PEAddress LIBPE_CALLTYPE GetVAFromRVA(PEAddress nRVA) override;
    PEAddress LIBPE_CALLTYPE GetRVAFromFOA(PEAddress nFOA) override;
    PEAddress LIBPE_CALLTYPE GetFOAFromRVA(PEAddress nRVA) override;
    PEAddress LIBPE_CALLTYPE GetVAFromFOA(PEAddress nFOA) override;
    PEAddress LIBPE_CALLTYPE GetFOAFromVA(PEAddress nVA) override;

    // Data directory entries operations
    HRESULT LIBPE_CALLTYPE GetExportTable(IPEExportTable **ppExportTable) override;
    HRESULT LIBPE_CALLTYPE GetImportTable(IPEImportTable **ppImportTable) override;
    HRESULT LIBPE_CALLTYPE GetResourceTable(IPEResourceTable **ppResourceTable) override;
    HRESULT LIBPE_CALLTYPE GetExceptionTable(IPEExceptionTable **ppExceptionTable) override;
    HRESULT LIBPE_CALLTYPE GetCertificateTable(IPECertificateTable **ppCertificateTable) override;
    HRESULT LIBPE_CALLTYPE GetRelocationTable(IPERelocationTable **ppRelocationTable) override;
    HRESULT LIBPE_CALLTYPE GetDebugInfoTable(IPEDebugInfoTable **ppDebugInfoTable) override;
    HRESULT LIBPE_CALLTYPE GetArchitectureDataTable(IPEArchitectureDataTable **ppArchitectureDataTable) override;
    HRESULT LIBPE_CALLTYPE GetGlobalPointerTable(IPEGlobalPointerTable **ppGlobalPointerTable) override;
    HRESULT LIBPE_CALLTYPE GetTlsTable(IPETlsTable **ppTlsTable) override;
    HRESULT LIBPE_CALLTYPE GetLoadConfigTable(IPELoadConfigTable **ppLoadConfigTable) override;
    HRESULT LIBPE_CALLTYPE GetBoundImportTable(IPEBoundImportTable **ppBoundImportTable) override;
    HRESULT LIBPE_CALLTYPE GetImportAddressTable(IPEImportAddressTable **ppImportAddressTable) override;
    HRESULT LIBPE_CALLTYPE GetDelayImportTable(IPEDelayImportTable **ppDelayImportTable) override;
    HRESULT LIBPE_CALLTYPE GetClrTable(IPEClrTable **ppClrTable) override;

    HRESULT LIBPE_CALLTYPE RemoveExportTable() override { return E_NOTIMPL; };
    HRESULT LIBPE_CALLTYPE RemoveImportTable() override { return E_NOTIMPL; };
    HRESULT LIBPE_CALLTYPE RemoveResourceTable() override { return E_NOTIMPL; };
    HRESULT LIBPE_CALLTYPE RemoveExceptionTable() override { return E_NOTIMPL; };
    HRESULT LIBPE_CALLTYPE RemoveCertificateTable() override { return E_NOTIMPL; };
    HRESULT LIBPE_CALLTYPE RemoveRelocationTable() override { return E_NOTIMPL; };
    HRESULT LIBPE_CALLTYPE RemoveDebugInfoTable() override { return E_NOTIMPL; };
    HRESULT LIBPE_CALLTYPE RemoveGlobalPointerTable() override { return E_NOTIMPL; };
    HRESULT LIBPE_CALLTYPE RemoveTlsTable() override { return E_NOTIMPL; };
    HRESULT LIBPE_CALLTYPE RemoveBoundImportTable() override { return E_NOTIMPL; };
    HRESULT LIBPE_CALLTYPE RemoveImportAddressTable() override { return E_NOTIMPL; };
    HRESULT LIBPE_CALLTYPE RemoveDelayImportTable() override { return E_NOTIMPL; };
    HRESULT LIBPE_CALLTYPE RemoveClrTable() override { return E_NOTIMPL; };

    // PE Verification
    BOOL LIBPE_CALLTYPE ValidatePEHeader() override { return true; }

    // Rebuild
    HRESULT LIBPE_CALLTYPE Rebuild(const file_char_t *pFilePath) override { return S_OK; }

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