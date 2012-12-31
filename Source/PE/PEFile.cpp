#include "stdafx.h"
#include "PE/PEFile.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
error_t
PEFileT<T>::Create(DataLoader *pLoader, IPEFile **ppFile)
{
    LIBPE_ASSERT_RET(NULL != pLoader && NULL != ppFile, ERR_POINTER);

    *ppFile = NULL;

    LibPEPtr<PEFileT<T>> pInnerFile = new PEFileT<T>();
    LibPEPtr<PEParserT<T>> pParser = PEParserT<T>::Create(pLoader->GetType());
    LIBPE_ASSERT_RET(NULL != pInnerFile && NULL != pParser, ERR_NO_MEM);

    pParser->SetPEFile(pInnerFile);
    pParser->SetDataLoader(pLoader);

    pInnerFile->Init(pParser);

    *ppFile = pInnerFile.Detach();

    return ERR_OK;
}

template <class T>
PEFileT<T>::PEFileT()
    : m_pDosHeader(NULL)
    , m_pFileHeader(NULL)
    , m_pNtHeaders(NULL)
    , m_pOptionalHeader(NULL)
{

}

template <class T>
PERawDosHeader *
PEFileT<T>::GetDosHeader()
{
    LIBPE_ASSERT_RET(NULL != m_pDosHeader, NULL);
    return m_pDosHeader;
}

template <class T>
bool_t
PEFileT<T>::IsDosFile()
{
    LIBPE_ASSERT_RET(NULL != m_pDosHeader, false);
    return (NULL == m_pDosHeader->e_lfanew);
}

template <class T>
void *
PEFileT<T>::GetNtHeaders()
{
    LIBPE_ASSERT_RET(NULL != m_pNtHeaders, NULL);
    return m_pNtHeaders;
}

template <class T>
PERawNtHeaders32 *
PEFileT<T>::GetNtHeaders32()
{
    LIBPE_ASSERT_RET(NULL != m_pNtHeaders, NULL);
    return (PERawNtHeaders32 *)(Is32Bit() ? m_pNtHeaders : NULL);
}

template <class T>
PERawNtHeaders64 *
PEFileT<T>::GetNtHeaders64()
{
    LIBPE_ASSERT_RET(NULL != m_pNtHeaders, NULL);
    return (PERawNtHeaders64 *)(Is32Bit() ? NULL : m_pNtHeaders);
}

template <class T>
PERawFileHeader *
PEFileT<T>::GetFileHeader()
{
    LIBPE_ASSERT_RET(NULL != m_pFileHeader, NULL);
    return m_pFileHeader;
}

template <class T>
void *
PEFileT<T>::GetOptionalHeader()
{
    LIBPE_ASSERT_RET(NULL != m_pOptionalHeader, NULL);
    return m_pOptionalHeader;
}

template <class T>
PERawOptionalHeader32 *
PEFileT<T>::GetOptionalHeader32()
{
    LIBPE_ASSERT_RET(NULL != m_pOptionalHeader, NULL);
    return (PERawOptionalHeader32 *)(Is32Bit() ? m_pOptionalHeader : NULL);
}

template <class T>
PERawOptionalHeader64 *
PEFileT<T>::GetOptionalHeader64()
{
    LIBPE_ASSERT_RET(NULL != m_pOptionalHeader, NULL);
    return (PERawOptionalHeader64 *)(Is32Bit() ? m_pOptionalHeader : NULL);
}

template <class T>
bool_t
PEFileT<T>::Is32Bit()
{
    return PETrait<T>::Is32Bit;
}

template <class T>
PEAddress
PEFileT<T>::GetImageBase()
{
    LIBPE_ASSERT_RET(NULL != m_pOptionalHeader, NULL);
    return ((LibPERawOptionalHeaderT(T) *)m_pOptionalHeader)->ImageBase;
}

template <class T>
uint32_t 
PEFileT<T>::GetImageSize()
{
    LIBPE_ASSERT_RET(NULL != m_pOptionalHeader, NULL);
    return ((LibPERawOptionalHeaderT(T) *)m_pOptionalHeader)->SizeOfImage;
}

