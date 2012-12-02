#include "stdafx.h"
#include "Parser/PEParserImpl.h"
#include "PE/PEFile.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
PEParserT<T> *
PEParserT<T>::Create(PEParserType nType)
{
    switch(nType) {
    case PE_PARSER_TYPE_DISK_FILE:
        return new PEParserDiskFileT<T>;
    }

    return NULL;
}

template <class T>
error_t
PEParserDiskFileT<T>::ParsePEBasicInfo()
{
    if(m_bIsBasicInfoParsed) {
        return ERR_OK;
    }

    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, ERR_FAIL);

    PEDosHeaderT<T> *pDosHeader = (PEDosHeaderT<T> *)m_pLoader->GetBuffer(0, sizeof(PEDosHeaderT<T>));
    LIBPE_ASSERT_RET(NULL != pDosHeader, ERR_NO_MEM);
    LIBPE_ASSERT_RET(IMAGE_DOS_SIGNATURE == pDosHeader->e_magic, ERR_FAIL);

    PENtHeadersT<T> *pNtHeaders = (PENtHeadersT<T> *)m_pLoader->GetBuffer(pDosHeader->e_lfanew, sizeof(PENtHeadersT<T>));
    LIBPE_ASSERT_RET(NULL != pNtHeaders, ERR_NO_MEM);
    LIBPE_ASSERT_RET(IMAGE_NT_SIGNATURE == pNtHeaders->Signature, ERR_FAIL);

    m_pFile->SetPEDosHeader(pDosHeader);
    m_pFile->SetPENtHeaders(pNtHeaders);
    m_pFile->SetPEFileHeader((PEFileHeaderT<T> *)&(pNtHeaders->FileHeader));
    m_pFile->SetPEOptionalHeader((PEOptionalHeaderT<T> *)&(pNtHeaders->OptionalHeader));

    m_bIsBasicInfoParsed = true;

    return ERR_OK;
}

template <class T>
error_t
PEParserDiskFileT<T>::ParsePESection()
{
    if(m_bIsSectionParsed) {
        return ERR_OK;
    }

    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, ERR_FAIL);

    PEFileHeaderT<T> *pFileHeader = m_pFile->GetFileHeader();
    if(NULL == pFileHeader) {
        return ERR_FAIL;
    }

    for(uint16_t nSectionId = 0; nSectionId < pFileHeader->NumberOfSections; ++nSectionId) {
        m_pFile->AddPESection(NULL);
    }

    m_bIsSectionParsed = true;

    return ERR_OK;
}

template <class T>
PEAddressT<T>
PEParserDiskFileT<T>::GetAddressFromRVA(PEAddressT<T> nRVA)
{
    return nRVA;
}

template <class T>
PEAddressT<T>
PEParserDiskFileT<T>::GetAddressFromVA(PEAddressT<T> nVA)
{
    return nVA;
}

template <class T>
PEAddressT<T>
PEParserDiskFileT<T>::GetAddressFromFOA(PEAddressT<T> nFOA)
{
    return nFOA;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEParserDiskFile);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParser, Create);

LIBPE_NAMESPACE_END