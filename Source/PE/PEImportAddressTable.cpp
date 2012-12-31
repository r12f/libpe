#include "stdafx.h"
#include "PEImportAddressTable.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
UINT32
PEImportAddressTableT<T>::GetBlockCount()
{
    return (UINT32)m_vBlocks.size();
}

template <class T>
HRESULT
PEImportAddressTableT<T>::GetBlockByIndex(UINT32 nIndex, IPEImportAddressBlock **ppBlock)
{
    LIBPE_ASSERT_RET(NULL != ppBlock, E_POINTER);
    
    UINT32 nBlockCount = GetBlockCount();
    LIBPE_ASSERT_RET(nIndex < nBlockCount, E_INVALIDARG);

    return m_vBlocks[nIndex].CopyTo(ppBlock);
}

template <class T>
BOOL
PEImportAddressTableT<T>::IsBlockExists(IPEImportAddressBlock *pBlock)
{
    LIBPE_ASSERT_RET(NULL != pBlock, false);

    void *pArgRawBlock = pBlock->GetRawMemory();
    LIBPE_ASSERT_RET(NULL != pArgRawBlock, false);

    BlockList::iterator oBlockIt;
    for(oBlockIt = m_vBlocks.begin(); oBlockIt != m_vBlocks.end(); ++oBlockIt) {
        if((*oBlockIt)->GetRawMemory() == pArgRawBlock) {
            return true;
        }
    }

    return false;
}

template <class T>
BOOL
PEImportAddressTableT<T>::IsItemExist(IPEImportAddressItem *pItem)
{
    LIBPE_ASSERT_RET(NULL != pItem, false);

    BlockList::iterator oBlockIt;
    for(oBlockIt = m_vBlocks.begin(); oBlockIt != m_vBlocks.end(); ++oBlockIt) {
        if((*oBlockIt)->IsItemExist(pItem)) {
            return true;
        }
    }

    return false;
}

template <class T>
UINT32
PEImportAddressBlockT<T>::GetItemCount()
{
    return (UINT32)m_vItems.size();
}

template <class T>
HRESULT
PEImportAddressBlockT<T>::GetItemByIndex(UINT32 nIndex, IPEImportAddressItem **ppItem)
{
    LIBPE_ASSERT_RET(NULL != ppItem, E_POINTER);
    
    UINT32 nItemCount = GetItemCount();
    LIBPE_ASSERT_RET(nIndex < nItemCount, E_INVALIDARG);

    return m_vItems[nIndex].CopyTo(ppItem);
}

template <class T>
BOOL
PEImportAddressBlockT<T>::IsItemExist(IPEImportAddressItem *pItem)
{
    LIBPE_ASSERT_RET(NULL != pItem, false);

    void *pArgRawItem = pItem->GetRawMemory();
    LIBPE_ASSERT_RET(NULL != pArgRawItem, false);

    ItemList::iterator oItemIt;
    for(oItemIt = m_vItems.begin(); oItemIt != m_vItems.end(); ++oItemIt) {
        if((*oItemIt)->GetRawMemory() == pArgRawItem) {
            return true;
        }
    }

    return false;
}

template <class T>
PEAddress
PEImportAddressItemT<T>::GetRawAddress()
{
    LibPERawThunkData(T) *pRawStruct = GetRawStruct();
    LIBPE_ASSERT_RET(NULL != pRawStruct, 0);
    return pRawStruct->u1.Function;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEImportAddressTableT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEImportAddressBlockT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEImportAddressItemT);

LIBPE_NAMESPACE_END