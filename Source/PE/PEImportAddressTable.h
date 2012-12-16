#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEImportAddressTableT :
    public IPEImportAddressTableT<T>,
    public PEElementT<T>
{
    typedef std::vector<LibPEPtr<IPEImportAddressBlockT<T>>> BlockList;

public:
    PEImportAddressTableT() {}
    virtual ~PEImportAddressTableT() {}

    LIBPE_SINGLE_THREAD_OBJECT()
    DECLARE_PE_ELEMENT(LibPERawThunkData(T))

    void InnerAddImportAddressBlock(IPEImportAddressBlockT<T> *pBlock) {
        LIBPE_ASSERT_RET_VOID(NULL != pBlock);
        m_vBlocks.push_back(pBlock);
    }

    virtual uint32_t LIBPE_CALLTYPE GetImportAddressBlockCount();
    virtual error_t LIBPE_CALLTYPE GetImportAddressBlockByIndex(uint32_t nIndex, IPEImportAddressBlockT<T> **ppBlock);
    virtual bool_t LIBPE_CALLTYPE IsImportAddressBlockInTable(IPEImportAddressBlockT<T> *pBlock);
    virtual bool_t LIBPE_CALLTYPE IsImportAddressItemInTable(IPEImportAddressItemT<T> *pItem);

private:
    BlockList   m_vBlocks;
};

template <class T>
class PEImportAddressBlockT :
    public IPEImportAddressBlockT<T>,
    public PEElementT<T>
{
    typedef std::vector<LibPEPtr<IPEImportAddressItemT<T>>> ItemList;

public:
    PEImportAddressBlockT() {}
    virtual ~PEImportAddressBlockT() {}

    LIBPE_SINGLE_THREAD_OBJECT()
    DECLARE_PE_ELEMENT(LibPERawThunkData(T))

    void InnerAddImportAddressItem(IPEImportAddressItemT<T> *pItem) {
        LIBPE_ASSERT_RET_VOID(NULL != pItem);
        m_vItems.push_back(pItem);
    }

    virtual uint32_t LIBPE_CALLTYPE GetImportAddressItemCount();
    virtual error_t LIBPE_CALLTYPE GetImportAddressItemByIndex(uint32_t nIndex, IPEImportAddressItemT<T> **ppItem);
    virtual bool_t LIBPE_CALLTYPE IsImportAddressItemInTable(IPEImportAddressItemT<T> *pItem);

private:
    ItemList    m_vItems;
};

template <class T>
class PEImportAddressItemT :
    public IPEImportAddressItemT<T>,
    public PEElementT<T>
{
public:
    PEImportAddressItemT() {}
    virtual ~PEImportAddressItemT() {}

    LIBPE_SINGLE_THREAD_OBJECT()
    DECLARE_PE_ELEMENT(LibPERawThunkData(T))

    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetRawAddress();
};

typedef PEImportAddressTableT<PE32> PEImportAddressTable32;
typedef PEImportAddressBlockT<PE32> PEImportAddressBlock32;
typedef PEImportAddressItemT<PE32>  PEImportAddressItem32;

typedef PEImportAddressTableT<PE64> PEImportAddressTable64;
typedef PEImportAddressBlockT<PE64> PEImportAddressBlock64;
typedef PEImportAddressItemT<PE64>  PEImportAddressItem64;

LIBPE_NAMESPACE_END