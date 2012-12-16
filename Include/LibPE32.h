#pragma once

#include "LibPEBase.h"
#include "LibPEError.h"
#include "LibPEInterface.h"
#include "LibPEPtr.h"

LIBPE_NAMESPACE_BEGIN

typedef IPEFileT<PE32>                  IPEFile32;
typedef IPEElementT<PE32>               IPEElement32;
typedef IPESectionHeaderT<PE32>         IPESectionHeader32;
typedef IPESectionT<PE32>               IPESection32;
typedef IPEExtraDataT<PE32>             IPEExtraData32;
typedef IPEExportTableT<PE32>           IPEExportTable32;
typedef IPEExportFunctionT<PE32>        IPEExportFunction32;
typedef IPEImportTableT<PE32>           IPEImportTable32;
typedef IPEImportModuleT<PE32>          IPEImportModule32;
typedef IPEImportFunctionT<PE32>        IPEImportFunction32;
typedef IPEResourceTableT<PE32>         IPEResourceTable32;
typedef IPEResourceTableItemT<PE32>     IPEResourceTableItem32;
typedef IPEExceptionTableT<PE32>        IPEExceptionTable32;
typedef IPECertificateTableT<PE32>      IPECertificateTable32;
typedef IPERelocationTableT<PE32>       IPERelocationTable32;
typedef IPERelocationPageT<PE32>        IPERelocationPage32;
typedef IPERelocationItemT<PE32>        IPERelocationItem32;
typedef IPEDebugInfoTableT<PE32>        IPEDebugInfoTable32;
typedef IPEGlobalRegisterT<PE32>        IPEGlobalRegister32;
typedef IPETlsTableT<PE32>              IPETlsTable32;
typedef IPEBoundImportTableT<PE32>      IPEBoundImportTable32;
typedef IPEImportAddressTableT<PE32>    IPEImportAddressTable32;
typedef IPEImportAddressBlockT<PE32>    IPEImportAddressBlock32;
typedef IPEImportAddressItemT<PE32>     IPEImportAddressItem32;
typedef IPEDelayImportTableT<PE32>      IPEDelayImportTable32;
typedef IPECLRHeaderT<PE32>             IPECLRHeader32;

error_t ParsePE32FromDiskFile(const file_char_t *pFilePath, IPEFile32 **ppFile);
error_t ParsePE32FromMappedFile(void *pMemory, IPEFile32 **ppFile);

#ifdef LIBPE_WINOS
error_t ParsePE32FromMappedResource(HMODULE hModule, IPEFile32 **ppFile);
error_t ParsePE32FromLoadedModule(HMODULE hModule, IPEFile32 **ppFile);
#endif

LIBPE_NAMESPACE_END