#include "stdafx.h"
#include "PE/PEResourceTable.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
HRESULT
PEResourceTableT<T>::GetRootDirectory(IPEResourceDirectory **ppDirectory)
{
    LIBPE_ASSERT_RET(NULL != ppDirectory, E_POINTER);
    return m_pRootDirectory.CopyTo(ppDirectory);
}

template <class T>
UINT32
PEResourceDirectoryT<T>::GetEntryCount()
{
    return (UINT32)m_vEntries.size();
}

template <class T>
HRESULT
PEResourceDirectoryT<T>::GetEntryByIndex(UINT32 nIndex, IPEResourceDirectoryEntry **ppEntry)
{
    LIBPE_ASSERT_RET(NULL != ppEntry, E_POINTER);

    UINT32 nEntryCount = GetEntryCount();
    LIBPE_ASSERT_RET(nIndex < nEntryCount, E_INVALIDARG);

    if(NULL == m_vEntries[nIndex]) {
        LIBPE_ASSERT_RET(NULL != m_pParser, E_FAIL);
        HRESULT hr = m_pParser->ParseResourceDirectoryEntry(this, nIndex, &m_vEntries[nIndex]);
        if(FAILED(hr)) {
            return hr;
        }
    }

    if(NULL == m_vEntries[nIndex]) {
        return E_OUTOFMEMORY;
    }

    return m_vEntries[nIndex].CopyTo(ppEntry);
}

template <class T>
BOOL
PEResourceDirectoryEntryT<T>::IsNameId()
{
    LibPERawResourceDirectoryEntry(T) *pRawEntry = GetRawStruct();
    if(NULL == pRawEntry) {
        return false;
    }

    return (pRawEntry->NameIsString == 0);
}

template <class T>
UINT16
PEResourceDirectoryEntryT<T>::GetId()
{
    LibPERawResourceDirectoryEntry(T) *pRawEntry = GetRawStruct();
    if(NULL == pRawEntry) {
        return 0;
    }

    return pRawEntry->Id;
}

template <class T>
BOOL
PEResourceDirectoryEntryT<T>::IsNameString()
{
    LibPERawResourceDirectoryEntry(T) *pRawEntry = GetRawStruct();
    if(NULL == pRawEntry) {
        return false;
    }

    return (pRawEntry->NameIsString != 0);
}

template <class T>
const wchar_t *
PEResourceDirectoryEntryT<T>::GetName()
{
    LIBPE_ASSERT_RET(NULL != m_pParser && NULL != m_pFile, NULL);

    LibPERawResourceDirectoryEntry(T) *pRawEntry = GetRawStruct();
    if(NULL == pRawEntry) {
        return NULL;
    }

    LibPEPtr<IPEResourceTable> pTable;
    if(FAILED(m_pFile->GetResourceTable(&pTable)) || NULL == pTable) {
        return NULL;
    }

    UINT64 nNameSize = 0;
    LibPERawResourceStringU(T) *pResourceString = m_pParser->ParseResourceStringU(0, pTable->GetFOA() + pRawEntry->NameOffset, nNameSize);
    if(NULL == pResourceString) {
        return NULL;
    }

    return pResourceString->NameString;
}

template <class T>
BOOL
PEResourceDirectoryEntryT<T>::IsEntryDirectory()
{
    LIBPE_ASSERT_RET(NULL != m_pParser, NULL);

    LibPERawResourceDirectoryEntry(T) *pRawEntry = GetRawStruct();
    if(NULL == pRawEntry) {
        return false;
    }

    return !!(pRawEntry->DataIsDirectory);
}

template <class T>
HRESULT
PEResourceDirectoryEntryT<T>::GetDirectory(IPEResourceDirectory **ppDirectory)
{
    LIBPE_ASSERT_RET(NULL != ppDirectory, E_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pParser, NULL);

    *ppDirectory = NULL;

    LibPERawResourceDirectoryEntry(T) *pRawEntry = GetRawStruct();
    if(NULL == pRawEntry) {
        return E_OUTOFMEMORY;
    }

    LibPEPtr<IPEResourceTable> pTable;
    if(FAILED(m_pFile->GetResourceTable(&pTable)) || NULL == pTable) {
        return NULL;
    }

    LibPEPtr<IPEResourceDirectory> pDirectory;
    if(FAILED(m_pParser->ParseResourceDirectory(0, pTable->GetFOA() + pRawEntry->OffsetToDirectory, &pDirectory)) || NULL == pDirectory) {
        return E_OUTOFMEMORY;
    }

    *ppDirectory = pDirectory.Detach();

    return S_OK;
}

template <class T>
BOOL
PEResourceDirectoryEntryT<T>::IsEntryDataEntry()
{
    LIBPE_ASSERT_RET(NULL != m_pParser, NULL);

    LibPERawResourceDirectoryEntry(T) *pRawEntry = GetRawStruct();
    if(NULL == pRawEntry) {
        return false;
    }

    return !(pRawEntry->DataIsDirectory);
}

template <class T>
HRESULT
PEResourceDirectoryEntryT<T>::GetDataEntry(IPEResourceDataEntry **ppDataEntry)
{
    LIBPE_ASSERT_RET(NULL != ppDataEntry, E_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pParser, NULL);

    *ppDataEntry = NULL;

    LibPERawResourceDirectoryEntry(T) *pRawEntry = GetRawStruct();
    if(NULL == pRawEntry) {
        return E_OUTOFMEMORY;
    }

    LibPEPtr<IPEResourceTable> pTable;
    if(FAILED(m_pFile->GetResourceTable(&pTable)) || NULL == pTable) {
        return NULL;
    }

    LibPEPtr<IPEResourceDataEntry> pDataEntry;
    if(FAILED(m_pParser->ParseResourceDataEntry(0, pTable->GetFOA() + pRawEntry->OffsetToData, &pDataEntry)) || NULL == pDataEntry) {
        return E_OUTOFMEMORY;
    }

    *ppDataEntry = pDataEntry.Detach();

    return S_OK;
}

template <class T>
HRESULT
PEResourceDataEntryT<T>::GetResource(IPEResource **ppResource)
{
    LIBPE_ASSERT_RET(NULL != ppResource, E_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pParser, NULL);

    *ppResource = NULL;

    if(NULL != m_pResource) {
        return m_pResource.CopyTo(ppResource);
    }

    LibPERawResourceDataEntry(T) *pRawEntry = GetRawStruct();
    if(NULL == pRawEntry) {
        return E_OUTOFMEMORY;
    }

    LibPEPtr<IPEResourceTable> pTable;
    if(FAILED(m_pFile->GetResourceTable(&pTable)) || NULL == pTable) {
        return NULL;
    }

    if(FAILED(m_pParser->ParseResource(this, &m_pResource)) || NULL == m_pResource) {
        return E_OUTOFMEMORY;
    }

    return m_pResource.CopyTo(ppResource);
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEResourceTableT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEResourceDirectoryT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEResourceDirectoryEntryT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEResourceDataEntryT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEResourceT);

LIBPE_NAMESPACE_END