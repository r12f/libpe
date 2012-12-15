#pragma once

#include "LibPE32.h"
#include "LibPE64.h"

LIBPE_NAMESPACE_BEGIN

void LIBPE_CALLTYPE SetPELoaderIOBlockSize(uint64_t nMinBlockSize, uint64_t nMaxBlockSize);

#ifndef _WIN64

// Compile under x86 configuration
typedef PEAddress32                 PEAddress;
typedef PERawNtHeaders32            PERawNtHeaders;
typedef PERawOptionalHeader32       PERawOptionalHeader;
typedef PERawThunkData32            PERawThunkData;

typedef IPEFile32                   IPEFile;
typedef IPEElement32                IPEElement;
typedef IPESectionHeader32          IPESectionHeader;
typedef IPESection32                IPESection;
typedef IPEExportTable32            IPEExportTable;
typedef IPEExportFunction32         IPEExportFunction;
typedef IPEImportTable32            IPEImportTable;
typedef IPEImportModule32           IPEImportModule;
typedef IPEImportFunction32         IPEImportFunction;
typedef IPEResourceTable32          IPEResourceTable;
typedef IPEResourceTableItem32      IPEResourceTableItem;
typedef IPEExceptionTable32         IPEExceptionTable;
typedef IPECertificateTable32       IPECertificateTable;
typedef IPERelocationTable32        IPERelocationTable;
typedef IPERelocationPage32         IPERelocationPage;
typedef IPERelocationItem32         IPERelocationItem;
typedef IPEDebugInfoTable32         IPEDebugInfoTable;
typedef IPEGlobalRegister32         IPEGlobalRegister;
typedef IPETlsTable32               IPETlsTable;
typedef IPEBoundImportTable32       IPEBoundImportTable;
typedef IPEImportAddressTable32     IPEImportAddressTable;
typedef IPEDelayImportTable32       IPEDelayImportTable;
typedef IPECLRHeader32              IPECLRHeader;

#else

typedef PEAddress64                 PEAddress;
typedef PERawNtHeaders64            PERawNtHeaders;
typedef PERawOptionalHeader64       PERawOptionalHeader;
typedef PERawThunkData64            PERawThunkData;

typedef IPEFile64                   IPEFile;
typedef IPEElement64                IPEElement;
typedef IPESectionHeader64          IPESectionHeader;
typedef IPESection64                IPESection;
typedef IPEExportTable64            IPEExportTable;
typedef IPEExportFunction64         IPEExportFunction;
typedef IPEImportTable64            IPEImportTable;
typedef IPEImportModule64           IPEImportModule;
typedef IPEImportFunction64         IPEImportFunction;
typedef IPEResourceTable64          IPEResourceTable;
typedef IPEResourceTableItem64      IPEResourceTableItem;
typedef IPEExceptionTable64         IPEExceptionTable;
typedef IPECertificateTable64       IPECertificateTable;
typedef IPERelocationTable64        IPERelocationTable;
typedef IPERelocationPage64         IPERelocationPage;
typedef IPERelocationItem64         IPERelocationItem;
typedef IPEDebugInfoTable64         IPEDebugInfoTable;
typedef IPEGlobalRegister64         IPEGlobalRegister;
typedef IPETlsTable64               IPETlsTable;
typedef IPEBoundImportTable64       IPEBoundImportTable;
typedef IPEImportAddressTable64     IPEImportAddressTable;
typedef IPEDelayImportTable64       IPEDelayImportTable;
typedef IPECLRHeader64              IPECLRHeader;

#endif

error_t ParsePEFromDiskFile(const file_char_t *pFilePath, IPEFile **ppFile);
error_t ParsePEFromMappedFile(void *pMemory, IPEFile **ppFile);

#ifdef LIBPE_WINOS
error_t ParsePEFromMappedResource(HMODULE hModule, IPEFile **ppFile);
error_t ParsePEFromLoadedModule(HMODULE hModule, IPEFile **ppFile);
#endif

LIBPE_NAMESPACE_END