#include "stdafx.h"
#include "PE/PEResourceTable.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
bool_t
PEResourceDirectoryEntryT<T>::IsNameId()
{
    LibPERawResourceDirectoryEntry(T) *pEntry = GetRawStruct();
    if(NULL == pEntry) {
        return false;
    }

    return (pEntry->NameIsString == 0);
}

template <class T>
uint16_t
PEResourceDirectoryEntryT<T>::GetId()
{
    LibPERawResourceDirectoryEntry(T) *pEntry = GetRawStruct();
    if(NULL == pEntry) {
        return false;
    }

    return pEntry->Id;
}

template <class T>
bool_t
PEResourceDirectoryEntryT<T>::IsNameString()
{
    LibPERawResourceDirectoryEntry(T) *pEntry = GetRawStruct();
    if(NULL == pEntry) {
        return false;
    }

    return (pEntry->NameIsString != 0);
}

template <class T>
const wchar_t *
PEResourceDirectoryEntryT<T>::GetName()
{
    return NULL;
}

template <class T>
bool_t
PEResourceDirectoryEntryT<T>::IsEntryDirectory()
{
    return true;
}

template <class T>
error_t
PEResourceDirectoryEntryT<T>::GetDirectory(IPEResourceDirectoryT<T> **ppDirectory)
{
    return ERR_OK;
}

template <class T>
bool_t
PEResourceDirectoryEntryT<T>::IsEntryDataEntry()
{
    return true;
}

template <class T>
error_t
PEResourceDirectoryEntryT<T>::GetDataEntry(IPEResourceDataEntryT<T> **ppDataEntry)
{
    return ERR_OK;
}

LIBPE_NAMESPACE_END