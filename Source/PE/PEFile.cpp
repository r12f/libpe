#include "stdafx.h"
#include "PE/PEFile.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
HRESULT
PEFileT<T>::Create(DataLoader *pLoader, IPEFile **ppFile)
{
    LIBPE_ASSERT_RET(NULL != pLoader && NULL != ppFile, E_POINTER);

    *ppFile = NULL;

    LibPEPtr<PEFileT<T>> pInnerFile = new PEFileT<T>();
    LibPEPtr<PEParserT<T>> pParser = PEParserT<T>::Create(pLoader->GetType());
    LIBPE_ASSERT_RET(NULL != pInnerFile && NULL != pParser, E_OUTOFMEMORY);

    pParser->SetPEFile(pInnerFile);
    pParser->SetDataLoader(pLoader);

    if(FAILED(pInnerFile->Init(pParser))) {
        return E_FAIL;
    }

    *ppFile = pInnerFile.Detach();

    return S_OK;
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
PEFileT<T>::GetRawDosHeader()
{
    LIBPE_ASSERT_RET(NULL != m_pDosHeader, NULL);
    return (PERawDosHeader *)m_pDosHeader->GetRawMemory();
}

template <class T>
void *
PEFileT<T>::GetRawNtHeaders()
{
    LIBPE_ASSERT_RET(NULL != m_pNtHeaders, NULL);
    return m_pNtHeaders->GetRawMemory();
}

template <class T>
PERawNtHeaders32 *
PEFileT<T>::GetRawNtHeaders32()
{
    return (PERawNtHeaders32 *)(Is32Bit() ? GetRawNtHeaders() : NULL);
}

template <class T>
PERawNtHeaders64 *
PEFileT<T>::GetRawNtHeaders64()
{
    LIBPE_ASSERT_RET(NULL != m_pNtHeaders, NULL);
    return (PERawNtHeaders64 *)(Is32Bit() ? NULL : GetRawNtHeaders());
}

template <class T>
PERawFileHeader *
PEFileT<T>::GetRawFileHeader()
{
    LIBPE_ASSERT_RET(NULL != m_pFileHeader, NULL);
    return (PERawFileHeader *)m_pFileHeader->GetRawMemory();
}

template <class T>
void *
PEFileT<T>::GetRawOptionalHeader()
{
    LIBPE_ASSERT_RET(NULL != m_pOptionalHeader, NULL);
    return m_pOptionalHeader->GetRawMemory();
}

template <class T>
PERawOptionalHeader32 *
PEFileT<T>::GetRawOptionalHeader32()
{
    return (PERawOptionalHeader32 *)(Is32Bit() ? GetRawOptionalHeader() : NULL);
}

template <class T>
PERawOptionalHeader64 *
PEFileT<T>::GetRawOptionalHeader64()
{
    return (PERawOptionalHeader64 *)(Is32Bit() ? GetRawOptionalHeader() : NULL);
}

template <class T>
HRESULT
PEFileT<T>::GetDosHeader(IPEDosHeader **ppDosHeader)
{
    return m_pDosHeader.CopyTo(ppDosHeader);
}

template <class T>
HRESULT
PEFileT<T>::GetNtHeaders(IPENtHeaders **ppNtHeaders)
{
    return m_pNtHeaders.CopyTo(ppNtHeaders);
}

template <class T>
HRESULT
PEFileT<T>::GetFileHeader(IPEFileHeader **ppFileHeader)
{
    LIBPE_ASSERT_RET(NULL != m_pNtHeaders, NULL);
    return m_pNtHeaders->GetFileHeader(ppFileHeader);
}

template <class T>
HRESULT
PEFileT<T>::GetOptionalHeader(IPEOptionalHeader **ppOptionalHeader)
{
    LIBPE_ASSERT_RET(NULL != m_pNtHeaders, NULL);
    return m_pNtHeaders->GetOptionalHeader(ppOptionalHeader);
}

template <class T>
BOOL
PEFileT<T>::IsDosFile()
{
    LIBPE_ASSERT_RET(NULL != m_pDosHeader, false);
    return (NULL == m_pDosHeader->GetFieldLfanew());
}

template <class T>
BOOL
PEFileT<T>::Is32Bit()
{
    return PETrait<T>::Is32Bit;
}

template <class T>
PEAddress
PEFileT<T>::GetImageBase()
{
    LIBPE_ASSERT_RET(NULL != m_pOptionalHeader, NULL);
    return m_pOptionalHeader->GetFieldImageBase();
}

template <class T>
UINT32 
PEFileT<T>::GetImageSize()
{
    LIBPE_ASSERT_RET(NULL != m_pOptionalHeader, 0);
    return m_pOptionalHeader->GetFieldSizeOfImage();
}

template <class T>
UINT32 
PEFileT<T>::GetEntryPoint()
{
    LIBPE_ASSERT_RET(NULL != m_pOptionalHeader, 0);
    return m_pOptionalHeader->GetFieldAddressOfEntryPoint();
}

template <class T>
UINT32
PEFileT<T>::GetSectionCount()
{
    return (UINT32)m_vSectionHeaders.size();
}

template <class T>
HRESULT
PEFileT<T>::GetSectionHeader(UINT32 nIndex, IPESectionHeader **ppSectionHeader)
{
    LIBPE_ASSERT_RET(NULL != ppSectionHeader, E_POINTER);
    LIBPE_ASSERT_RET(nIndex < GetSectionCount(), E_INVALIDARG);
    return m_vSectionHeaders[nIndex].CopyTo(ppSectionHeader);
}

template <class T>
HRESULT
PEFileT<T>::GetSection(UINT32 nIndex, IPESection **ppSection)
{
    LIBPE_ASSERT_RET(NULL != ppSection, E_POINTER);
    LIBPE_ASSERT_RET(nIndex < GetSectionCount(), E_INVALIDARG);
    LIBPE_ASSERT_RET(NULL != m_vSectionHeaders[nIndex], E_FAIL);
    return m_vSectionHeaders[nIndex]->GetSection(ppSection);
}

template <class T>
HRESULT
PEFileT<T>::GetSectionByRVA(PEAddress nRVA, IPESection **ppSection)
{
    LIBPE_ASSERT_RET(NULL != ppSection, E_POINTER);

    UINT32 nSectionCount = GetSectionCount();
    for(UINT32 nSectionIndex = 0; nSectionIndex < nSectionCount; ++nSectionIndex) {
        LibPEPtr<IPESection> pSection;
        if(SUCCEEDED(GetSection(nSectionIndex, &pSection)) && NULL != pSection) {
            if(pSection->GetRVA() <= nRVA && nRVA <= pSection->GetRVA() + pSection->GetSizeInMemory()) {
                *ppSection = pSection.Detach();
                return S_OK;
            }
        }
    }

    return E_FAIL;
}

template <class T>
HRESULT
PEFileT<T>::GetSectionByVA(PEAddress nVA, IPESection **ppSection)
{
    return GetSectionByRVA(GetRVAFromVA(nVA), ppSection);
}

template <class T>
HRESULT
PEFileT<T>::GetSectionByFOA(PEAddress nFOA, IPESection **ppSection)
{
    LIBPE_ASSERT_RET(NULL != ppSection, E_POINTER);

    UINT32 nSectionCount = GetSectionCount();
    for(UINT32 nSectionIndex = 0; nSectionIndex < nSectionCount; ++nSectionIndex) {
        LibPEPtr<IPESection> pSection;
        if(SUCCEEDED(GetSection(nSectionIndex, &pSection)) && NULL != pSection) {
            if(pSection->GetFOA() <= nFOA && nFOA <= pSection->GetFOA() + pSection->GetSizeInMemory()) {
                *ppSection = pSection.Detach();
                return S_OK;
            }
        }
    }

    return E_FAIL;
}

template <class T>
HRESULT
PEFileT<T>::GetOverlay(IPEOverlay **ppOverlay)
{
    LIBPE_ASSERT_RET(NULL != ppOverlay, E_POINTER);
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
HRESULT
PEFileT<T>::GetExportTable(IPEExportTable **ppExportTable)
{
    if(NULL == m_pExportTable) {
        LIBPE_ASSERT_RET(NULL != m_pParser, E_FAIL);
        if(FAILED(m_pParser->ParseExportTable(&m_pExportTable)) || NULL == m_pExportTable) {
            return E_FAIL;
        }
    }

    return m_pExportTable.CopyTo(ppExportTable);
}

template <class T>
HRESULT
PEFileT<T>::GetImportTable(IPEImportTable **ppImportTable)
{
    if(NULL == m_pImportTable) {
        LIBPE_ASSERT_RET(NULL != m_pParser, E_FAIL);
        if(FAILED(m_pParser->ParseImportTable(&m_pImportTable)) || NULL == m_pImportTable) {
            return E_FAIL;
        }
    }

    return m_pImportTable.CopyTo(ppImportTable);
}


template <class T>
HRESULT
PEFileT<T>::GetResourceTable(IPEResourceTable **ppResourceTable)
{
    if(NULL == m_pResourceTable) {
        LIBPE_ASSERT_RET(NULL != m_pParser, E_FAIL);
        if(FAILED(m_pParser->ParseResourceTable(&m_pResourceTable)) || NULL == m_pResourceTable) {
            return E_FAIL;
        }
    }

    return m_pResourceTable.CopyTo(ppResourceTable);
}

template <class T>
HRESULT
PEFileT<T>::GetExceptionTable(IPEExceptionTable **ppExceptionTable)
{
    return E_NOTIMPL;
}

template <class T>
HRESULT
PEFileT<T>::GetCertificateTable(IPECertificateTable **ppCertificateTable)
{
    return E_NOTIMPL;
}

template <class T>
HRESULT
PEFileT<T>::GetRelocationTable(IPERelocationTable **ppRelocationTable)
{
    if(NULL == m_pRelocationTable) {
        LIBPE_ASSERT_RET(NULL != m_pParser, E_FAIL);
        if(FAILED(m_pParser->ParseRelocationTable(&m_pRelocationTable)) || NULL == m_pRelocationTable) {
            return E_FAIL;
        }
    }

    return m_pRelocationTable.CopyTo(ppRelocationTable);
}

template <class T>
HRESULT
PEFileT<T>::GetImportAddressTable(IPEImportAddressTable **ppImportAddressTable)
{
    if(NULL == m_pImportAddressTable) {
        LIBPE_ASSERT_RET(NULL != m_pParser, E_FAIL);
        if(FAILED(m_pParser->ParseImportAddressTable(&m_pImportAddressTable)) || NULL == m_pImportAddressTable) {
            return E_FAIL;
        }
    }

    return m_pImportAddressTable.CopyTo(ppImportAddressTable);
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEFileT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEFileT, Create);

LIBPE_NAMESPACE_END