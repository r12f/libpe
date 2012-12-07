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

    LibPEPtr<DataLoader> pDataLoader = new DataLoaderDiskFile;
    LIBPE_ASSERT_RET(NULL != pDataLoader, NULL);

    DataLoaderDiskFile *pRawDataLoader = (DataLoaderDiskFile *)pDataLoader.p;
    LIBPE_ASSERT_RET(pRawDataLoader->LoadFile(pFilePath), NULL);

    PEParserT<T > *pDiskFileParser = PEParserT::Create(PE_PARSER_TYPE_DISK_FILE);
    LIBPE_ASSERT_RET(NULL != pDiskFileParser, NULL);

    PEParserDiskFileT<T > *pRawDiskFileParser = (PEParserDiskFileT<T > *)pDiskFileParser;

    pRawDiskFileParser->SetPEFile(pFile);
    pRawDiskFileParser->SetDataLoader(pDataLoader);

    return pDiskFileParser;
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

template <class T>
int8_t *
PEParserT<T>::GetRawMemory(uint64_t nOffset, uint64_t nSize)
{
    LIBPE_ASSERT_RET(NULL != m_pLoader, NULL);
    return m_pLoader->GetBuffer(nOffset, nSize);
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEParser);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParser, CreateForDiskFile);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParser, CreateForMappedFile);
#ifdef LIBPE_WINOS
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParser, CreateForMappedResource);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParser, CreateForLoadedModule);
#endif

LIBPE_NAMESPACE_END