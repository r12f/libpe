#pragma once

#include "LibPEBase.h"
#include "LibPEError.h"
#include "LibPEInterface.h"
#include "LibPEPtr.h"

LIBPE_NAMESPACE_BEGIN

typedef IPEFileT<PE32>                  IPEFile32;
typedef IPEElementT<PE32>               IPEElement32;
typedef IPESectionT<PE32>               IPESection32;
typedef IPEDataDirectoryT<PE32>         IPEDataDirectory32;
typedef IPEDataDirectoryItemT<PE32>     IPEDataDirectoryItem32;
typedef IPEExportTableT<PE32>           IPEExportTable32;
typedef IPEExportTableItemT<PE32>       IPEExportTableItem32;
typedef IPEImportTableT<PE32>           IPEImportTable32;
typedef IPEImportTableItemT<PE32>       IPEImportTableItem32;
typedef IPEResourceTableT<PE32>         IPEResourceTable32;
typedef IPEResourceTableItemT<PE32>     IPEResourceTableItem32;
typedef IPEExceptionTableT<PE32>        IPEExceptionTable32;
typedef IPECertificateTableT<PE32>      IPECertificateTable32;
typedef IPERelocationTableT<PE32>       IPERelocationTableT32;
typedef IPEDebugInfoTableT<PE32>        IPEDebugInfoTable32;
typedef IPEGlobalRegisterT<PE32>        IPEGlobalRegister32;
typedef IPETlsTableT<PE32>              IPETlsTable32;
typedef IPEBoundImportTableT<PE32>      IPEBoundImportTable32;
typedef IPEImportAddressTableT<PE32>    IPEImportAddressTable32;
typedef IPEDelayImportTableT<PE32>      IPEDelayImportTable32;
typedef IPECLRHeaderT<PE32>             IPECLRHeader32;

error_t ParsePE32FromDiskFile(const file_char_t *pFilePath, IPEFile32 **ppFile);
error_t ParsePE32FromMappedFile(void *pMemory, IPEFile32 **ppFile);

#ifdef LIBPE_WINOS
error_t ParsePE32FromMappedResource(HMODULE hModule, IPEFile32 **ppFile);
error_t ParsePE32FromLoadedModule(HMODULE hModule, IPEFile32 **ppFile);
#endif

LIBPE_NAMESPACE_END