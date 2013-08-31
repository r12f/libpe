#pragma once

#include "Parser/PEParser.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEFileT :
    public IPEFile
{
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
    virtual HRESULT LIBPE_CALLTYPE GetGlobalRegister(IPEGlobalRegister **ppGlobalRegister) { return E_NOTIMPL; }
    virtual HRESULT LIBPE_CALLTYPE GetTlsTable(IPETlsTable **ppTlsTable);
    virtual HRESULT LIBPE_CALLTYPE GetLoadConfigTable(IPELoadConfigTable **ppLoadConfigTable);
    virtual HRESULT LIBPE_CALLTYPE GetBoundImportTable(IPEBoundImportTable **ppBoundImportTable) { return E_NOTIMPL; }
    virtual HRESULT LIBPE_CALLTYPE GetImportAddressTable(IPEImportAddressTable **ppImportAddressTable);
    virtual HRESULT LIBPE_CALLTYPE GetDelayImportTable(IPEDelayImportTable **ppDelayImportTable) { return E_NOTIMPL; }
    virtual HRESULT LIBPE_CALLTYPE GetCLRHeader(IPECLRHeader **ppCLRHeader) { return E_NOTIMPL; }

    virtual HRESULT LIBPE_CALLTYPE RemoveExportTable() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveImportTable() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveResourceTable() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveExceptionTable() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveCertificateTable() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveRelocationTable() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveDebugInfoTable() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveGlobalRegister() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveTlsTable() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveBoundImportTable() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveImportAddressTable() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveDelayImportTable() { return E_NOTIMPL; };
    virtual HRESULT LIBPE_CALLTYPE RemoveCLRHeader() { return E_NOTIMPL; };

    // PE Verification
    virtual BOOL LIBPE_CALLTYPE ValidatePEHeader() { return true; }

    // Rebuild
    virtual HRESULT LIBPE_CALLTYPE Rebuild(const file_char_t *pFilePath) { return S_OK; }

private:
    LibPEPtr<PEParserT<T>>                  m_pParser;
    LibPEPtr<IPEDosHeader>                  m_pDosHeader;
    LibPEPtr<IPENtHeaders>                  m_pNtHeaders;
    LibPEPtr<IPEFileHeader>                 m_pFileHeader;
    LibPEPtr<IPEOptionalHeader>             m_pOptionalHeader;
    SectionHeaderList                       m_vSectionHeaders;
    LibPEPtr<IPEOverlay>                    m_pOverlay;
    LibPEPtr<IPEExportTable>                m_pExportTable;
    LibPEPtr<IPEImportTable>                m_pImportTable;
    LibPEPtr<IPEResourceTable>              m_pResourceTable;
    LibPEPtr<IPEExceptionTable>             m_pExceptionTable;
    LibPEPtr<IPECertificateTable>           m_pCertificateTable;
    LibPEPtr<IPERelocationTable>            m_pRelocationTable;
    LibPEPtr<IPEDebugInfoTable>             m_pDebugInfoTable;
    LibPEPtr<IPETlsTable>                   m_pTlsTable;
    LibPEPtr<IPELoadConfigTable>            m_pLoadConfigTable;
    LibPEPtr<IPEImportAddressTable>         m_pImportAddressTable;
};

typedef PEFileT<PE32> PEFile32;
typedef PEFileT<PE64> PEFile64;

LIBPE_NAMESPACE_END