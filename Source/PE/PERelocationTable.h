#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PERelocationTableT :
    public IPERelocationTableT<T>,
    public PEElementT<T>
{
    typedef std::vector<LibPEPtr<IPERelocationPageT<T>>> RelocationPageList;

public:
    PERelocationTableT() {}
    virtual ~PERelocationTableT() {}

    DECLARE_PE_ELEMENT(LibPERawBaseRelocation(T))
    LIBPE_SINGLE_THREAD_OBJECT()

    void InnerAddRelocationPage(IPERelocationPageT<T> *pPage) {
        LIBPE_ASSERT_RET_VOID(NULL != pPage);
        m_vPages.push_back(pPage);
    }

    virtual uint32_t LIBPE_CALLTYPE GetRelocationPageCount();
    virtual error_t LIBPE_CALLTYPE GetRelocationPageByIndex(uint32_t nIndex, IPERelocationPageT<T> **ppRelocationPage);
    virtual bool_t LIBPE_CALLTYPE IsRVANeedRelocation(LibPEAddressT(T) nRVA);
    virtual error_t LIBPE_CALLTYPE GetRelocationItemByRVA(LibPEAddressT(T) nRVA, IPERelocationItemT<T> **ppRelocationItem);

private:
    RelocationPageList m_vPages;
};

template <class T>
class PERelocationPageT :
    public IPERelocationPageT<T>,
    public PEElementT<T>
{
    typedef std::vector<LibPEPtr<IPERelocationItemT<T>>> RelocationItemList;

public:
    PERelocationPageT() {}
    virtual ~PERelocationPageT() {}

    DECLARE_PE_ELEMENT(LibPERawBaseRelocation(T))
    LIBPE_SINGLE_THREAD_OBJECT()

    void InnerAddRelocationItem(IPERelocationItemT<T> *pItem) {
        LIBPE_ASSERT_RET_VOID(NULL != pItem);
        m_vItems.push_back(pItem);
    }

    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetPageRVA();
    virtual uint32_t LIBPE_CALLTYPE GetRelocationItemCount();
    virtual error_t LIBPE_CALLTYPE GetRelocationItemByIndex(uint32_t nIndex, IPERelocationItemT<T> **ppRelocationItem);
    virtual bool_t LIBPE_CALLTYPE IsRVANeedRelocation(LibPEAddressT(T) nRVA);
    virtual error_t LIBPE_CALLTYPE GetRelocationItemByRVA(LibPEAddressT(T) nRVA, IPERelocationItemT<T> **ppRelocationItem);

private:
    RelocationItemList m_vItems;
};

template <class T>
class PERelocationItemT :
    public IPERelocationItemT<T>,
    public PEElementT<T>
{
public:
    PERelocationItemT() : m_nAddressRVA(0) {}
    virtual ~PERelocationItemT() {}

    DECLARE_PE_ELEMENT(void)
    LIBPE_SINGLE_THREAD_OBJECT()

    void InnerSetRelocateFlag(uint16_t nRelocateFlag) { m_nRelocateFlag = nRelocateFlag; }
    void InnerSetAddressRVA(LibPEAddressT(T) nRVA) { m_nAddressRVA = nRVA; }

    virtual uint16_t LIBPE_CALLTYPE GetFlag();
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetAddressRVA();
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetAddressContent();
    virtual LibPEAddressT(T) * LIBPE_CALLTYPE GetRawAddressContent();

private:
    uint16_t                        m_nRelocateFlag;
    LibPEAddressT(T)                m_nAddressRVA;
};

typedef PERelocationTableT<PE32> PERelocationTable32;
typedef PERelocationPageT<PE32> PERelocationPage32;
typedef PERelocationItemT<PE32> PERelocationItem32;

typedef PERelocationTableT<PE64> PERelocationTable64;
typedef PERelocationPageT<PE64> PERelocationPage64;
typedef PERelocationItemT<PE64> PERelocationItem64;

LIBPE_NAMESPACE_END