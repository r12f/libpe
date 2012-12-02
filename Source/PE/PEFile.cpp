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

    PEFileT<T> *pRawFile = new PEFileT<T>();
    if(NULL == pRawFile) {
        return ERR_NO_MEM;
    }

    LibPEPtr<IPEFileT<T>> pFile = (IPEFileT<T> *)pRawFile;
    ScopedPtr<PEParserT<T>> pParser = PEParserT<T>::CreateForDiskFile(pFilePath, pRawFile);
    if(NULL == pParser) {
        return ERR_NO_MEM;
    }

    pRawFile->SetParser(pParser.Detach());

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
PEFileT<T>::~PEFileT()
{

}

template <class T>
int8_t *
PEFileT<T>::GetRawMemory(uint64_t nOffset, uint64_t nSize)
{
    LIBPE_ASSERT_RET(NULL != m_pParser, NULL);
    return m_pParser->GetRawMemory(nOffset, nSize);
}

template <class T>
PEDosHeaderT<T> *
PEFileT<T>::GetDosHeader()
{
    if(NULL == m_pDosHeader) {
        LIBPE_ASSERT_RET(NULL != m_pParser, NULL);
        m_pParser->ParsePEBasicInfo();
    }
    LIBPE_ASSERT_RET(NULL != m_pDosHeader, NULL);
    return m_pDosHeader;
}

template <class T>
PENtHeadersT<T> *
PEFileT<T>::GetNtHeaders()
{
    if(NULL == m_pNtHeaders) {
        LIBPE_ASSERT_RET(NULL != m_pParser, NULL);
        m_pParser->ParsePEBasicInfo();
    }
    LIBPE_ASSERT_RET(NULL != m_pNtHeaders, NULL);
    return m_pNtHeaders;
}

template <class T>
PEFileHeaderT<T> *
PEFileT<T>::GetFileHeader()
{
    if(NULL == m_pFileHeader) {
        LIBPE_ASSERT_RET(NULL != m_pParser, NULL);
        m_pParser->ParsePEBasicInfo();
    }
    LIBPE_ASSERT_RET(NULL != m_pFileHeader, NULL);
    return m_pFileHeader;
}

template <class T>
PEOptionalHeaderT<T> *
PEFileT<T>::GetOptionalHeader()
{
    if(NULL == m_pOptionalHeader) {
        LIBPE_ASSERT_RET(NULL != m_pParser, NULL);
        m_pParser->ParsePEBasicInfo();
    }
    LIBPE_ASSERT_RET(NULL != m_pOptionalHeader, NULL);
    return m_pOptionalHeader;
}

template <class T>
uint32_t
PEFileT<T>::GetSectionNum()
{
    uint32_t nSectionNum = (uint32_t)m_vSections.size();
    if(0 == nSectionNum) {
        LIBPE_ASSERT_RET(NULL != m_pParser, 0);
        m_pParser->ParsePESection();
    }

    return (uint32_t)m_vSections.size();
}

template <class T>
error_t
PEFileT<T>::GetSection(uint32_t nIndex, IPESectionT<T> **ppSection)
{
    LIBPE_ASSERT_RET(NULL != ppSection, ERR_POINTER);
    LIBPE_ASSERT_RET(nIndex < GetSectionNum(), ERR_INVALID_ARG);
    return m_vSections[nIndex].CopyTo(ppSection);
}

template <class T>
PEAddressT<T>
PEFileT<T>::GetRVAFromVA(PEAddressT<T> nVA)
{
    PEOptionalHeaderT<T> *pOptinalHeader = GetOptionalHeader();
    LIBPE_ASSERT_RET(NULL != pOptinalHeader, NULL);
    if(nVA < pOptinalHeader->ImageBase) {
        return 0;
    }
    return nVA - pOptinalHeader->ImageBase;
}

template <class T>
PEAddressT<T>
PEFileT<T>::GetRVAFromFOA(PEAddressT<T> nFOA)
{
    return 0;
}

template <class T>
PEAddressT<T>
PEFileT<T>::GetVAFromRVA(PEAddressT<T> nRVA)
{
    PEOptionalHeaderT<T> *pOptinalHeader = GetOptionalHeader();
    LIBPE_ASSERT_RET(NULL != pOptinalHeader, NULL);
    return pOptinalHeader->ImageBase + nRVA;
}

template <class T>
PEAddressT<T>
PEFileT<T>::GetVAFromFOA(PEAddressT<T> nFOA)
{
    return 0;
}

template <class T>
PEAddressT<T>
PEFileT<T>::GetFOAFromRVA(PEAddressT<T> nRVA)
{
    return 0;
}

template <class T>
PEAddressT<T>
PEFileT<T>::GetFOAFromVA(PEAddressT<T> nVA)
{
    return 0;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEFile);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEFile, ParsePEFromDiskFile);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEFile, ParsePEFromMappedFile);
#ifdef LIBPE_WINOS
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEFile, ParsePEFromMappedResource);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEFile, ParsePEFromLoadedModule);
#endif

LIBPE_NAMESPACE_END