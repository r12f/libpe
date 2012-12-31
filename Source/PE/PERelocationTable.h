#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PERelocationTableT :
    public IPERelocationTable,
    public PEElementT<T>
{
    typedef std::vector<LibPEPtr<IPERelocationPage>> RelocationPageList;

public:
    PERelocationTableT() {}
    virtual ~PERelocationTableT() {}

    DECLARE_PE_ELEMENT(LibPERawBaseRelocation(T))
    LIBPE_SINGLE_THREAD_OBJECT()

    void InnerAddRelocationPage(IPERelocationPage *pPage) {
        LIBPE_ASSERT_RET_VOID(NULL != pPage);
        m_vPages.push_back(pPage);
    }

    virtual uint32_t LIBPE_CALLTYPE GetPageCount();
    virtual error_t LIBPE_CALLTYPE GetPageByIndex(uint32_t nIndex, IPERelocationPage **ppRelocationPage);
    virtual bool_t LIBPE_CALLTYPE IsRVANeedRelocation(PEAddress nRVA);
    virtual error_t LIBPE_CALLTYPE GetItemByRVA(PEAddress nRVA, IPERelocationItem **ppRelocationItem);

private:
    RelocationPageList m_vPages;
};

template <class T>
class PERelocationPageT :
    public IPERelocationPage,
    public PEElementT<T>
{
    typedef std::vector<LibPEPtr<IPERelocationItem>> RelocationItemList;

public:
    PERelocationPageT() {}
    virtual ~PERelocationPageT() {}

    DECLARE_PE_ELEMENT(LibPERawBaseRelocation(T))
    LIBPE_SINGLE_THREAD_OBJECT()

    void InnerAddRelocationItem(IPERelocationItem *pItem) {
        LIBPE_ASSERT_RET_VOID(NULL != pItem);
        m_vItems.push_back(pItem);
    }

    virtual PEAddress LIBPE_CALLTYPE GetPageRVA();
    virtual uint32_t LIBPE_CALLTYPE GetItemCount();
    virtual error_t LIBPE_CALLTYPE GetItemByIndex(uint32_t nIndex, IPERelocationItem **ppRelocationItem);
    virtual bool_t LIBPE_CALLTYPE IsRVANeedRelocation(PEAddress nRVA);
    virtual error_t LIBPE_CALLTYPE GetItemByRVA(PEAddress nRVA, IPERelocationItem **ppRelocationItem);

private:
    RelocationItemList m_vItems;
};

template <class T>
class PERelocationItemT :
    public IPERelocationItem,
    public PEElementT<T>
{
public:
    PERelocationItemT() : m_nAddressRVA(0) {}
    virtual ~PERelocationItemT() {}

    DECLARE_PE_ELEMENT(void)
    LIBPE_SINGLE_THREAD_OBJECT()

    void InnerSetRelocateFlag(uint16_t nRelocateFlag) { m_nRelocateFlag = nRelocateFlag; }
    void InnerSetAddressRVA(PEAddress nRVA) { m_nAddressRVA = nRVA; }

    virtual uint16_t LIBPE_CALLTYPE GetFlag();
    virtual PEAddress LIBPE_CALLTYPE GetAddressRVA();
    virtual PEAddress LIBPE_CALLTYPE GetAddressContent();
    virtual PEAddress * LIBPE_CALLTYPE GetRawAddressContent();

private:
    uint16_t    m_nRelocateFlag;
    PEAddress   m_nAddressRVA;
};

typedef PERelocationTableT<PE32> PERelocationTable32;
typedef PERelocationPageT<PE32> PERelocationPage32;
typedef PERelocationItemT<PE32> PERelocationItem32;

typedef PERelocationTableT<PE64> PERelocationTable64;
typedef PERelocationPageT<PE64> PERelocationPage64;
typedef PERelocationItemT<PE64> PERelocationItem64;

LIBPE_NAMESPACE_END