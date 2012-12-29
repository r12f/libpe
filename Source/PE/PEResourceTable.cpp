#include "stdafx.h"
#include "PE/PEResourceTable.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
error_t
PEResourceTableT<T>::GetRootDirectory(IPEResourceDirectoryT<T> **ppDirectory)
{
    LIBPE_ASSERT_RET(NULL != ppDirectory, ERR_POINTER);
    return m_pRootDirectory.CopyTo(ppDirectory);
}

template <class T>
uint32_t
PEResourceDirectoryT<T>::GetEntryCount()
{
    return (uint32_t)m_vEntries.size();
}

template <class T>
error_t
PEResourceDirectoryT<T>::GetEntryByIndex(uint32_t nIndex, IPEResourceDirectoryEntryT<T> **ppEntry)
{
    LIBPE_ASSERT_RET(NULL != ppEntry, ERR_POINTER);

    uint32_t nEntryCount = GetEntryCount();
    LIBPE_ASSERT_RET(nIndex < nEntryCount, ERR_INVALID_ARG);

    if(NULL == m_vEntries[nIndex]) {
        LIBPE_ASSERT_RET(NULL != m_pParser, ERR_FAIL);
        error_t nRetCode = m_pParser->ParseResourceDirectoryEntry(this, nIndex, &m_vEntries[nIndex]);
        if(ERR_OK != nRetCode) {
            return nRetCode;
        }
    }

    if(NULL == m_vEntries[nIndex]) {
        return ERR_NO_MEM;
    }

    return m_vEntries[nIndex].CopyTo(ppEntry);
}

template <class T>
bool_t
PEResourceDirectoryEntryT<T>::IsNameId()
{
    LibPERawResourceDirectoryEntry(T) *pRawEntry = GetRawStruct();
    if(NULL == pRawEntry) {
        return false;
    }

    return (pRawEntry->NameIsString == 0);
}

template <class T>
uint16_t
PEResourceDirectoryEntryT<T>::GetId()
{
    LibPERawResourceDirectoryEntry(T) *pRawEntry = GetRawStruct();
    if(NULL == pRawEntry) {
        return 0;
    }

    return pRawEntry->Id;
}

template <class T>
bool_t
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

    LibPEPtr<IPEResourceTableT<T>> pTable;
    if(ERR_OK != m_pFile->GetResourceTable(&pTable) || NULL == pTable) {
        return NULL;
    }

    uint64_t nNameSize = 0;
    LibPERawResourceStringU(T) *pResourceString = m_pParser->ParseResourceStringU(0, pTable->GetFOA() + pRawEntry->NameOffset, nNameSize);
    if(NULL == pResourceString) {
        return NULL;
    }

    return pResourceString->NameString;
}

template <class T>
bool_t
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
error_t
PEResourceDirectoryEntryT<T>::GetDirectory(IPEResourceDirectoryT<T> **ppDirectory)
{
    LIBPE_ASSERT_RET(NULL != ppDirectory, ERR_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pParser, NULL);

    *ppDirectory = NULL;

    LibPERawResourceDirectoryEntry(T) *pRawEntry = GetRawStruct();
    if(NULL == pRawEntry) {
        return ERR_NO_MEM;
    }

    LibPEPtr<IPEResourceTableT<T>> pTable;
    if(ERR_OK != m_pFile->GetResourceTable(&pTable) || NULL == pTable) {
        return NULL;
    }

    LibPEPtr<IPEResourceDirectoryT<T>> pDirectory;
    if(ERR_OK != m_pParser->ParseResourceDirectory(0, pTable->GetFOA() + pRawEntry->OffsetToDirectory, &pDirectory) || NULL == pDirectory) {
        return ERR_NO_MEM;
    }

    *ppDirectory = pDirectory.Detach();

    return ERR_OK;
}

template <class T>
bool_t
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
error_t
PEResourceDirectoryEntryT<T>::GetDataEntry(IPEResourceDataEntryT<T> **ppDataEntry)
{
    LIBPE_ASSERT_RET(NULL != ppDataEntry, ERR_POINTER);
    LIBPE_ASSERT_RET(NULL != m_pParser, NULL);

    *ppDataEntry = NULL;

    LibPERawResourceDirectoryEntry(T) *pRawEntry = GetRawStruct();
    if(NULL == pRawEntry) {
        return ERR_NO_MEM;
    }

    LibPEPtr<IPEResourceTableT<T>> pTable;
    if(ERR_OK != m_pFile->GetResourceTable(&pTable) || NULL == pTable) {
        return NULL;
    }

    LibPEPtr<IPEResourceDataEntryT<T>> pDataEntry;
    if(ERR_OK != m_pParser->ParseResourceDataEntry(0, pTable->GetFOA() + pRawEntry->OffsetToData, &pDataEntry) || NULL == pDataEntry) {
        return ERR_NO_MEM;
    }

    *ppDataEntry = pDataEntry.Detach();

    return ERR_OK;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEResourceTableT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEResourceDirectoryT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEResourceDirectoryEntryT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEResourceDataEntryT);

LIBPE_NAMESPACE_END