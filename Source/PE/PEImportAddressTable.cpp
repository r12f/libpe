#include "stdafx.h"
#include "PEImportAddressTable.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
uint32_t
PEImportAddressTableT<T>::GetBlockCount()
{
    return (uint32_t)m_vBlocks.size();
}

template <class T>
error_t
PEImportAddressTableT<T>::GetBlockByIndex(uint32_t nIndex, IPEImportAddressBlock **ppBlock)
{
    LIBPE_ASSERT_RET(NULL != ppBlock, ERR_POINTER);
    
    uint32_t nBlockCount = GetBlockCount();
    LIBPE_ASSERT_RET(nIndex < nBlockCount, ERR_INVALID_ARG);

    return m_vBlocks[nIndex].CopyTo(ppBlock);
}

template <class T>
bool_t
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
bool_t
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
uint32_t
PEImportAddressBlockT<T>::GetItemCount()
{
    return (uint32_t)m_vItems.size();
}

template <class T>
error_t
PEImportAddressBlockT<T>::GetItemByIndex(uint32_t nIndex, IPEImportAddressItem **ppItem)
{
    LIBPE_ASSERT_RET(NULL != ppItem, ERR_POINTER);
    
    uint32_t nItemCount = GetItemCount();
    LIBPE_ASSERT_RET(nIndex < nItemCount, ERR_INVALID_ARG);

    return m_vItems[nIndex].CopyTo(ppItem);
}

template <class T>
bool_t
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