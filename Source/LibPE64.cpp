#include "stdafx.h"

LIBPE_NAMESPACE_BEGIN

error_t
ParsePE64FromDiskFile(const file_char_t *pFilePath, IPEFile64 **ppFile)
{
    LIBPE_UNUSED_PARAM(pFilePath);
    LIBPE_UNUSED_PARAM(ppFile);
    return ERR_NOT_IMPL;
}

error_t
ParsePE64FromMappedFile(void *pMemory, IPEFile64 **ppFile)
{
    LIBPE_UNUSED_PARAM(pMemory);
    LIBPE_UNUSED_PARAM(ppFile);
    return ERR_NOT_IMPL;
}

#ifdef LIBPE_WINOS

error_t
ParsePE64FromMappedResource(HMODULE hModule, IPEFile64 **ppFile)
{
    LIBPE_UNUSED_PARAM(hModule);
    LIBPE_UNUSED_PARAM(ppFile);
    return ERR_NOT_IMPL;
}

error_t
ParsePE64FromLoadedModule(HMODULE hModule, IPEFile64 **ppFile)
{
    LIBPE_UNUSED_PARAM(hModule);
    LIBPE_UNUSED_PARAM(ppFile);
    return ERR_NOT_IMPL;
}

#endif

LIBPE_NAMESPACE_END