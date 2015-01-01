#pragma once

#include "LibPEBase.h"
#include "LibPEError.h"
#include "LibPEInterface.h"
#include "LibPEPtr.h"

LIBPE_NAMESPACE_BEGIN

#if defined(LIBPE_WINOS) && defined(LIBPE_DLL)
#ifdef LIBPEDLL_EXPORTS
#define LIBPE_API           __declspec(dllexport) LIBPE_CALLTYPE
#else
#define LIBPE_API           __declspec(dllimport) LIBPE_CALLTYPE
#endif
#else
#define LIBPE_API           LIBPE_CALLTYPE
#endif

void LIBPE_API SetPELoaderIOBlockSize(UINT64 nMinBlockSize, UINT64 nMaxBlockSize);

HRESULT LIBPE_API ParsePEFromDiskFile(const file_char_t *pFilePath, IPEFile **ppFile);
HRESULT LIBPE_API ParsePEFromMappedFile(void *pMemory, IPEFile **ppFile);

#ifdef LIBPE_WINOS
HRESULT LIBPE_API ParsePEFromMappedResource(HMODULE hModule, IPEFile **ppFile);
HRESULT LIBPE_API ParsePEFromLoadedModule(HMODULE hModule, IPEFile **ppFile);
#endif

LIBPE_NAMESPACE_END