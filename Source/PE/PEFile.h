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

    void SetParser(PEParserT<T> *pParser) { m_pParser = pParser; }
    void SetDosHeader(LibPERawDosHeaderT(T) *pDosHeader) { m_pDosHeader = pDosHeader; }
    void SetNtHeaders(LibPERawNtHeadersT(T) *pNtHeaders) { m_pNtHeaders = pNtHeaders; }
    void SetFileHeader(LibPERawFileHeaderT(T) *pFileHeader) { m_pFileHeader = pFileHeader; }
    void SetOptionalHeader(LibPERawOptionalHeaderT(T) *pOptionalHeader) { m_pOptionalHeader = pOptionalHeader; }

    void AddSectionHeader(IPESectionHeaderT<T> *pSectionHeader) {
        LIBPE_ASSERT_RET_VOID(NULL != pSectionHeader);
        m_vSectionHeaders.push_back(pSectionHeader);
    }

    int8_t * GetRawMemory(uint64_t nOffset, uint64_t nSize) {
        LIBPE_ASSERT_RET(NULL != m_pParser, NULL);
        return m_pParser->GetRawMemory(nOffset, nSize);
    }

    // Override IPEFileT<T>
    // Rebuild
    virtual error_t LIBPE_CALLTYPE Rebuild(const file_char_t *pFilePath) { return ERR_OK; }

    // Basic info
    virtual bool_t LIBPE_CALLTYPE Is32BitFile() { return PETrait<T>::Is32Bit; }
    virtual LibPERawDosHeaderT(T) * LIBPE_CALLTYPE GetDosHeader();
    virtual LibPERawNtHeadersT(T) * LIBPE_CALLTYPE GetNtHeaders();
    virtual LibPERawFileHeaderT(T) * LIBPE_CALLTYPE GetFileHeader();
    virtual LibPERawOptionalHeaderT(T) * LIBPE_CALLTYPE GetOptionalHeader();

    // Section
    virtual uint32_t LIBPE_CALLTYPE GetSectionNum();
    virtual error_t LIBPE_CALLTYPE GetSectionHeader(uint32_t nIndex, IPESectionHeaderT<T> **ppSectionHeader);
    virtual error_t LIBPE_CALLTYPE GetSection(uint32_t nIndex, IPESectionT<T> **ppSection);

    // LibPEAddressT(T) convert tools
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetRVAFromVA(LibPEAddressT(T) nVA);
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetRVAFromFOA(LibPEAddressT(T) nFOA);
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetVAFromRVA(LibPEAddressT(T) nRVA);
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetVAFromFOA(LibPEAddressT(T) nFOA);
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetFOAFromRVA(LibPEAddressT(T) nRVA);
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetFOAFromVA(LibPEAddressT(T) nVA);

    // Data directory & Data Directory Entries
    virtual error_t LIBPE_CALLTYPE GetDataDirectory(int32_t nIndex, IPEDataDirectoryT<T> **ppDataDirectory) { return ERR_OK; }
    virtual error_t LIBPE_CALLTYPE GetExportTable(IPEExportTableT<T> **ppExportTable) { return ERR_OK; }
    virtual error_t LIBPE_CALLTYPE GetImportTable(IPEImportTableT<T> **ppImportTable) { return ERR_OK; }
    virtual error_t LIBPE_CALLTYPE GetResourceTable(IPEResourceTableT<T> **ppResourceTable) { return ERR_OK; }
    virtual error_t LIBPE_CALLTYPE GetExceptionTable(IPEExceptionTableT<T> **ppExceptionTable) { return ERR_OK; }
    virtual error_t LIBPE_CALLTYPE GetCertificateTable(IPECertificateTableT<T> **ppCertificateTable) { return ERR_OK; }
    virtual error_t LIBPE_CALLTYPE GetRelocationTable(IPERelocationTableT<T> **ppRelocationTable) { return ERR_OK; }
    virtual error_t LIBPE_CALLTYPE GetDebugInfoTable(IPEDebugInfoTableT<T> **ppDebugInfoTable) { return ERR_OK; }
    virtual error_t LIBPE_CALLTYPE GetGlobalRegister(IPEGlobalRegisterT<T> **ppGlobalRegister) { return ERR_OK; }
    virtual error_t LIBPE_CALLTYPE GetTlsTable(IPETlsTableT<T> **ppTlsTable) { return ERR_OK; }
    virtual error_t LIBPE_CALLTYPE GetBoundImportTable(IPEBoundImportTableT<T> **ppBoundImportTable) { return ERR_OK; }
    virtual error_t LIBPE_CALLTYPE GetImportAddressTable(IPEImportAddressTableT<T> **ppImportAddressTable) { return ERR_OK; }
    virtual error_t LIBPE_CALLTYPE GetDelayImportTable(IPEDelayImportTableT<T> **ppDelayImportTable) { return ERR_OK; }
    virtual error_t LIBPE_CALLTYPE GetCLRHeader(IPECLRHeaderT<T> **ppCLRHeader) { return ERR_OK; }

    // PE Verification
    virtual bool_t LIBPE_CALLTYPE IsValidPE() { return true; }

private:
    LibPEPtr<PEParserT<T>>          m_pParser;
    LibPERawDosHeaderT(T)           *m_pDosHeader;
    LibPERawNtHeadersT(T)           *m_pNtHeaders;
    LibPERawFileHeaderT(T)          *m_pFileHeader;
    LibPERawOptionalHeaderT(T)      *m_pOptionalHeader;
    SectionHeaderList               m_vSectionHeaders;
};

typedef PEFileT<PE32> PEFile32;
typedef PEFileT<PE64> PEFile64;

LIBPE_NAMESPACE_END