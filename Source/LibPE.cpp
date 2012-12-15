#include "stdafx.h"

LIBPE_NAMESPACE_BEGIN

#ifndef _WIN64

error_t
ParsePEFromDiskFile(const file_char_t *pFilePath, IPEFile **ppFile)
{
    return ParsePE32FromDiskFile(pFilePath, ppFile);
}

error_t
ParsePEFromMappedFile(void *pMemory, IPEFile **ppFile)
{
    return ParsePE32FromMappedFile(pMemory, ppFile);
}

#ifdef LIBPE_WINOS

error_t
ParsePEFromMappedResource(HMODULE hModule, IPEFile **ppFile)
{
    return ParsePE32FromMappedResource(hModule, ppFile);
}

error_t
ParsePEFromLoadedModule(HMODULE hModule, IPEFile **ppFile)
{
    return ParsePE32FromLoadedModule(hModule, ppFile);
}

#endif

#else

error_t
ParsePEFromDiskFile(const file_char_t *pFilePath, IPEFile **ppFile)
{
    return ParsePE64FromDiskFile(pFilePath, ppFile);
}

error_t
ParsePEFromMappedFile(void *pMemory, IPEFile **ppFile)
{
    return ParsePE64FromMappedFile(pMemory, ppFile);
}

#ifdef LIBPE_WINOS

error_t
ParsePEFromMappedResource(HMODULE hModule, IPEFile **ppFile)
{
    return ParsePE64FromMappedResource(hModule, ppFile);
}

error_t
ParsePEFromLoadedModule(HMODULE hModule, IPEFile **ppFile)
{
    return ParsePE64FromLoadedModule(hModule, ppFile);
}

#endif

#endif

LIBPE_NAMESPACE_END