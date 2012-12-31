#pragma once

#include "Parser/PEParser.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEFileT :
    public IPEFile
{
    typedef std::vector<LibPEPtr<IPESectionHeader>> SectionHeaderList;

public:
    static error_t Create(DataLoader *pLoader, IPEFile **ppFile);

public:
    PEFileT();
    virtual ~PEFileT() {}

    LIBPE_SINGLE_THREAD_OBJECT();

    void Init(PEParserT<T> *pParser) {
        LIBPE_ASSERT_RET_VOID(NULL != pParser);
        m_pParser = pParser;
        if(ERR_OK == pParser->ParseBasicInfo(&m_pDosHeader, &m_pNtHeaders, &m_vSectionHeaders, &m_pOverlay) && NULL != m_pDosHeader && NULL != m_pNtHeaders) {
            m_pFileHeader = &(((LibPERawNtHeadersT(T) *)m_pNtHeaders)->FileHeader);
            m_pOptionalHeader = &(((LibPERawNtHeadersT(T) *)m_pNtHeaders)->OptionalHeader);
        }
    }

    // Override IPEFile
    // Dos header
    virtual PERawDosHeader * LIBPE_CALLTYPE GetDosHeader();
    virtual bool_t LIBPE_CALLTYPE IsDosFile();

    // PE Header
    virtual void * LIBPE_CALLTYPE GetNtHeaders();
    virtual PERawNtHeaders32 * LIBPE_CALLTYPE GetNtHeaders32();
    virtual PERawNtHeaders64 * LIBPE_CALLTYPE GetNtHeaders64();
    virtual PERawFileHeader * LIBPE_CALLTYPE GetFileHeader();
    virtual void * LIBPE_CALLTYPE GetOptionalHeader();
    virtual PERawOptionalHeader32 * LIBPE_CALLTYPE GetOptionalHeader32();
    virtual PERawOptionalHeader64 * LIBPE_CALLTYPE GetOptionalHeader64();
    virtual bool_t LIBPE_CALLTYPE Is32Bit();
    virtual PEAddress LIBPE_CALLTYPE GetImageBase();
    virtual uint32_t LIBPE_CALLTYPE GetImageSize();
    virtual uint32_t LIBPE_CALLTYPE GetEntryPoint();

    // Section
    virtual uint32_t LIBPE_CALLTYPE GetSectionCount();
    virtual error_t LIBPE_CALLTYPE GetSectionHeader(uint32_t nIndex, IPESectionHeader **ppSectionHeader);
    virtual error_t LIBPE_CALLTYPE GetSection(uint32_t nIndex, IPESection **ppSection);
    virtual error_t LIBPE_CALLTYPE GetSectionByRVA(PEAddress nRVA, IPESection **ppSection);
    virtual error_t LIBPE_CALLTYPE GetSectionByVA(PEAddress nVA, IPESection **ppSection);
    virtual error_t LIBPE_CALLTYPE GetSectionByFOA(PEAddress nFOA, IPESection **ppSection);
    virtual error_t LIBPE_CALLTYPE GetOverlay(IPEOverlay **ppOverlay);

    // PEAddress convert tools
    virtual PEAddress LIBPE_CALLTYPE GetRVAFromVA(PEAddress nVA);
    virtual PEAddress LIBPE_CALLTYPE GetVAFromRVA(PEAddress nRVA);
    virtual PEAddress LIBPE_CALLTYPE GetRVAFromFOA(PEAddress nFOA);
    virtual PEAddress LIBPE_CALLTYPE GetFOAFromRVA(PEAddress nRVA);
    virtual PEAddress LIBPE_CALLTYPE GetVAFromFOA(PEAddress nFOA);
    virtual PEAddress LIBPE_CALLTYPE GetFOAFromVA(PEAddress nVA);

    // Data directory entries operations
    virtual error_t LIBPE_CALLTYPE GetExportTable(IPEExportTable **ppExportTable);
    virtual error_t LIBPE_CALLTYPE GetImportTable(IPEImportTable **ppImportTable);
    virtual error_t LIBPE_CALLTYPE GetResourceTable(IPEResourceTable **ppResourceTable);
    virtual error_t LIBPE_CALLTYPE GetExceptionTable(IPEExceptionTable **ppExceptionTable);
    virtual error_t LIBPE_CALLTYPE GetCertificateTable(IPECertificateTable **ppCertificateTable);
    virtual error_t LIBPE_CALLTYPE GetRelocationTable(IPERelocationTable **ppRelocationTable);
    virtual error_t LIBPE_CALLTYPE GetDebugInfoTable(IPEDebugInfoTable **ppDebugInfoTable) { return ERR_NOT_IMPL; }
    virtual error_t LIBPE_CALLTYPE GetGlobalRegister(IPEGlobalRegister **ppGlobalRegister) { return ERR_NOT_IMPL; }
    virtual error_t LIBPE_CALLTYPE GetTlsTable(IPETlsTable **ppTlsTable) { return ERR_NOT_IMPL; }
    virtual error_t LIBPE_CALLTYPE GetBoundImportTable(IPEBoundImportTable **ppBoundImportTable) { return ERR_NOT_IMPL; }
    virtual error_t LIBPE_CALLTYPE GetImportAddressTable(IPEImportAddressTable **ppImportAddressTable);
    virtual error_t LIBPE_CALLTYPE GetDelayImportTable(IPEDelayImportTable **ppDelayImportTable) { return ERR_NOT_IMPL; }
    virtual error_t LIBPE_CALLTYPE GetCLRHeader(IPECLRHeader **ppCLRHeader) { return ERR_NOT_IMPL; }

    virtual error_t LIBPE_CALLTYPE RemoveExportTable() { return ERR_NOT_IMPL; };
    virtual error_t LIBPE_CALLTYPE RemoveImportTable() { return ERR_NOT_IMPL; };
    virtual error_t LIBPE_CALLTYPE RemoveResourceTable() { return ERR_NOT_IMPL; };
    virtual error_t LIBPE_CALLTYPE RemoveExceptionTable() { return ERR_NOT_IMPL; };
    virtual error_t LIBPE_CALLTYPE RemoveCertificateTable() { return ERR_NOT_IMPL; };
    virtual error_t LIBPE_CALLTYPE RemoveRelocationTable() { return ERR_NOT_IMPL; };
    virtual error_t LIBPE_CALLTYPE RemoveDebugInfoTable() { return ERR_NOT_IMPL; };
    virtual error_t LIBPE_CALLTYPE RemoveGlobalRegister() { return ERR_NOT_IMPL; };
    virtual error_t LIBPE_CALLTYPE RemoveTlsTable() { return ERR_NOT_IMPL; };
    virtual error_t LIBPE_CALLTYPE RemoveBoundImportTable() { return ERR_NOT_IMPL; };
    virtual error_t LIBPE_CALLTYPE RemoveImportAddressTable() { return ERR_NOT_IMPL; };
    virtual error_t LIBPE_CALLTYPE RemoveDelayImportTable() { return ERR_NOT_IMPL; };
    virtual error_t LIBPE_CALLTYPE RemoveCLRHeader() { return ERR_NOT_IMPL; };

    // PE Verification
    virtual bool_t LIBPE_CALLTYPE ValidatePEHeader() { return true; }

    // Rebuild
    virtual error_t LIBPE_CALLTYPE Rebuild(const file_char_t *pFilePath) { return ERR_OK; }

private:
    LibPEPtr<PEParserT<T>>                  m_pParser;
    PERawDosHeader                          *m_pDosHeader;
    void                                    *m_pNtHeaders;
    PERawFileHeader                         *m_pFileHeader;
    void                                    *m_pOptionalHeader;
    SectionHeaderList                       m_vSectionHeaders;
    LibPEPtr<IPEOverlay>                    m_pOverlay;
    LibPEPtr<IPEExportTable>                m_pExportTable;
    LibPEPtr<IPEImportTable>                m_pImportTable;
    LibPEPtr<IPEResourceTable>              m_pResourceTable;
    LibPEPtr<IPERelocationTable>            m_pRelocationTable;
    LibPEPtr<IPEImportAddressTable>         m_pImportAddressTable;
};

typedef PEFileT<PE32> PEFile32;
typedef PEFileT<PE64> PEFile64;

LIBPE_NAMESPACE_END