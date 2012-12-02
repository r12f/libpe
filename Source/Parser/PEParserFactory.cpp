#include "stdafx.h"
#include "Parser/PEParserFactory.h"
#include "Parser/PEParserImpl.h"
#include "Parser/DataLoader.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
PEParserT<T> *
PEParserFactoryT<T>::CreateForDiskFile(const file_char_t *pFilePath, PEFileT<T> *pFile)
{
    LIBPE_ASSERT_RET(NULL != pFilePath && NULL != pFile, ERR_POINTER);

    ScopedPtr<DataLoader> pDataLoader = new DataLoaderDiskFile;
    LIBPE_ASSERT_RET(NULL != pDataLoader, NULL);

    DataLoaderDiskFile *pRawDataLoader = (DataLoaderDiskFile *)pDataLoader;
    LIBPE_ASSERT_RET(ERR_OK == pRawDataLoader->LoadFile(pFilePath), NULL);

    ScopedPtr<PEParserDiskFileT<T>> pDiskFile = new PEParserDiskFileT<T>;
    LIBPE_ASSERT_RET(NULL != pDiskFile, NULL);

    pDiskFile->SetPEFile(pFile);
    pDiskFile->SetDataLoader(pDataLoader.Detach());

    return pDiskFile.Detach();
}

template <class T>
PEParserT<T> *
PEParserFactoryT<T>::CreateForMappedFile(void *pMemory, PEFileT<T> *pFile)
{
    return NULL;
}

#ifdef LIBPE_WINOS

template <class T>
PEParserT<T> *
PEParserFactoryT<T>::CreateForMappedResource(HMODULE hModule, PEFileT<T> *pFile)
{
    return NULL;
}

template <class T>
PEParserT<T> *
PEParserFactoryT<T>::CreateForLoadedModule(HMODULE hModule, PEFileT<T> *pFile)
{
    return NULL;
}

#endif

LIBPE_NAMESPACE_END