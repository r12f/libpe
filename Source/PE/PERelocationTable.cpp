#include "stdafx.h"
#include "PERelocationTable.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
UINT32
PERelocationTableT<T>::GetPageCount()
{
    return (UINT32)m_vPages.size();
}

template <class T>
HRESULT
PERelocationTableT<T>::GetPageByIndex(_In_ UINT32 nIndex, _Outptr_ IPERelocationPage **ppRelocationPage)
{
    LIBPE_CHK(NULL != ppRelocationPage, E_POINTER);

    UINT32 nPageCount = GetPageCount();
    LIBPE_CHK(nIndex < nPageCount, E_FAIL);

    return m_vPages[nIndex].CopyTo(ppRelocationPage);
}

template <class T>
BOOL
PERelocationTableT<T>::IsRVANeedRelocation(_In_ PEAddress nRVA)
{
    LibPEPtr<IPERelocationItem> pItem;
    return ((SUCCEEDED(GetItemByRVA(nRVA, &pItem))) && NULL != pItem);
}

template <class T>
HRESULT
PERelocationTableT<T>::GetItemByRVA(_In_ PEAddress nRVA, _Outptr_ IPERelocationItem **ppRelocationItem)
{
    LIBPE_CHK(NULL != ppRelocationItem, E_POINTER);
    *ppRelocationItem = NULL;

    UINT32 nPageCount = GetPageCount();
    for(UINT32 nPageIndex = 0; nPageIndex < nPageCount; ++nPageIndex) {
        switch(m_vPages[nPageIndex]->GetItemByRVA(nRVA, ppRelocationItem)) {
        case S_OK:
            return S_OK;
        case E_INVALIDARG:
            return E_INVALIDARG;
        }
    }

    return E_FAIL;
}

template <class T>
PEAddress
PERelocationPageT<T>::GetPageRVA()
{
    LibPERawBaseRelocation(T) *pRawStruct = GetRawStruct();
    if(NULL == pRawStruct) {
        return LIBPE_INVALID_ADDRESS;
    }

    return pRawStruct->VirtualAddress;
}

template <class T>
UINT32
PERelocationPageT<T>::GetItemCount()
{
    return (UINT32)m_vItems.size();
}

template <class T>
HRESULT
PERelocationPageT<T>::GetItemByIndex(_In_ UINT32 nIndex, _Outptr_ IPERelocationItem **ppRelocationItem)
{
    LIBPE_CHK(NULL != ppRelocationItem, E_POINTER);

    UINT32 nItemCount = GetItemCount();
    LIBPE_CHK(nIndex < nItemCount, E_FAIL);

    return m_vItems[nIndex].CopyTo(ppRelocationItem);
}

template <class T>
BOOL
PERelocationPageT<T>::IsRVANeedRelocation(_In_ PEAddress nRVA)
{
    LibPEPtr<IPERelocationItem> pItem;
    return ((SUCCEEDED(GetItemByRVA(nRVA, &pItem))) && NULL != pItem);
}

template <class T>
HRESULT
PERelocationPageT<T>::GetItemByRVA(_In_ PEAddress nRVA, _Outptr_ IPERelocationItem **ppRelocationItem)
{
    LIBPE_CHK(NULL != ppRelocationItem, E_POINTER);
    *ppRelocationItem = NULL;

    PEAddress nRVABase = (nRVA & 0xFFFF0000);
    if(nRVABase != GetPageRVA()) {
        return E_INVALIDARG;
    }

    UINT32 nItemCount = GetItemCount();
    for(UINT32 nItemIndex = 0; nItemIndex < nItemCount; ++nItemIndex) {
        if(m_vItems[nItemIndex]->GetAddressRVA() == nRVA) {
            return m_vItems[nItemIndex].CopyTo(ppRelocationItem);
        }
    }

    return E_FAIL;
}

template <class T>
UINT16
PERelocationItemT<T>::GetFlag()
{
    return m_nRelocateFlag;
}

template <class T>
PEAddress
PERelocationItemT<T>::GetAddressRVA()
{
    return m_nAddressRVA;
}

template <class T>
PEAddress
PERelocationItemT<T>::GetAddressContent()
{
    PEAddress *pAddressContent = GetRawAddressContent();
    if(NULL == pAddressContent) {
        return LIBPE_INVALID_ADDRESS;
    }
    
    return *pAddressContent;
}

template <class T>
PEAddress *
PERelocationItemT<T>::GetRawAddressContent()
{
    LIBPE_CHK(NULL != m_pParser, 0);
    PEAddress nAddressFOA = m_pParser->GetFOAFromRVA(m_nAddressRVA);
    return (_In_ PEAddress *)m_pParser->GetRawMemory(nAddressFOA, sizeof(UINT16));
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PERelocationTableT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PERelocationPageT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PERelocationItemT);

LIBPE_NAMESPACE_END
