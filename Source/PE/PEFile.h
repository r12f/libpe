#pragma once

#include "Parser/PEParser.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEFileT :
    public IPEFileT<T>
{
    typedef std::vector<LibPEPtr<IPESectionHeaderT<T>>> SectionHeaderList;

public:
    static error_t ParsePEFromDiskFile(const file_char_t *pFilePath, IPEFileT<T> **ppFile);
    static error_t ParsePEFromMappedFile(void *pMemory, IPEFileT<T> **ppFile);
#ifdef LIBPE_WINOS
    static error_t ParsePEFromMappedResource(HMODULE hModule, IPEFileT<T> **ppFile);
    static error_t ParsePEFromLoadedModule(HMODULE hModule, IPEFileT<T> **ppFile);
#endif

public:
    PEFileT();
    virtual ~PEFileT() {}

    LIBPE_SINGLE_THREAD_OBJECT();

    void Init(PEParserT<T> *pParser) {
        LIBPE_ASSERT_RET_VOID(NULL != pParser);
        m_pParser = pParser;
        if(ERR_OK == pParser->ParseBasicInfo(&m_pDosHeader, &m_pNtHeaders, &m_vSectionHeaders, &m_pOverlay) && NULL != m_pDosHeader && NULL != m_pNtHeaders) {
            m_pFileHeader = &(m_pNtHeaders->FileHeader);
            m_pOptionalHeader = &(m_pNtHeaders->OptionalHeader);
        }
    }

    // Override IPEFileT<T>
    // Basic info
    virtual LibPERawDosHeaderT(T) * LIBPE_CALLTYPE GetDosHeader();
    virtual LibPERawNtHeadersT(T) * LIBPE_CALLTYPE GetNtHeaders();
    virtual LibPERawFileHeaderT(T) * LIBPE_CALLTYPE GetFileHeader();
    virtual LibPERawOptionalHeaderT(T) * LIBPE_CALLTYPE GetOptionalHeader();
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetImageBase();
    virtual uint32_t LIBPE_CALLTYPE GetImageSize();
    virtual uint32_t LIBPE_CALLTYPE GetEntryPoint();

    // Section
    virtual uint32_t LIBPE_CALLTYPE GetSectionCount();
    virtual error_t LIBPE_CALLTYPE GetSectionHeader(uint32_t nIndex, IPESectionHeaderT<T> **ppSectionHeader);
    virtual error_t LIBPE_CALLTYPE GetSection(uint32_t nIndex, IPESectionT<T> **ppSection);
    virtual error_t LIBPE_CALLTYPE GetSectionByRVA(LibPEAddressT(T) nRVA, IPESectionT<T> **ppSection);
    virtual error_t LIBPE_CALLTYPE GetSectionByVA(LibPEAddressT(T) nVA, IPESectionT<T> **ppSection);
    virtual error_t LIBPE_CALLTYPE GetSectionByFOA(LibPEAddressT(T) nFOA, IPESectionT<T> **ppSection);
    virtual error_t LIBPE_CALLTYPE GetOverlay(IPEOverlayT<T> **ppOverlay);

    // LibPEAddressT(T) convert tools
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetRVAFromVA(LibPEAddressT(T) nVA);
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetVAFromRVA(LibPEAddressT(T) nRVA);
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetRVAFromFOA(LibPEAddressT(T) nFOA);
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetFOAFromRVA(LibPEAddressT(T) nRVA);
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetVAFromFOA(LibPEAddressT(T) nFOA);
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetFOAFromVA(LibPEAddressT(T) nVA);

    // Data directory entries operations
    virtual error_t LIBPE_CALLTYPE GetExportTable(IPEExportTableT<T> **ppExportTable);
    virtual error_t LIBPE_CALLTYPE GetImportTable(IPEImportTableT<T> **ppImportTable);
    virtual error_t LIBPE_CALLTYPE GetResourceTable(IPEResourceTableT<T> **ppResourceTable);
    virtual error_t LIBPE_CALLTYPE GetExceptionTable(IPEExceptionTableT<T> **ppExceptionTable);
    virtual error_t LIBPE_CALLTYPE GetCertificateTable(IPECertificateTableT<T> **ppCertificateTable);
    virtual error_t LIBPE_CALLTYPE GetRelocationTable(IPERelocationTableT<T> **ppRelocationTable);
    virtual error_t LIBPE_CALLTYPE GetDebugInfoTable(IPEDebugInfoTableT<T> **ppDebugInfoTable) { return ERR_NOT_IMPL; }
    virtual error_t LIBPE_CALLTYPE GetGlobalRegister(IPEGlobalRegisterT<T> **ppGlobalRegister) { return ERR_NOT_IMPL; }
    virtual error_t LIBPE_CALLTYPE GetTlsTable(IPETlsTableT<T> **ppTlsTable) { return ERR_NOT_IMPL; }
    virtual error_t LIBPE_CALLTYPE GetBoundImportTable(IPEBoundImportTableT<T> **ppBoundImportTable) { return ERR_NOT_IMPL; }
    virtual error_t LIBPE_CALLTYPE GetImportAddressTable(IPEImportAddressTableT<T> **ppImportAddressTable);
    virtual error_t LIBPE_CALLTYPE GetDelayImportTable(IPEDelayImportTableT<T> **ppDelayImportTable) { return ERR_NOT_IMPL; }
    virtual error_t LIBPE_CALLTYPE GetCLRHeader(IPECLRHeaderT<T> **ppCLRHeader) { return ERR_NOT_IMPL; }

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
    LibPEPtr<PEParserT<T>>              m_pParser;
    LibPERawDosHeaderT(T)               *m_pDosHeader;
    LibPERawNtHeadersT(T)               *m_pNtHeaders;
    LibPERawFileHeaderT(T)              *m_pFileHeader;
    LibPERawOptionalHeaderT(T)          *m_pOptionalHeader;
    SectionHeaderList                   m_vSectionHeaders;
    LibPEPtr<IPEOverlayT<T>>          m_pOverlay;
    LibPEPtr<IPEExportTableT<T>>        m_pExportTable;
    LibPEPtr<IPEImportTableT<T>>        m_pImportTable;
    LibPEPtr<IPERelocationTableT<T>>    m_pRelocationTable;
    LibPEPtr<IPEImportAddressTableT<T>> m_pImportAddressTable;
};

typedef PEFileT<PE32> PEFile32;
typedef PEFileT<PE64> PEFile64;

LIBPE_NAMESPACE_END