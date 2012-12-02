#pragma once

#include "Parser/PEParser.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEFileT :
    public IPEFileT<T>
{
    typedef std::vector<LibPEPtr<IPESectionT<T>>> SectionList;

public:
    static error_t ParsePEFromDiskFile(const file_char_t *pFilePath, IPEFileT<T> **ppFile);
    static error_t ParsePEFromMappedFile(void *pMemory, IPEFileT<T> **ppFile);
#ifdef LIBPE_WINOS
    static error_t ParsePEFromMappedResource(HMODULE hModule, IPEFileT<T> **ppFile);
    static error_t ParsePEFromLoadedModule(HMODULE hModule, IPEFileT<T> **ppFile);
#endif

public:
    PEFileT();
    virtual ~PEFileT();

    LIBPE_SINGLE_THREAD_OBJECT();

    void SetParser(PEParserT<T> *pParser) { m_pParser = pParser; }
    void SetPEDosHeader(PEDosHeaderT<T> *pDosHeader) { m_pDosHeader = pDosHeader; }
    void SetPENtHeaders(PENtHeadersT<T> *pNtHeaders) { m_pNtHeaders = pNtHeaders; }
    void SetPEFileHeader(PEFileHeaderT<T> *pFileHeader) { m_pFileHeader = pFileHeader; }
    void SetPEOptionalHeader(PEOptionalHeaderT<T> *pOptionalHeader) { m_pOptionalHeader = pOptionalHeader; }
    void AddPESection(IPESectionT<T> *pSection) { LIBPE_ASSERT_RET_VOID(NULL != pSection); m_vSections.push_back(pSection); }

    int8_t * GetRawMemory(uint64_t nOffset, uint64_t nSize);

    // Override IPEFileT<T>
    // Rebuild
    virtual error_t LIBPE_CALLTYPE Rebuild(const file_char_t *pFilePath) { return ERR_OK; }

    // Basic info
    virtual bool_t LIBPE_CALLTYPE Is32BitFile() { return PETrait<T>::Is32Bit; }
    virtual PEDosHeaderT<T> * LIBPE_CALLTYPE GetDosHeader();
    virtual PENtHeadersT<T> * LIBPE_CALLTYPE GetNtHeaders();
    virtual PEFileHeaderT<T> * LIBPE_CALLTYPE GetFileHeader();
    virtual PEOptionalHeaderT<T> * LIBPE_CALLTYPE GetOptionalHeader();

    // Section
    virtual uint32_t LIBPE_CALLTYPE GetSectionNum();
    virtual error_t LIBPE_CALLTYPE GetSection(uint32_t nIndex, IPESectionT<T> **ppSection);

    // PEAddressT<T> convert tools
    virtual PEAddressT<T> LIBPE_CALLTYPE GetRVAFromFOA(PEAddressT<T> nFOA) { return 0; }
    virtual PEAddressT<T> LIBPE_CALLTYPE GetVAFromFOA(PEAddressT<T> nFOA) { return 0; }
    virtual PEAddressT<T> LIBPE_CALLTYPE GetFOAFromRVA(PEAddressT<T> nRVA) { return 0; }
    virtual PEAddressT<T> LIBPE_CALLTYPE GetFOAFromVA(PEAddressT<T> nVA) { return 0; }

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
    ScopedPtr<PEParserT<T>>     m_pParser;
    PEDosHeaderT<T>             *m_pDosHeader;
    PENtHeadersT<T>             *m_pNtHeaders;
    PEFileHeaderT<T>            *m_pFileHeader;
    PEOptionalHeaderT<T>        *m_pOptionalHeader;
    SectionList                 m_vSections;
};

typedef PEFileT<PE32> PEFile32;
typedef PEFileT<PE64> PEFile64;

LIBPE_NAMESPACE_END