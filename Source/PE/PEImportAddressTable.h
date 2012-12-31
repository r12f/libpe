#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEImportAddressTableT :
    public IPEImportAddressTable,
    public PEElementT<T>
{
    typedef std::vector<LibPEPtr<IPEImportAddressBlock>> BlockList;

public:
    PEImportAddressTableT() {}
    virtual ~PEImportAddressTableT() {}

    LIBPE_SINGLE_THREAD_OBJECT()
    DECLARE_PE_ELEMENT(LibPERawThunkData(T))

    void InnerAddImportAddressBlock(IPEImportAddressBlock *pBlock) {
        LIBPE_ASSERT_RET_VOID(NULL != pBlock);
        m_vBlocks.push_back(pBlock);
    }

    virtual uint32_t LIBPE_CALLTYPE GetBlockCount();
    virtual error_t LIBPE_CALLTYPE GetBlockByIndex(uint32_t nIndex, IPEImportAddressBlock **ppBlock);
    virtual bool_t LIBPE_CALLTYPE IsBlockExists(IPEImportAddressBlock *pBlock);
    virtual bool_t LIBPE_CALLTYPE IsItemExist(IPEImportAddressItem *pItem);

private:
    BlockList   m_vBlocks;
};

template <class T>
class PEImportAddressBlockT :
    public IPEImportAddressBlock,
    public PEElementT<T>
{
    typedef std::vector<LibPEPtr<IPEImportAddressItem>> ItemList;

public:
    PEImportAddressBlockT() {}
    virtual ~PEImportAddressBlockT() {}

    LIBPE_SINGLE_THREAD_OBJECT()
    DECLARE_PE_ELEMENT(LibPERawThunkData(T))

    void InnerAddImportAddressItem(IPEImportAddressItem *pItem) {
        LIBPE_ASSERT_RET_VOID(NULL != pItem);
        m_vItems.push_back(pItem);
    }

    virtual uint32_t LIBPE_CALLTYPE GetItemCount();
    virtual error_t LIBPE_CALLTYPE GetItemByIndex(uint32_t nIndex, IPEImportAddressItem **ppItem);
    virtual bool_t LIBPE_CALLTYPE IsItemExist(IPEImportAddressItem *pItem);

private:
    ItemList    m_vItems;
};

template <class T>
class PEImportAddressItemT :
    public IPEImportAddressItem,
    public PEElementT<T>
{
public:
    PEImportAddressItemT() {}
    virtual ~PEImportAddressItemT() {}

    LIBPE_SINGLE_THREAD_OBJECT()
    DECLARE_PE_ELEMENT(LibPERawThunkData(T))

    virtual PEAddress LIBPE_CALLTYPE GetRawAddress();
};

typedef PEImportAddressTableT<PE32> PEImportAddressTable32;
typedef PEImportAddressBlockT<PE32> PEImportAddressBlock32;
typedef PEImportAddressItemT<PE32>  PEImportAddressItem32;

typedef PEImportAddressTableT<PE64> PEImportAddressTable64;
typedef PEImportAddressBlockT<PE64> PEImportAddressBlock64;
typedef PEImportAddressItemT<PE64>  PEImportAddressItem64;

LIBPE_NAMESPACE_END