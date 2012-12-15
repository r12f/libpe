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

LIBPE_NAMESPACE_END