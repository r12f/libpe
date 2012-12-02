#include "stdafx.h"
#include "Parser/PEParser.h"
#include "Parser/PEParserImpl.h"
#include "PE/PEFile.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
PEParserT<T> *
PEParserT<T>::CreateForDiskFile(const file_char_t *pFilePath, PEFileT<T> *pFile)
{
    LIBPE_ASSERT_RET(NULL != pFilePath && NULL != pFile, NULL);

    ScopedPtr<DataLoader> pDataLoader = new DataLoaderDiskFile;
    LIBPE_ASSERT_RET(NULL != pDataLoader, NULL);

    DataLoaderDiskFile *pRawDataLoader = (DataLoaderDiskFile *)pDataLoader.m_ptr;
    LIBPE_ASSERT_RET(ERR_OK == pRawDataLoader->LoadFile(pFilePath), NULL);

    ScopedPtr<PEParserT<T > > pDiskFile = PEParserT::Create(PE_PARSER_TYPE_DISK_FILE);
    LIBPE_ASSERT_RET(NULL != pDiskFile, NULL);

    PEParserDiskFileT<T > *pRawDiskFile = (PEParserDiskFileT<T > *)pDiskFile.m_ptr;

    pRawDiskFile->SetPEFile(pFile);
    pRawDiskFile->SetDataLoader(pDataLoader.Detach());

    return pDiskFile.Detach();
}

template <class T>
PEParserT<T> *
PEParserT<T>::CreateForMappedFile(void *pMemory, PEFileT<T> *pFile)
{
    return NULL;
}

#ifdef LIBPE_WINOS

template <class T>
PEParserT<T> *
PEParserT<T>::CreateForMappedResource(HMODULE hModule, PEFileT<T> *pFile)
{
    return NULL;
}

template <class T>
PEParserT<T> *
PEParserT<T>::CreateForLoadedModule(HMODULE hModule, PEFileT<T> *pFile)
{
    return NULL;
}

#endif

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParser32, CreateForDiskFile);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParser32, CreateForMappedFile);
#ifdef LIBPE_WINOS
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParser32, CreateForMappedResource);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParser32, CreateForLoadedModule);
#endif

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParser64, CreateForDiskFile);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParser64, CreateForMappedFile);
#ifdef LIBPE_WINOS
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParser64, CreateForMappedResource);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParser64, CreateForLoadedModule);
#endif

LIBPE_NAMESPACE_END