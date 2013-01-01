#include "stdafx.h"
#include "Parser/DataLoader.h"
#include "PE/PEFile.h"

LIBPE_NAMESPACE_BEGIN

static HRESULT
ParsePEFromDataLoader(DataLoader *pDataLoader, IPEFile **ppFile)
{
    LIBPE_ASSERT_RET(NULL != pDataLoader && NULL != ppFile, E_POINTER);

    if(SUCCEEDED(PEFile32::Create(pDataLoader, ppFile)) && NULL != *ppFile) {
        return S_OK;
    }

    if(SUCCEEDED(PEFile64::Create(pDataLoader, ppFile)) && NULL != *ppFile) {
        return S_OK;
    }

    return E_FAIL;
}


HRESULT LIBPE_API
ParsePEFromDiskFile(const file_char_t *pFilePath, IPEFile **ppFile)
{
    LibPEPtr<DataLoader> pDataLoader = new DataLoaderDiskFile;
    LIBPE_ASSERT_RET(NULL != pDataLoader, NULL);

    DataLoaderDiskFile *pRawDataLoader = (DataLoaderDiskFile *)pDataLoader.p;
    LIBPE_ASSERT_RET(pRawDataLoader->LoadFile(pFilePath), NULL);

    return ParsePEFromDataLoader(pDataLoader, ppFile);
}

HRESULT LIBPE_API
ParsePEFromMappedFile(void *pMemory, IPEFile **ppFile)
{
    return E_NOTIMPL;
}

#ifdef LIBPE_WINOS
HRESULT LIBPE_API
ParsePEFromMappedResource(HMODULE hModule, IPEFile **ppFile)
{
    return E_NOTIMPL;
}

HRESULT LIBPE_API
ParsePEFromLoadedModule(HMODULE hModule, IPEFile **ppFile)
{
    return E_NOTIMPL;
}
#endif

LIBPE_NAMESPACE_END