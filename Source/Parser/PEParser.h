#pragma once

#include "Parser/PEParserCommon.h"
#include "Parser/DataLoader.h"

LIBPE_NAMESPACE_BEGIN

template <class T> class PEFileT;
template <class T> class PEParserFactoryT;

template <class T>
class PEParserT
{
    friend class PEParserFactoryT<T>;

public:
    PEParserT() : m_pLoader(NULL) {}
    virtual ~PEParserT() {}

    virtual PEParserType GetType() = 0;
    virtual error_t ParsePEBasicInfo() = 0;
    virtual error_t ParseDataDirectory(int32_t nIndex, IPEDataDirectoryT<T> **ppDataDirectory) {}
    virtual error_t ParseExportTable(IPEExportTableT<T> **ppExportTable) {}
    virtual error_t ParseImportTable(IPEImportTableT<T> **ppImportTable) {}
    virtual error_t ParseResourceTable(IPEResourceTableT<T> **ppResourceTable) {}
    virtual error_t ParseExceptionTable(IPEExceptionTableT<T> **ppExceptionTable) {}
    virtual error_t ParseCertificateTable(IPECertificateTableT<T> **ppCertificateTable) {}
    virtual error_t ParseRelocationTable(IPERelocationTableT<T> **ppRelocationTable) {}
    virtual error_t ParseDebugInfoTable(IPEDebugInfoTableT<T> **ppDebugInfoTable) {}
    virtual error_t ParseGlobalRegister(IPEGlobalRegisterT<T> **ppGlobalRegister) {}
    virtual error_t ParseTlsTable(IPETlsTableT<T> **ppTlsTable) {}
    virtual error_t ParseBoundImportTable(IPEBoundImportTableT<T> **ppBoundImportTable) {}
    virtual error_t ParseImportAddressTable(IPEImportAddressTableT<T> **ppImportAddressTable) {}
    virtual error_t ParseDelayImportTable(IPEDelayImportTableT<T> **ppDelayImportTable) {}
    virtual error_t ParseCLRHeader(IPECLRHeaderT<T> **ppCLRHeader) {}

protected:
    virtual PEAddressT<T> GetRVAFromRawAddress(PEAddressT<T> nRawAddress) = 0;
    virtual PEAddressT<T> GetVAFromRawAddress(PEAddressT<T> nRawAddress) = 0;
    virtual PEAddressT<T> GetFOAFromRawAddress(PEAddressT<T> nRawAddress) = 0;

protected:
    void SetPEFile(PEFileT<T> *pFile) { m_pFile = pFile; }
    void SetDataLoader(DataLoader *pLoader) { m_pLoader = pLoader; }

    error_t SetPEDosHeader(PEDosHeaderT<T> *pDosHeader) { LIBPE_ASSERT_RET(NULL != m_pFile, ERR_FAIL); m_pFile->m_pDosHeader = pDosHeader; }
    error_t SetPEFileHeader(PEFileHeaderT<T> *pFileHeader) { LIBPE_ASSERT_RET(NULL != m_pFile, ERR_FAIL); m_pFile->m_pFileHeader = pFileHeader; }
    error_t SetPENtHeaders(PENtHeadersT<T> *pNtHeaders) { LIBPE_ASSERT_RET(NULL != m_pFile, ERR_FAIL); m_pFile->m_pNtHeader = pNtHeaders; }
    error_t SetPEOptionalHeader(PEOptionalHeaderT<T> *pOptionalHeader) { LIBPE_ASSERT_RET(NULL != m_pFile, ERR_FAIL); m_pFile->m_pOptionalHeader = pOptionalHeader; }

    PEDosHeaderT<T> * GetPEDosHeader() { LIBPE_ASSERT_RET(NULL != m_pFile, NULL); return m_pFile->m_pDosHeader; }
    PEFileHeaderT<T> * GetPEFileHeader() { LIBPE_ASSERT_RET(NULL != m_pFile, NULL); return m_pFile->m_pFileHeader; }
    PENtHeadersT<T> * GetPENtHeaders() { LIBPE_ASSERT_RET(NULL != m_pFile, NULL); return m_pFile->m_pNtHeader; }
    PEOptionalHeaderT<T> * GetPEOptionalHeader() { LIBPE_ASSERT_RET(NULL != m_pFile, NULL); return m_pFile->m_pOptionalHeader; }

private:
    ScopedPtr<DataLoader> m_pLoader;
    PEFileT<T> *m_pFile;
};

LIBPE_NAMESPACE_END