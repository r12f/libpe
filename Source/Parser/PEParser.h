#pragma once

#include "Parser/PEParserCommon.h"
#include "Parser/DataLoader.h"

LIBPE_NAMESPACE_BEGIN

template <class T> class PEFileT;

template <class T>
class PEParserT
{
public:
    static PEParserT<T> * Create(PEParserType nType);
    static PEParserT<T> * CreateForDiskFile(const file_char_t *pFilePath, PEFileT<T> *pFile);
    static PEParserT<T> * CreateForMappedFile(void *pMemory, PEFileT<T> *pFile);
#ifdef LIBPE_WINOS
    static PEParserT<T> * CreateForMappedResource(HMODULE hModule, PEFileT<T> *pFile);
    static PEParserT<T> * CreateForLoadedModule(HMODULE hModule, PEFileT<T> *pFile);
#endif

public:
    PEParserT() : m_pLoader(NULL) {}
    virtual ~PEParserT() {}

    virtual PEParserType GetType() = 0;
    virtual error_t ParsePEBasicInfo() = 0;
    virtual error_t ParseDataDirectory(int32_t nIndex, IPEDataDirectoryT<T> **ppDataDirectory) { return ERR_NOT_IMPL; }
    virtual error_t ParseExportTable(IPEExportTableT<T> **ppExportTable) { return ERR_NOT_IMPL; }
    virtual error_t ParseImportTable(IPEImportTableT<T> **ppImportTable) { return ERR_NOT_IMPL; }
    virtual error_t ParseResourceTable(IPEResourceTableT<T> **ppResourceTable) { return ERR_NOT_IMPL; }
    virtual error_t ParseExceptionTable(IPEExceptionTableT<T> **ppExceptionTable) { return ERR_NOT_IMPL; }
    virtual error_t ParseCertificateTable(IPECertificateTableT<T> **ppCertificateTable) { return ERR_NOT_IMPL; }
    virtual error_t ParseRelocationTable(IPERelocationTableT<T> **ppRelocationTable) { return ERR_NOT_IMPL; }
    virtual error_t ParseDebugInfoTable(IPEDebugInfoTableT<T> **ppDebugInfoTable) { return ERR_NOT_IMPL; }
    virtual error_t ParseGlobalRegister(IPEGlobalRegisterT<T> **ppGlobalRegister) { return ERR_NOT_IMPL; }
    virtual error_t ParseTlsTable(IPETlsTableT<T> **ppTlsTable) { return ERR_NOT_IMPL; }
    virtual error_t ParseBoundImportTable(IPEBoundImportTableT<T> **ppBoundImportTable) { return ERR_NOT_IMPL; }
    virtual error_t ParseImportAddressTable(IPEImportAddressTableT<T> **ppImportAddressTable) { return ERR_NOT_IMPL; }
    virtual error_t ParseDelayImportTable(IPEDelayImportTableT<T> **ppDelayImportTable) { return ERR_NOT_IMPL; }
    virtual error_t ParseCLRHeader(IPECLRHeaderT<T> **ppCLRHeader) { return ERR_NOT_IMPL; }

protected:
    virtual PEAddressT<T> GetAddressFromRVA(PEAddressT<T> nRVA) = 0;
    virtual PEAddressT<T> GetAddressFromVA(PEAddressT<T> nVA) = 0;
    virtual PEAddressT<T> GetAddressFromFOA(PEAddressT<T> nFOA) = 0;

protected:
    void SetPEFile(PEFileT<T> *pFile) { m_pFile = pFile; }
    void SetDataLoader(DataLoader *pLoader) { m_pLoader = pLoader; }

    error_t SetPEDosHeader(PEDosHeaderT<T> *pDosHeader) { LIBPE_ASSERT_RET(NULL != m_pFile, ERR_FAIL); m_pFile->m_pDosHeader = pDosHeader; return ERR_OK; }
    error_t SetPEFileHeader(PEFileHeaderT<T> *pFileHeader) { LIBPE_ASSERT_RET(NULL != m_pFile, ERR_FAIL); m_pFile->m_pFileHeader = pFileHeader; return ERR_OK; }
    error_t SetPENtHeaders(PENtHeadersT<T> *pNtHeaders) { LIBPE_ASSERT_RET(NULL != m_pFile, ERR_FAIL); m_pFile->m_pNtHeader = pNtHeaders; return ERR_OK; }
    error_t SetPEOptionalHeader(PEOptionalHeaderT<T> *pOptionalHeader) { LIBPE_ASSERT_RET(NULL != m_pFile, ERR_FAIL); m_pFile->m_pOptionalHeader = pOptionalHeader; return ERR_OK; }

    PEDosHeaderT<T> * GetPEDosHeader() { LIBPE_ASSERT_RET(NULL != m_pFile, NULL); return m_pFile->m_pDosHeader; }
    PEFileHeaderT<T> * GetPEFileHeader() { LIBPE_ASSERT_RET(NULL != m_pFile, NULL); return m_pFile->m_pFileHeader; }
    PENtHeadersT<T> * GetPENtHeaders() { LIBPE_ASSERT_RET(NULL != m_pFile, NULL); return m_pFile->m_pNtHeader; }
    PEOptionalHeaderT<T> * GetPEOptionalHeader() { LIBPE_ASSERT_RET(NULL != m_pFile, NULL); return m_pFile->m_pOptionalHeader; }

protected:
    ScopedPtr<DataLoader> m_pLoader;
    PEFileT<T> *m_pFile;
};

typedef PEParserT<PE32> PEParser32;
typedef PEParserT<PE64> PEParser64;

LIBPE_NAMESPACE_END