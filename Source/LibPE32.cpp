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

#endif

LIBPE_NAMESPACE_END