template <class T>
uint32_t 
PEFileT<T>::GetEntryPoint()
{
    LIBPE_ASSERT_RET(NULL != m_pOptionalHeader, NULL);
    return ((LibPERawOptionalHeaderT(T) *)m_pOptionalHeader)->AddressOfEntryPoint;
}

template <class T>
uint32_t
PEFileT<T>::GetSectionCount()
{
    return (uint32_t)m_vSectionHeaders.size();
}

template <class T>
error_t
PEFileT<T>::GetSectionHeader(uint32_t nIndex, IPESectionHeader **ppSectionHeader)
{
    LIBPE_ASSERT_RET(NULL != ppSectionHeader, ERR_POINTER);
    LIBPE_ASSERT_RET(nIndex < GetSectionCount(), ERR_INVALID_ARG);
    return m_vSectionHeaders[nIndex].CopyTo(ppSectionHeader);
}

template <class T>
error_t
PEFileT<T>::GetSection(uint32_t nIndex, IPESection **ppSection)
{
    LIBPE_ASSERT_RET(NULL != ppSection, ERR_POINTER);
    LIBPE_ASSERT_RET(nIndex < GetSectionCount(), ERR_INVALID_ARG);
    LIBPE_ASSERT_RET(NULL != m_vSectionHeaders[nIndex], ERR_FAIL);
    return m_vSectionHeaders[nIndex]->GetSection(ppSection);
}

template <class T>
error_t
PEFileT<T>::GetSectionByRVA(PEAddress nRVA, IPESection **ppSection)
{
    LIBPE_ASSERT_RET(NULL != ppSection, ERR_POINTER);

    uint32_t nSectionCount = GetSectionCount();
    for(uint32_t nSectionIndex = 0; nSectionIndex < nSectionCount; ++nSectionIndex) {
        LibPEPtr<IPESection> pSection;
        if(ERR_OK == GetSection(nSectionIndex, &pSection) && NULL != pSection) {
            if(pSection->GetRVA() <= nRVA && nRVA <= pSection->GetRVA() + pSection->GetSizeInMemory()) {
                *ppSection = pSection.Detach();
                return ERR_OK;
            }
        }
    }

    return ERR_FAIL;
}

template <class T>
error_t
PEFileT<T>::GetSectionByVA(PEAddress nVA, IPESection **ppSection)
{
    return GetSectionByRVA(GetRVAFromVA(nVA), ppSection);
}

template <class T>
error_t
PEFileT<T>::GetSectionByFOA(PEAddress nFOA, IPESection **ppSection)
{
    LIBPE_ASSERT_RET(NULL != ppSection, ERR_POINTER);

    uint32_t nSectionCount = GetSectionCount();
    for(uint32_t nSectionIndex = 0; nSectionIndex < nSectionCount; ++nSectionIndex) {
        LibPEPtr<IPESection> pSection;
        if(ERR_OK == GetSection(nSectionIndex, &pSection) && NULL != pSection) {
            if(pSection->GetFOA() <= nFOA && nFOA <= pSection->GetFOA() + pSection->GetSizeInMemory()) {
                *ppSection = pSection.Detach();
                return ERR_OK;
            }
        }
    }

    return ERR_FAIL;
}

template <class T>
error_t
PEFileT<T>::GetOverlay(IPEOverlay **ppOverlay)
{
    LIBPE_ASSERT_RET(NULL != ppOverlay, ERR_POINTER);
    return m_pOverlay.CopyTo(ppOverlay);
}

template <class T>
PEAddress
PEFileT<T>::GetRVAFromVA(PEAddress nVA)
{
    LIBPE_ASSERT_RET(NULL != m_pParser, 0);
    return m_pParser->GetRVAFromVA(nVA);
}

template <class T>
PEAddress
PEFileT<T>::GetVAFromRVA(PEAddress nRVA)
{
    LIBPE_ASSERT_RET(NULL != m_pParser, 0);
    return m_pParser->GetVAFromRVA(nRVA);
}

