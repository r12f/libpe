#pragma once

#include "LibPEBase.h"
#include "LibPEError.h"
#include "LibPEInterface.h"
#include "LibPEPtr.h"

LIBPE_NAMESPACE_BEGIN

typedef PEAddressT<PE64> PEAddress64;
typedef PEDosHeaderT<PE64> PEDosHeader64;
typedef PEFileHeaderT<PE64> PEFileHeader64;
typedef PENtHeadersT<PE64> PENtHeaders64;
typedef PEOptionalHeaderT<PE64> PEOptionalHeader64;

typedef IPEFileT<PE64>                  IPEFile64;
typedef IPEElementT<PE64>               IPEElement64;
typedef IPESectionT<PE64>               IPESection64;
typedef IPEDataDirectoryT<PE64>         IPEDataDirectory64;
typedef IPEDataDirectoryItemT<PE64>     IPEDataDirectoryItem64;
typedef IPEExportTableT<PE64>           IPEExportTable64;
typedef IPEExportTableItemT<PE64>       IPEExportTableItem64;
typedef IPEImportTableT<PE64>           IPEImportTable64;
typedef IPEImportTableItemT<PE64>       IPEImportTableItem64;
typedef IPEResourceTableT<PE64>         IPEResourceTable64;
typedef IPEResourceTableItemT<PE64>     IPEResourceTableItem64;
typedef IPEExceptionTableT<PE64>        IPEExceptionTable64;
typedef IPECertificateTableT<PE64>      IPECertificateTable64;
typedef IPERelocationTableT<PE64>       IPERelocationTable64;
typedef IPEDebugInfoTableT<PE64>        IPEDebugInfoTable64;
typedef IPEGlobalRegisterT<PE64>        IPEGlobalRegister64;
typedef IPETlsTableT<PE64>              IPETlsTable64;
typedef IPEBoundImportTableT<PE64>      IPEBoundImportTable64;
typedef IPEImportAddressTableT<PE64>    IPEImportAddressTable64;
typedef IPEDelayImportTableT<PE64>      IPEDelayImportTable64;
typedef IPECLRHeaderT<PE64>             IPECLRHeader64;

error_t ParsePE64FromDiskFile(const file_char_t *pFilePath, IPEFile64 **ppFile);
error_t ParsePE64FromMappedFile(void *pMemory, IPEFile64 **ppFile);

#ifdef LIBPE_WINOS
error_t ParsePE64FromMappedResource(HMODULE hModule, IPEFile64 **ppFile);
error_t ParsePE64FromLoadedModule(HMODULE hModule, IPEFile64 **ppFile);
#endif

LIBPE_NAMESPACE_END