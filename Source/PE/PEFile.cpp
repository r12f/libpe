#include "stdafx.h"
#include "PE/PEFile.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
error_t
PEFileT<T>::ParsePEFromDiskFile(const file_char_t *pFilePath, IPEFileT<T> **ppFile)
{
    if(NULL == pFilePath || NULL == ppFile) {
        return ERR_POINTER;
    }

    *ppFile = NULL;

    PEFileT<T> *pRawFile = new PEFileT<T>();
    if(NULL == pRawFile) {
        return ERR_NO_MEM;
    }

    LibPEPtr<IPEFileT<T>> pFile = (IPEFileT<T> *)pRawFile;
    LibPEPtr<PEParserT<T>> pParser = PEParserT<T>::CreateForDiskFile(pFilePath, pRawFile);
    if(NULL == pParser) {
        return ERR_NO_MEM;
    }

    // We need the basic PE head and section info to perform nearly all of the operations.
    // So we parse them first.
    error_t nError = pParser->ParseBasicInfo();
    if(ERR_OK != nError) {
        return nError;
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
    : m_pDosHeader(NULL)
    , m_pFileHeader(NULL)
    , m_pNtHeaders(NULL)
    , m_pOptionalHeader(NULL)
{

}

template <class T>
LibPERawDosHeaderT(T) *
PEFileT<T>::GetDosHeader()
{
    LIBPE_ASSERT_RET(NULL != m_pDosHeader, NULL);
    return m_pDosHeader;
}

template <class T>
LibPERawNtHeadersT(T) *
PEFileT<T>::GetNtHeaders()
{
    LIBPE_ASSERT_RET(NULL != m_pNtHeaders, NULL);
    return m_pNtHeaders;
}

template <class T>
LibPERawFileHeaderT(T) *
PEFileT<T>::GetFileHeader()
{
    LIBPE_ASSERT_RET(NULL != m_pFileHeader, NULL);
    return m_pFileHeader;
}

template <class T>
LibPERawOptionalHeaderT(T) *
PEFileT<T>::GetOptionalHeader()
{
    LIBPE_ASSERT_RET(NULL != m_pOptionalHeader, NULL);
    return m_pOptionalHeader;
}

template <class T>
uint32_t
PEFileT<T>::GetSectionNum()
{
    return (uint32_t)m_vSectionHeaders.size();
}

template <class T>
error_t
PEFileT<T>::GetSectionHeader(uint32_t nIndex, IPESectionHeaderT<T> **ppSectionHeader)
{
    LIBPE_ASSERT_RET(NULL != ppSectionHeader, ERR_POINTER);
    LIBPE_ASSERT_RET(nIndex < GetSectionNum(), ERR_INVALID_ARG);
    return m_vSectionHeaders[nIndex].CopyTo(ppSectionHeader);
}

template <class T>
error_t
PEFileT<T>::GetSection(uint32_t nIndex, IPESectionT<T> **ppSection)
{
    LIBPE_ASSERT_RET(NULL != ppSection, ERR_POINTER);
    LIBPE_ASSERT_RET(nIndex < GetSectionNum(), ERR_INVALID_ARG);
    LIBPE_ASSERT_RET(NULL != m_vSectionHeaders[nIndex], ERR_FAIL);
    return m_vSectionHeaders[nIndex]->GetSection(ppSection);
}

template <class T>
LibPEAddressT(T)
PEFileT<T>::GetRVAFromVA(LibPEAddressT(T) nVA)
{
    LIBPE_ASSERT_RET(NULL != m_pParser, 0);
    return m_pParser->GetRVAFromVA(nVA);
}

template <class T>
LibPEAddressT(T)
PEFileT<T>::GetVAFromRVA(LibPEAddressT(T) nRVA)
{
    LIBPE_ASSERT_RET(NULL != m_pParser, 0);
    return m_pParser->GetVAFromRVA(nRVA);
}

template <class T>
LibPEAddressT(T)
PEFileT<T>::GetRVAFromFOA(LibPEAddressT(T) nFOA)
{
    LIBPE_ASSERT_RET(NULL != m_pParser, 0);
    return m_pParser->GetRVAFromFOA(nFOA);
}

template <class T>
LibPEAddressT(T)
PEFileT<T>::GetFOAFromRVA(LibPEAddressT(T) nRVA)
{
    LIBPE_ASSERT_RET(NULL != m_pParser, 0);
    return m_pParser->GetFOAFromRVA(nRVA);
}

template <class T>
LibPEAddressT(T)
PEFileT<T>::GetVAFromFOA(LibPEAddressT(T) nFOA)
{
    LIBPE_ASSERT_RET(NULL != m_pParser, 0);
    return m_pParser->GetVAFromFOA(nFOA);
}

template <class T>
LibPEAddressT(T)
PEFileT<T>::GetFOAFromVA(LibPEAddressT(T) nVA)
{
    LIBPE_ASSERT_RET(NULL != m_pParser, 0);
    return m_pParser->GetFOAFromVA(nVA);
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEFile);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEFile, ParsePEFromDiskFile);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEFile, ParsePEFromMappedFile);
#ifdef LIBPE_WINOS
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEFile, ParsePEFromMappedResource);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEFile, ParsePEFromLoadedModule);
#endif

LIBPE_NAMESPACE_END