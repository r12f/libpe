#pragma once

#include "LibPEBase.h"
#include "LibPEError.h"
#include "LibPEInterface.h"
#include "LibPEPtr.h"

LIBPE_NAMESPACE_BEGIN

void LIBPE_CALLTYPE SetPELoaderIOBlockSize(uint64_t nMinBlockSize, uint64_t nMaxBlockSize);

error_t ParsePEFromDiskFile(const file_char_t *pFilePath, IPEFile **ppFile);
error_t ParsePEFromMappedFile(void *pMemory, IPEFile **ppFile);

#ifdef LIBPE_WINOS
error_t ParsePEFromMappedResource(HMODULE hModule, IPEFile **ppFile);
error_t ParsePEFromLoadedModule(HMODULE hModule, IPEFile **ppFile);
#endif

LIBPE_NAMESPACE_END