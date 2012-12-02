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
    LIBPE_UNUSED_PARAM(pMemory);
    LIBPE_UNUSED_PARAM(ppFile);
    return ERR_NOT_IMPL;
}

#ifdef LIBPE_WINOS

error_t
ParsePE32FromMappedResource(HMODULE hModule, IPEFile32 **ppFile)
{
    LIBPE_UNUSED_PARAM(hModule);
    LIBPE_UNUSED_PARAM(ppFile);
    return ERR_NOT_IMPL;
}

error_t
ParsePE32FromLoadedModule(HMODULE hModule, IPEFile32 **ppFile)
{
    LIBPE_UNUSED_PARAM(hModule);
    LIBPE_UNUSED_PARAM(ppFile);
    return ERR_NOT_IMPL;
}

#endif

LIBPE_NAMESPACE_END