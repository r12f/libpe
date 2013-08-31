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

    DECLARE_PE_ELEMENT(LibPERawThunkData(T))

    void InnerAddImportAddressBlock(IPEImportAddressBlock *pBlock) {
        LIBPE_CHK_RET_VOID(NULL != pBlock);
        m_vBlocks.push_back(pBlock);
    }

    virtual UINT32 LIBPE_CALLTYPE GetBlockCount();
    virtual HRESULT LIBPE_CALLTYPE GetBlockByIndex(UINT32 nIndex, IPEImportAddressBlock **ppBlock);
    virtual BOOL LIBPE_CALLTYPE IsBlockExists(IPEImportAddressBlock *pBlock);
    virtual BOOL LIBPE_CALLTYPE IsItemExist(IPEImportAddressItem *pItem);

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

    DECLARE_PE_ELEMENT(LibPERawThunkData(T))

    void InnerAddImportAddressItem(IPEImportAddressItem *pItem) {
        LIBPE_CHK_RET_VOID(NULL != pItem);
        m_vItems.push_back(pItem);
    }

    virtual UINT32 LIBPE_CALLTYPE GetItemCount();
    virtual HRESULT LIBPE_CALLTYPE GetItemByIndex(UINT32 nIndex, IPEImportAddressItem **ppItem);
    virtual BOOL LIBPE_CALLTYPE IsItemExist(IPEImportAddressItem *pItem);

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

    DECLARE_PE_ELEMENT(LibPERawThunkData(T))

    LIBPE_FIELD_ACCESSOR_EX(PEAddress, ForwarderString, u1.ForwarderString)
    LIBPE_FIELD_ACCESSOR_EX(PEAddress, Function, u1.Function)
    LIBPE_FIELD_ACCESSOR_EX(PEAddress, Ordinal, u1.Ordinal)
    LIBPE_FIELD_ACCESSOR_EX(PEAddress, AddressOfData, u1.AddressOfData)
};

typedef PEImportAddressTableT<PE32> PEImportAddressTable32;
typedef PEImportAddressBlockT<PE32> PEImportAddressBlock32;
typedef PEImportAddressItemT<PE32>  PEImportAddressItem32;

typedef PEImportAddressTableT<PE64> PEImportAddressTable64;
typedef PEImportAddressBlockT<PE64> PEImportAddressBlock64;
typedef PEImportAddressItemT<PE64>  PEImportAddressItem64;

LIBPE_NAMESPACE_END