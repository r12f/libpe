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
    ~PEImportAddressTableT() override {}

    DECLARE_PE_ELEMENT(LibPERawThunkData(T))

    void InnerAddImportAddressBlock(IPEImportAddressBlock *pBlock) {
        LIBPE_CHK_RET_VOID(NULL != pBlock);
        m_vBlocks.push_back(pBlock);
    }

    UINT32 LIBPE_CALLTYPE GetBlockCount() override;
    HRESULT LIBPE_CALLTYPE GetBlockByIndex(UINT32 nIndex, IPEImportAddressBlock **ppBlock) override;
    BOOL LIBPE_CALLTYPE IsBlockExists(IPEImportAddressBlock *pBlock) override;
    BOOL LIBPE_CALLTYPE IsItemExist(IPEImportAddressItem *pItem) override;

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
    ~PEImportAddressBlockT() override {}

    DECLARE_PE_ELEMENT(LibPERawThunkData(T))

    void InnerAddImportAddressItem(IPEImportAddressItem *pItem) {
        LIBPE_CHK_RET_VOID(NULL != pItem);
        m_vItems.push_back(pItem);
    }

    UINT32 LIBPE_CALLTYPE GetItemCount() override;
    HRESULT LIBPE_CALLTYPE GetItemByIndex(UINT32 nIndex, IPEImportAddressItem **ppItem) override;
    BOOL LIBPE_CALLTYPE IsItemExist(IPEImportAddressItem *pItem) override;

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
    ~PEImportAddressItemT() override {}

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