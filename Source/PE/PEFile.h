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
    PEFileT(DataLoader *pRawFile);
    virtual ~PEFileT();

    // Override IPEFileT<T>
    // Rebuild
    virtual error_t LIBPE_CALLTYPE Rebuild(const file_char_t *pFilePath) {}

    // Basic info
    virtual bool_t LIBPE_CALLTYPE Is32BitFile() {}
    virtual PEDosHeaderT<T> * LIBPE_CALLTYPE GetDosHeader() {}
    virtual PENtHeadersT<T> * LIBPE_CALLTYPE GetNtHeaders() {}
    virtual PEFileHeaderT<T> * LIBPE_CALLTYPE GetFileHeader() {}
    virtual PEOptionalHeaderT<T> * LIBPE_CALLTYPE GetOptionalHeader() {}

    // Section
    virtual int32_t LIBPE_CALLTYPE GetSectionNum() {}
    virtual error_t LIBPE_CALLTYPE GetSection(int32_t nIndex, IPESectionT<T> **ppSection) {}

    // PEAddressT<T> convert tools
    virtual PEAddressT<T> LIBPE_CALLTYPE GetRVAFromFOA(PEAddressT<T> nFOA) {}
    virtual PEAddressT<T> LIBPE_CALLTYPE GetVAFromFOA(PEAddressT<T> nFOA) {}
    virtual PEAddressT<T> LIBPE_CALLTYPE GetFOAFromRVA(PEAddressT<T> nRVA) {}
    virtual PEAddressT<T> LIBPE_CALLTYPE GetFOAFromVA(PEAddressT<T> nVA) {}

    // Data directory & Data Directory Entries
    virtual error_t LIBPE_CALLTYPE GetDataDirectory(int32_t nIndex, IPEDataDirectoryT<T> **ppDataDirectory) {}
    virtual error_t LIBPE_CALLTYPE GetExportTable(IPEExportTableT<T> **ppExportTable) {}
    virtual error_t LIBPE_CALLTYPE GetImportTable(IPEImportTableT<T> **ppImportTable) {}
    virtual error_t LIBPE_CALLTYPE GetResourceTable(IPEResourceTableT<T> **ppResourceTable) {}
    virtual error_t LIBPE_CALLTYPE GetExceptionTable(IPEExceptionTableT<T> **ppExceptionTable) {}
    virtual error_t LIBPE_CALLTYPE GetCertificateTable(IPECertificateTableT<T> **ppCertificateTable) {}
    virtual error_t LIBPE_CALLTYPE GetRelocationTable(IPERelocationTableT<T> **ppRelocationTable) {}
    virtual error_t LIBPE_CALLTYPE GetDebugInfoTable(IPEDebugInfoTableT<T> **ppDebugInfoTable) {}
    virtual error_t LIBPE_CALLTYPE GetGlobalRegister(IPEGlobalRegisterT<T> **ppGlobalRegister) {}
    virtual error_t LIBPE_CALLTYPE GetTlsTable(IPETlsTableT<T> **ppTlsTable) {}
    virtual error_t LIBPE_CALLTYPE GetBoundImportTable(IPEBoundImportTableT<T> **ppBoundImportTable) {}
    virtual error_t LIBPE_CALLTYPE GetImportAddressTable(IPEImportAddressTableT<T> **ppImportAddressTable) {}
    virtual error_t LIBPE_CALLTYPE GetDelayImportTable(IPEDelayImportTableT<T> **ppDelayImportTable) {}
    virtual error_t LIBPE_CALLTYPE GetCLRHeader(IPECLRHeaderT<T> **ppCLRHeader) {}

    // PE Verification
    virtual bool_t LIBPE_CALLTYPE IsValidPE() {}

private:
    PEParserT<T>            *m_pParser;
    PEDosHeaderT<T>         *m_pDosHeader;
    PENtHeadersT<T>         *m_pNtHeader;
    PEFileHeaderT<T>        *m_pFileHeader;
    PEOptionalHeaderT<T>    *m_pOptionalHeader;
};

typedef PEFileT<PE32> PEFile32;
typedef PEFileT<PE64> PEFile64;

LIBPE_NAMESPACE_END