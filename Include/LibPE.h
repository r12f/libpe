#pragma once

#include "LibPEBase.h"
#include "LibPEError.h"
#include "LibPEInterface.h"
#include "LibPEPtr.h"

LIBPE_NAMESPACE_BEGIN

void LIBPE_CALLTYPE SetPELoaderIOBlockSize(UINT64 nMinBlockSize, UINT64 nMaxBlockSize);

HRESULT ParsePEFromDiskFile(const file_char_t *pFilePath, IPEFile **ppFile);
HRESULT ParsePEFromMappedFile(void *pMemory, IPEFile **ppFile);

#ifdef LIBPE_WINOS
HRESULT ParsePEFromMappedResource(HMODULE hModule, IPEFile **ppFile);
HRESULT ParsePEFromLoadedModule(HMODULE hModule, IPEFile **ppFile);
#endif

LIBPE_NAMESPACE_END