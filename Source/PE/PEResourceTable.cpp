#include "stdafx.h"
#include "PE/PEResourceTable.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
HRESULT
PEResourceTableT<T>::GetRootDirectory(_Outptr_ IPEResourceDirectory **ppDirectory)
{
    LIBPE_CHK(NULL != ppDirectory, E_POINTER);
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
PEResourceDirectoryT<T>::GetEntryByIndex(_In_ UINT32 nIndex, _Outptr_ IPEResourceDirectoryEntry **ppEntry)
{
    LIBPE_CHK(NULL != ppEntry, E_POINTER);

    UINT32 nEntryCount = GetEntryCount();
    LIBPE_CHK(nIndex < nEntryCount, E_INVALIDARG);

    if(NULL == m_vEntries[nIndex]) {
        LIBPE_STRICTCHK(NULL != m_pParser);
        LIBPE_CHK_HR(m_pParser->ParseResourceDirectoryEntry(this, nIndex, &m_vEntries[nIndex]));
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
    return (pRawEntry->NameIsString == 0);
}

template <class T>
UINT16
PEResourceDirectoryEntryT<T>::GetId()
{
    LibPERawResourceDirectoryEntry(T) *pRawEntry = GetRawStruct();
    return pRawEntry->Id;
}

template <class T>
BOOL
PEResourceDirectoryEntryT<T>::IsNameString()
{
    LibPERawResourceDirectoryEntry(T) *pRawEntry = GetRawStruct();
    return (pRawEntry->NameIsString != 0);
}

template <class T>
const wchar_t *
PEResourceDirectoryEntryT<T>::GetName()
{
    LIBPE_CHK(NULL != m_pParser && NULL != m_pFile, NULL);

    LibPEPtr<IPEResourceTable> pTable;
    if(FAILED(m_pFile->GetResourceTable(&pTable)) || NULL == pTable) {
        return NULL;
    }

    UINT64 nNameSize = 0;
    LibPERawResourceDirectoryEntry(T) *pRawEntry = GetRawStruct();
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
    LIBPE_STRICTCHK(NULL != m_pParser);

    LibPERawResourceDirectoryEntry(T) *pRawEntry = GetRawStruct();
    return !!(pRawEntry->DataIsDirectory);
}

template <class T>
HRESULT
PEResourceDirectoryEntryT<T>::GetDirectory(_Outptr_ IPEResourceDirectory **ppDirectory)
{
    LIBPE_CHK(NULL != ppDirectory, E_POINTER);
    LIBPE_STRICTCHK(NULL != m_pParser);

    *ppDirectory = NULL;

    LibPEPtr<IPEResourceTable> pTable;
    if(FAILED(m_pFile->GetResourceTable(&pTable)) || NULL == pTable) {
        return E_FAIL;
    }

    LibPEPtr<IPEResourceDirectory> pDirectory;
    LibPERawResourceDirectoryEntry(T) *pRawEntry = GetRawStruct();
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
    LIBPE_STRICTCHK(NULL != m_pParser);

    LibPERawResourceDirectoryEntry(T) *pRawEntry = GetRawStruct();
    return !(pRawEntry->DataIsDirectory);
}

template <class T>
HRESULT
PEResourceDirectoryEntryT<T>::GetDataEntry(_Outptr_ IPEResourceDataEntry **ppDataEntry)
{
    LIBPE_CHK(NULL != ppDataEntry, E_POINTER);
    LIBPE_STRICTCHK(NULL != m_pParser);

    *ppDataEntry = NULL;

    LibPEPtr<IPEResourceTable> pTable;
    if(FAILED(m_pFile->GetResourceTable(&pTable)) || NULL == pTable) {
        return NULL;
    }

    LibPEPtr<IPEResourceDataEntry> pDataEntry;
    LibPERawResourceDirectoryEntry(T) *pRawEntry = GetRawStruct();
    if(FAILED(m_pParser->ParseResourceDataEntry(0, pTable->GetFOA() + pRawEntry->OffsetToData, &pDataEntry)) || NULL == pDataEntry) {
        return E_OUTOFMEMORY;
    }

    *ppDataEntry = pDataEntry.Detach();

    return S_OK;
}

template <class T>
HRESULT
PEResourceDataEntryT<T>::GetResource(_Outptr_ IPEResource **ppResource)
{
    LIBPE_CHK(NULL != ppResource, E_POINTER);
    LIBPE_STRICTCHK(NULL != m_pParser);

    *ppResource = NULL;

    if(NULL != m_pResource) {
        return m_pResource.CopyTo(ppResource);
    }

    LibPEPtr<IPEResourceTable> pTable;
    if(FAILED(m_pFile->GetResourceTable(&pTable)) || NULL == pTable) {
        return E_FAIL;
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