#include "stdafx.h"
#include "PE/PEFile.h"

LIBPE_NAMESPACE_BEGIN

error_t
ParsePE32FromDiskFile(const file_char_t *pFilePath, IPEFile32 **ppFile)
{
    return PEFile32::ParsePEFromDiskFile(pFilePath, ppFile);
}

error_t
ParsePE32FromMappedFile(void *pMemory, IPEFile32 **ppFile)
{
    return PEFile32::ParsePEFromMappedFile(pMemory, ppFile);
}

#ifdef LIBPE_WINOS

error_t
ParsePE32FromMappedResource(HMODULE hModule, IPEFile32 **ppFile)
{
    return PEFile32::ParsePEFromMappedResource(hModule, ppFile);
}

error_t
ParsePE32FromLoadedModule(HMODULE hModule, IPEFile32 **ppFile)
{
    return PEFile32::ParsePEFromLoadedModule(hModule, ppFile);
}

#endif

LIBPE_NAMESPACE_END