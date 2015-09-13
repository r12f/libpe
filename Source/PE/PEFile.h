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
    static HRESULT Create(_In_ DataLoader *pLoader, _Outptr_ IPEFile **ppFile);

public:
    PEFileT() : m_nTableParsedFlags(0) {}
    virtual ~PEFileT() {}

    LIBPE_SINGLE_THREAD_OBJECT();

    HRESULT Init(_In_ PEParserT<T> *pParser);

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
    HRESULT LIBPE_CALLTYPE GetDosHeader(_Outptr_ IPEDosHeader **ppDosHeader) override;
    HRESULT LIBPE_CALLTYPE GetNtHeaders(_Outptr_ IPENtHeaders **ppNtHeaders) override;
    HRESULT LIBPE_CALLTYPE GetFileHeader(_Outptr_ IPEFileHeader **ppFileHeader) override;
    HRESULT LIBPE_CALLTYPE GetOptionalHeader(_Outptr_ IPEOptionalHeader **ppOptionalHeader) override;

    // PE Header utilities
    BOOL LIBPE_CALLTYPE IsDosFile() override;
    BOOL LIBPE_CALLTYPE Is32Bit() override;
    PEAddress LIBPE_CALLTYPE GetImageBase() override;
    UINT32 LIBPE_CALLTYPE GetImageSize() override;
    UINT32 LIBPE_CALLTYPE GetEntryPoint() override;

    // Section
    UINT32 LIBPE_CALLTYPE GetSectionCount() override;
    HRESULT LIBPE_CALLTYPE GetSectionHeader(_In_ UINT32 nIndex, _Outptr_ IPESectionHeader **ppSectionHeader) override;
    HRESULT LIBPE_CALLTYPE GetSection(_In_ UINT32 nIndex, _Outptr_ IPESection **ppSection) override;
    HRESULT LIBPE_CALLTYPE GetSectionByRVA(_In_ PEAddress nRVA, _Outptr_ IPESection **ppSection) override;
    HRESULT LIBPE_CALLTYPE GetSectionByVA(_In_ PEAddress nVA, _Outptr_ IPESection **ppSection) override;
    HRESULT LIBPE_CALLTYPE GetSectionByFOA(_In_ PEAddress nFOA, _Outptr_ IPESection **ppSection) override;
    HRESULT LIBPE_CALLTYPE GetOverlay(_Outptr_ IPEOverlay **ppOverlay) override;

    // PEAddress convert tools
    PEAddress LIBPE_CALLTYPE GetRVAFromVA(_In_ PEAddress nVA) override;
    PEAddress LIBPE_CALLTYPE GetVAFromRVA(_In_ PEAddress nRVA) override;
    PEAddress LIBPE_CALLTYPE GetRVAFromFOA(_In_ PEAddress nFOA) override;
    PEAddress LIBPE_CALLTYPE GetFOAFromRVA(_In_ PEAddress nRVA) override;
    PEAddress LIBPE_CALLTYPE GetVAFromFOA(_In_ PEAddress nFOA) override;
    PEAddress LIBPE_CALLTYPE GetFOAFromVA(_In_ PEAddress nVA) override;

    // Data directory entries operations
    HRESULT LIBPE_CALLTYPE GetExportTable(_Outptr_ IPEExportTable **ppExportTable) override;
    HRESULT LIBPE_CALLTYPE GetImportTable(_Outptr_ IPEImportTable **ppImportTable) override;
    HRESULT LIBPE_CALLTYPE GetResourceTable(_Outptr_ IPEResourceTable **ppResourceTable) override;
    HRESULT LIBPE_CALLTYPE GetExceptionTable(_Outptr_ IPEExceptionTable **ppExceptionTable) override;
    HRESULT LIBPE_CALLTYPE GetCertificateTable(_Outptr_ IPECertificateTable **ppCertificateTable) override;
    HRESULT LIBPE_CALLTYPE GetRelocationTable(_Outptr_ IPERelocationTable **ppRelocationTable) override;
    HRESULT LIBPE_CALLTYPE GetDebugInfoTable(_Outptr_ IPEDebugInfoTable **ppDebugInfoTable) override;
    HRESULT LIBPE_CALLTYPE GetArchitectureDataTable(_Outptr_ IPEArchitectureDataTable **ppArchitectureDataTable) override;
    HRESULT LIBPE_CALLTYPE GetGlobalPointerTable(_Outptr_ IPEGlobalPointerTable **ppGlobalPointerTable) override;
    HRESULT LIBPE_CALLTYPE GetTlsTable(_Outptr_ IPETlsTable **ppTlsTable) override;
    HRESULT LIBPE_CALLTYPE GetLoadConfigTable(_Outptr_ IPELoadConfigTable **ppLoadConfigTable) override;
    HRESULT LIBPE_CALLTYPE GetBoundImportTable(_Outptr_ IPEBoundImportTable **ppBoundImportTable) override;
    HRESULT LIBPE_CALLTYPE GetImportAddressTable(_Outptr_ IPEImportAddressTable **ppImportAddressTable) override;
    HRESULT LIBPE_CALLTYPE GetDelayImportTable(_Outptr_ IPEDelayImportTable **ppDelayImportTable) override;
    HRESULT LIBPE_CALLTYPE GetClrTable(_Outptr_ IPEClrTable **ppClrTable) override;

protected:
    template <class ITable, class ParseFunc>
    HRESULT ParsePETable(_In_ ParseFunc pParseFunc, _In_ UINT32 nTableParsedFlag, _Out_ LibPEPtr<ITable> &pParsedTable, _Outptr_ ITable **ppRetTable)
    {
        HRESULT hr = S_OK;

        *ppRetTable = nullptr;

        if ((m_nTableParsedFlags & nTableParsedFlag) == 0) {
            LIBPE_STRICTCHK(NULL != m_pParser);
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