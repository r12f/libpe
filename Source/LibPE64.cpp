#include "stdafx.h"
#include "PE/PEFile.h"

LIBPE_NAMESPACE_BEGIN

error_t
ParsePE64FromDiskFile(const file_char_t *pFilePath, IPEFile64 **ppFile)
{
    return PEFile64::ParsePEFromDiskFile(pFilePath, ppFile);
}

error_t
ParsePE64FromMappedFile(void *pMemory, IPEFile64 **ppFile)
{
    return PEFile64::ParsePEFromMappedFile(pMemory, ppFile);
}

#ifdef LIBPE_WINOS

error_t
ParsePE64FromMappedResource(HMODULE hModule, IPEFile64 **ppFile)
{
    return PEFile64::ParsePEFromMappedResource(hModule, ppFile);
}

error_t
ParsePE64FromLoadedModule(HMODULE hModule, IPEFile64 **ppFile)
{
    return PEFile64::ParsePEFromLoadedModule(hModule, ppFile);
}

#endif

#ifdef _WIN64

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