template <class T>
PEAddress
PEFileT<T>::GetRVAFromFOA(PEAddress nFOA)
{
    LIBPE_ASSERT_RET(NULL != m_pParser, 0);
    return m_pParser->GetRVAFromFOA(nFOA);
}

template <class T>
PEAddress
PEFileT<T>::GetFOAFromRVA(PEAddress nRVA)
{
    LIBPE_ASSERT_RET(NULL != m_pParser, 0);
    return m_pParser->GetFOAFromRVA(nRVA);
}

template <class T>
PEAddress
PEFileT<T>::GetVAFromFOA(PEAddress nFOA)
{
    LIBPE_ASSERT_RET(NULL != m_pParser, 0);
    return m_pParser->GetVAFromFOA(nFOA);
}

template <class T>
PEAddress
PEFileT<T>::GetFOAFromVA(PEAddress nVA)
{
    LIBPE_ASSERT_RET(NULL != m_pParser, 0);
    return m_pParser->GetFOAFromVA(nVA);
}

template <class T>
error_t
PEFileT<T>::GetExportTable(IPEExportTable **ppExportTable)
{
    if(NULL == m_pExportTable) {
        LIBPE_ASSERT_RET(NULL != m_pParser, ERR_FAIL);
        if(ERR_OK != m_pParser->ParseExportTable(&m_pExportTable) || NULL == m_pExportTable) {
            return ERR_FAIL;
        }
    }

    return m_pExportTable.CopyTo(ppExportTable);
}

template <class T>
error_t
PEFileT<T>::GetImportTable(IPEImportTable **ppImportTable)
{
    if(NULL == m_pImportTable) {
        LIBPE_ASSERT_RET(NULL != m_pParser, ERR_FAIL);
        if(ERR_OK != m_pParser->ParseImportTable(&m_pImportTable) || NULL == m_pImportTable) {
            return ERR_FAIL;
        }
    }

    return m_pImportTable.CopyTo(ppImportTable);
}


template <class T>
error_t
PEFileT<T>::GetResourceTable(IPEResourceTable **ppResourceTable)
{
    if(NULL == m_pResourceTable) {
        LIBPE_ASSERT_RET(NULL != m_pParser, ERR_FAIL);
        if(ERR_OK != m_pParser->ParseResourceTable(&m_pResourceTable) || NULL == m_pResourceTable) {
            return ERR_FAIL;
        }
    }

    return m_pResourceTable.CopyTo(ppResourceTable);
}

template <class T>
error_t
PEFileT<T>::GetExceptionTable(IPEExceptionTable **ppExceptionTable)
{
    return ERR_NOT_IMPL;
}

template <class T>
error_t
PEFileT<T>::GetCertificateTable(IPECertificateTable **ppCertificateTable)
{
    return ERR_NOT_IMPL;
}

template <class T>
error_t
PEFileT<T>::GetRelocationTable(IPERelocationTable **ppRelocationTable)
{
    if(NULL == m_pRelocationTable) {
        LIBPE_ASSERT_RET(NULL != m_pParser, ERR_FAIL);
        if(ERR_OK != m_pParser->ParseRelocationTable(&m_pRelocationTable) || NULL == m_pRelocationTable) {
            return ERR_FAIL;
        }
    }

    return m_pRelocationTable.CopyTo(ppRelocationTable);
}

template <class T>
error_t
PEFileT<T>::GetImportAddressTable(IPEImportAddressTable **ppImportAddressTable)
{
    if(NULL == m_pImportAddressTable) {
        LIBPE_ASSERT_RET(NULL != m_pParser, ERR_FAIL);
        if(ERR_OK != m_pParser->ParseImportAddressTable(&m_pImportAddressTable) || NULL == m_pImportAddressTable) {
            return ERR_FAIL;
        }
    }

    return m_pImportAddressTable.CopyTo(ppImportAddressTable);
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEFileT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEFileT, Create);

LIBPE_NAMESPACE_END