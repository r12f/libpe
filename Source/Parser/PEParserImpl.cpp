#include "stdafx.h"
#include "Parser/PEParserImpl.h"
#include "PE/PEFile.h"
#include "PE/PESectionHeader.h"
#include "PE/PESection.h"

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
PEParserDiskFileT<T>::ParseBasicInfo()
{
    if(m_bIsBasicInfoParsed) {
        return ERR_OK;
    }

    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, ERR_FAIL);

    LibPERawDosHeaderT(T) *pDosHeader = (LibPERawDosHeaderT(T) *)m_pLoader->GetBuffer(0, sizeof(LibPERawDosHeaderT(T)));
    LIBPE_ASSERT_RET(NULL != pDosHeader, ERR_NO_MEM);
    LIBPE_ASSERT_RET(IMAGE_DOS_SIGNATURE == pDosHeader->e_magic, ERR_FAIL);

    LibPERawNtHeadersT(T) *pNtHeaders = (LibPERawNtHeadersT(T) *)m_pLoader->GetBuffer(pDosHeader->e_lfanew, sizeof(LibPERawNtHeadersT(T)));
    LIBPE_ASSERT_RET(NULL != pNtHeaders, ERR_NO_MEM);
    LIBPE_ASSERT_RET(IMAGE_NT_SIGNATURE == pNtHeaders->Signature, ERR_FAIL);

    m_pFile->SetDosHeader(pDosHeader);
    m_pFile->SetNtHeaders(pNtHeaders);
    m_pFile->SetFileHeader(&(pNtHeaders->FileHeader));
    m_pFile->SetOptionalHeader(&(pNtHeaders->OptionalHeader));

    uint32_t nSectionHeaderOffset = 0;
    uint32_t nStartSectionHeaderOffset = pDosHeader->e_lfanew + sizeof(DWORD) + sizeof(LibPERawFileHeaderT(T)) + pNtHeaders->FileHeader.SizeOfOptionalHeader;
    LibPERawSectionHeaderT(T) *pSectionHeader = NULL;
    for(uint16_t nSectionId = 0; nSectionId < pNtHeaders->FileHeader.NumberOfSections; ++nSectionId) {
        nSectionHeaderOffset = nStartSectionHeaderOffset + nSectionId * sizeof(LibPERawSectionHeaderT(T));
        pSectionHeader = (LibPERawSectionHeaderT(T) *)m_pLoader->GetBuffer(nSectionHeaderOffset, sizeof(LibPERawSectionHeaderT(T)));
        if(NULL == pSectionHeader) {
            return ERR_FAIL;
        }

        LibPEPtr<PESectionHeaderT<T>> pRawSectionHeader = new PESectionHeaderT<T>();
        LibPEPtr<PESectionT<T>> pRawSection = new PESectionT<T>();
        if(NULL == pRawSectionHeader || NULL == pRawSection) {
            return ERR_NO_MEM;
        }

        pRawSectionHeader->SetParser(this);
        pRawSectionHeader->SetPEFile(m_pFile);
        pRawSectionHeader->SetRVA(nSectionHeaderOffset);
        pRawSectionHeader->SetSizeInMemory(sizeof(LibPERawOptionalHeaderT(T)));
        pRawSectionHeader->SetFOA(nSectionHeaderOffset);
        pRawSectionHeader->SetSizeInFile(sizeof(LibPERawOptionalHeaderT(T)));
        pRawSectionHeader->SetRawSectionHeader(pSectionHeader);

        m_pFile->AddSectionHeader(pRawSectionHeader);
    }

    m_bIsBasicInfoParsed = true;

    return ERR_OK;
}

template <class T>
error_t
PEParserDiskFileT<T>::ParseSection(LibPERawSectionHeaderT(T) *pSectionHeader, IPESectionT<T> **ppSection)
{
    /*
    if(m_bIsSectionParsed) {
        return ERR_OK;
    }

    LIBPE_ASSERT_RET(NULL != m_pLoader && NULL != m_pFile, ERR_FAIL);

    LibPERawDosHeaderT(T) *pDosHeader = m_pFile->GetDosHeader();
    LibPERawFileHeaderT(T) *pFileHeader = m_pFile->GetFileHeader();
    if(NULL == pDosHeader || NULL == pFileHeader) {
        return ERR_FAIL;
    }


    m_bIsSectionParsed = true;

        pRawSection->SetParser(this);
        pRawSection->SetPEFile(m_pFile);
        pRawSection->SetRVA(pSectionHeader->VirtualAddress);
        pRawSection->SetSizeInMemory(pSectionHeader->SizeOfRawData);
        pRawSection->SetFOA(pSectionHeader->PointerToRawData);
        pRawSection->SetSizeInFile(pSectionHeader->SizeOfRawData);
        pRawSection->SetRawSectionHeader(pSectionHeader);
*/
    return ERR_OK;
}

template <class T>
LibPEAddressT(T)
PEParserDiskFileT<T>::GetAddressFromRVA(LibPEAddressT(T) nRVA)
{
    return nRVA;
}

template <class T>
LibPEAddressT(T)
PEParserDiskFileT<T>::GetAddressFromVA(LibPEAddressT(T) nVA)
{
    return nVA;
}

template <class T>
LibPEAddressT(T)
PEParserDiskFileT<T>::GetAddressFromFOA(LibPEAddressT(T) nFOA)
{
    return nFOA;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEParserDiskFile);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParser, Create);

LIBPE_NAMESPACE_END