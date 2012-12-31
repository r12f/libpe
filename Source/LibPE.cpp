#include "stdafx.h"
#include "LibPE.h"
#include "Parser/DataLoader.h"
#include "PE/PEFile.h"

LIBPE_NAMESPACE_BEGIN

error_t
ParsePEFromDiskFile(const file_char_t *pFilePath, IPEFile **ppFile)
{
    LibPEPtr<DataLoader> pDataLoader = new DataLoaderDiskFile;
    LIBPE_ASSERT_RET(NULL != pDataLoader, NULL);

    DataLoaderDiskFile *pRawDataLoader = (DataLoaderDiskFile *)pDataLoader.p;
    LIBPE_ASSERT_RET(pRawDataLoader->LoadFile(pFilePath), NULL);

    if(ERR_OK == PEFile32::Create(pDataLoader, ppFile) && NULL != *ppFile) {
        return ERR_OK;
    }

    if(ERR_OK == PEFile64::Create(pDataLoader, ppFile) && NULL != *ppFile) {
        return ERR_OK;
    }

    return ERR_FAIL;
}

error_t
ParsePEFromMappedFile(void *pMemory, IPEFile **ppFile)
{
    return ERR_NOT_IMPL;
}

#ifdef LIBPE_WINOS
error_t
ParsePEFromMappedResource(HMODULE hModule, IPEFile **ppFile)
{
    return ERR_NOT_IMPL;
}

error_t
ParsePEFromLoadedModule(HMODULE hModule, IPEFile **ppFile)
{
    return ERR_NOT_IMPL;
}
#endif

LIBPE_NAMESPACE_END