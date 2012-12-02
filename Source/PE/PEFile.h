#pragma once

#include "PEBase.h"
#include "Parser/PEParser.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEFileT :
    public IPEFileT<T>,
    public PEBase
{
    friend class PEParserT<T>;

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

    // Override IPEFileT<T>
    // Rebuild
    virtual error_t LIBPE_CALLTYPE Rebuild(const file_char_t *pFilePath) { return ERR_OK; }

    // Basic info
    virtual bool_t LIBPE_CALLTYPE Is32BitFile() { return true; }
    virtual PEDosHeaderT<T> * LIBPE_CALLTYPE GetDosHeader() { return NULL; }
    virtual PENtHeadersT<T> * LIBPE_CALLTYPE GetNtHeaders() { return NULL; }
    virtual PEFileHeaderT<T> * LIBPE_CALLTYPE GetFileHeader() { return NULL; }
    virtual PEOptionalHeaderT<T> * LIBPE_CALLTYPE GetOptionalHeader() { return NULL; }

    // Section
    virtual int32_t LIBPE_CALLTYPE GetSectionNum() { return 0; }
    virtual error_t LIBPE_CALLTYPE GetSection(int32_t nIndex, IPESectionT<T> **ppSection) { return ERR_OK; }

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
    PENtHeadersT<T>             *m_pNtHeader;
    PEFileHeaderT<T>            *m_pFileHeader;
    PEOptionalHeaderT<T>        *m_pOptionalHeader;
};

typedef PEFileT<PE32> PEFile32;
typedef PEFileT<PE64> PEFile64;

LIBPE_NAMESPACE_END