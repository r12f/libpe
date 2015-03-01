#include "stdafx.h"
#include "Parser/DataLoader.h"
#include "PE/PEFile.h"

LIBPE_NAMESPACE_BEGIN

static HRESULT
ParsePEFromDataLoader(DataLoader *pDataLoader, IPEFile **ppFile)
{
    LIBPE_CHK(NULL != pDataLoader && NULL != ppFile, E_POINTER);

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
    LIBPE_CHK(NULL != pDataLoader, E_OUTOFMEMORY);

    DataLoaderDiskFile *pRawDataLoader = (DataLoaderDiskFile *)pDataLoader.p;
    LIBPE_CHK(pRawDataLoader->LoadFile(pFilePath), E_ACCESSDENIED);

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