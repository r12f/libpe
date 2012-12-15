#include "stdafx.h"
#include "PERelocationTable.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
uint32_t
PERelocationTableT<T>::GetRelocationPageCount()
{
    return (uint32_t)m_vPages.size();
}

template <class T>
error_t
PERelocationTableT<T>::GetRelocationPageByIndex(uint32_t nIndex, IPERelocationPageT<T> **ppRelocationPage)
{
    LIBPE_ASSERT_RET(NULL != ppRelocationPage, ERR_POINTER);

    uint32_t nPageCount = GetRelocationPageCount();
    LIBPE_ASSERT_RET(nIndex < nPageCount, ERR_FAIL);

    return m_vPages[nIndex].CopyTo(ppRelocationPage);
}

template <class T>
bool_t
PERelocationTableT<T>::IsRVANeedRelocation(LibPEAddressT(T) nRVA)
{
    LibPEPtr<IPERelocationItemT<T>> pItem;
    return ((ERR_OK == GetRelocationItemByRVA(nRVA, &pItem)) && NULL != pItem);
}

template <class T>
error_t
PERelocationTableT<T>::GetRelocationItemByRVA(LibPEAddressT(T) nRVA, IPERelocationItemT<T> **ppRelocationItem)
{
    LIBPE_ASSERT_RET(NULL != ppRelocationItem, ERR_POINTER);
    *ppRelocationItem = NULL;

    uint32_t nPageCount = GetRelocationPageCount();
    for(uint32_t nPageIndex = 0; nPageIndex < nPageCount; ++nPageIndex) {
        switch(m_vPages[nPageIndex]->GetRelocationItemByRVA(nRVA, ppRelocationItem)) {
        case ERR_OK:
            return ERR_OK;
        case ERR_INVALID_ARG:
            return ERR_INVALID_ARG;
        }
    }

    return ERR_FAIL;
}

template <class T>
LibPEAddressT(T)
PERelocationPageT<T>::GetPageRVA()
{
    LibPERawBaseRelocation(T) *pRawStruct = GetRawStruct();
    if(NULL == pRawStruct) {
        return 0;
    }

    return pRawStruct->VirtualAddress;
}

template <class T>
uint32_t
PERelocationPageT<T>::GetRelocationItemCount()
{
    return (uint32_t)m_vItems.size();
}

template <class T>
error_t
PERelocationPageT<T>::GetRelocationItemByIndex(uint32_t nIndex, IPERelocationItemT<T> **ppRelocationItem)
{
    LIBPE_ASSERT_RET(NULL != ppRelocationItem, ERR_POINTER);

    uint32_t nItemCount = GetRelocationItemCount();
    LIBPE_ASSERT_RET(nIndex < nItemCount, ERR_FAIL);

    return m_vItems[nIndex].CopyTo(ppRelocationItem);
}

template <class T>
bool_t
PERelocationPageT<T>::IsRVANeedRelocation(LibPEAddressT(T) nRVA)
{
    LibPEPtr<IPERelocationItemT<T>> pItem;
    return ((ERR_OK == GetRelocationItemByRVA(nRVA, &pItem)) && NULL != pItem);
}

template <class T>
error_t
PERelocationPageT<T>::GetRelocationItemByRVA(LibPEAddressT(T) nRVA, IPERelocationItemT<T> **ppRelocationItem)
{
    LIBPE_ASSERT_RET(NULL != ppRelocationItem, ERR_POINTER);
    *ppRelocationItem = NULL;

    LibPEAddressT(T) nRVABase = (nRVA & 0xFFFF0000);
    if(nRVABase != GetPageRVA()) {
        return ERR_INVALID_ARG;
    }

    uint32_t nItemCount = GetRelocationItemCount();
    for(uint32_t nItemIndex = 0; nItemIndex < nItemCount; ++nItemIndex) {
        if(m_vItems[nItemIndex]->GetAddressRVA() == nRVA) {
            return m_vItems[nItemIndex].CopyTo(ppRelocationItem);
        }
    }

    return ERR_FAIL;
}

template <class T>
uint16_t
PERelocationItemT<T>::GetFlag()
{
    return m_nRelocateFlag;
}

template <class T>
LibPEAddressT(T)
PERelocationItemT<T>::GetAddressRVA()
{
    return m_nAddressRVA;
}

template <class T>
LibPEAddressT(T)
PERelocationItemT<T>::GetAddressContent()
{
    LibPEAddressT(T) *pAddressContent = GetRawAddressContent();
    if(NULL == pAddressContent) {
        return 0;
    }
    
    return *pAddressContent;
}

template <class T>
LibPEAddressT(T) *
PERelocationItemT<T>::GetRawAddressContent()
{
    LIBPE_ASSERT_RET(NULL != m_pParser, 0);
    LibPEAddressT(T) nAddressFOA = m_pParser->GetFOAFromRVA(m_nAddressRVA);
    return (LibPEAddressT(T) *)m_pParser->GetRawMemory(nAddressFOA, sizeof(uint16_t));
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PERelocationTable);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PERelocationPage);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PERelocationItem);

LIBPE_NAMESPACE_END
