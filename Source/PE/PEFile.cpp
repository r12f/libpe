#include "stdafx.h"
#include "PEFile.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
error_t
PEFileT<T>::ParsePEFromDiskFile(const file_char_t *pFilePath, IPEFileT<T> **ppFile)
{
    if(NULL == pFilePath || NULL == ppFile) {
        return ERR_POINTER;
    }

    PEFileT<T> *pRawFile = new PEFileT<T>();
    if(NULL == pRawFile) {
        return ERR_NO_MEM;
    }

    LibPEPtr<IPEFileT<T>> pFile = (IPEFileT<T> *)pRawFile;
    ScopedPtr<PEParserT<T>> pParser = PEParserT<T>::CreateForDiskFile(pFilePath, pRawFile);
    if(NULL == pParser) {
        return ERR_NO_MEM;
    }

    pRawFile->SetParser(pParser);

    return pFile.CopyTo(ppFile);
}

template <class T>
error_t
PEFileT<T>::ParsePEFromMappedFile(void *pMemory, IPEFileT<T> **ppFile)
{
    LIBPE_UNUSED_PARAM(pMemory);
    LIBPE_UNUSED_PARAM(ppFile);
    return ERR_NOT_IMPL;
}

#ifdef LIBPE_WINOS

template <class T>
error_t
PEFileT<T>::ParsePEFromMappedResource(HMODULE hModule, IPEFileT<T> **ppFile)
{
    LIBPE_UNUSED_PARAM(hModule);
    LIBPE_UNUSED_PARAM(ppFile);
    return ERR_NOT_IMPL;
}

template <class T>
error_t
PEFileT<T>::ParsePEFromLoadedModule(HMODULE hModule, IPEFileT<T> **ppFile)
{
    LIBPE_UNUSED_PARAM(hModule);
    LIBPE_UNUSED_PARAM(ppFile);
    return ERR_NOT_IMPL;
}

#endif

template <class T>
PEFileT<T>::PEFileT()
    : PEBase()
    , m_pDosHeader(NULL)
    , m_pFileHeader(NULL)
    , m_pNtHeader(NULL)
    , m_pOptionalHeader(NULL)
{

}

template <class T>
PEFileT<T>::~PEFileT()
{

}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEFile32, ParsePEFromDiskFile);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEFile32, ParsePEFromMappedFile);
#ifdef LIBPE_WINOS
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEFile32, ParsePEFromMappedResource);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEFile32, ParsePEFromLoadedModule);
#endif

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEFile64, ParsePEFromDiskFile);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEFile64, ParsePEFromMappedFile);
#ifdef LIBPE_WINOS
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEFile64, ParsePEFromMappedResource);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEFile64, ParsePEFromLoadedModule);
#endif

LIBPE_NAMESPACE_END