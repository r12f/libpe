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
    ~PERelocationTableT() override {}

    DECLARE_PE_ELEMENT(LibPERawBaseRelocation(T))

    void InnerAddRelocationPage(_In_ IPERelocationPage *pPage) { m_vPages.push_back(pPage); }

    UINT32 LIBPE_CALLTYPE GetPageCount() override;
    HRESULT LIBPE_CALLTYPE GetPageByIndex(_In_ UINT32 nIndex, _Outptr_ IPERelocationPage **ppRelocationPage) override;
    BOOL LIBPE_CALLTYPE IsRVANeedRelocation(_In_ PEAddress nRVA) override;
    HRESULT LIBPE_CALLTYPE GetItemByRVA(_In_ PEAddress nRVA, _Outptr_ IPERelocationItem **ppRelocationItem) override;

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
    ~PERelocationPageT() override {}

    DECLARE_PE_ELEMENT(LibPERawBaseRelocation(T))

    void InnerAddRelocationItem(_In_ IPERelocationItem *pItem) { m_vItems.push_back(pItem); }

    LIBPE_FIELD_ACCESSOR(UINT32, VirtualAddress)
    LIBPE_FIELD_ACCESSOR(UINT32, SizeOfBlock)

    PEAddress LIBPE_CALLTYPE GetPageRVA() override;
    UINT32 LIBPE_CALLTYPE GetItemCount() override;
    HRESULT LIBPE_CALLTYPE GetItemByIndex(_In_ UINT32 nIndex, _Outptr_ IPERelocationItem **ppRelocationItem) override;
    BOOL LIBPE_CALLTYPE IsRVANeedRelocation(_In_ PEAddress nRVA) override;
    HRESULT LIBPE_CALLTYPE GetItemByRVA(_In_ PEAddress nRVA, _Outptr_ IPERelocationItem **ppRelocationItem) override;

private:
    RelocationItemList m_vItems;
};

template <class T>
class PERelocationItemT :
    public IPERelocationItem,
    public PEElementT<T>
{
public:
    PERelocationItemT() : m_nAddressRVA(LIBPE_INVALID_ADDRESS) {}
    ~PERelocationItemT() override {}

    DECLARE_PE_ELEMENT(void)

    void InnerSetRelocateFlag(UINT16 nRelocateFlag) { m_nRelocateFlag = nRelocateFlag; }
    void InnerSetAddressRVA(_In_ PEAddress nRVA) { m_nAddressRVA = nRVA; }

    UINT16 LIBPE_CALLTYPE GetFlag() override;
    PEAddress LIBPE_CALLTYPE GetAddressRVA() override;
    PEAddress LIBPE_CALLTYPE GetAddressContent() override;
    PEAddress * LIBPE_CALLTYPE GetRawAddressContent() override;

private:
    UINT16      m_nRelocateFlag;
    PEAddress   m_nAddressRVA;
};

typedef PERelocationTableT<PE32> PERelocationTable32;
typedef PERelocationPageT<PE32> PERelocationPage32;
typedef PERelocationItemT<PE32> PERelocationItem32;

typedef PERelocationTableT<PE64> PERelocationTable64;
typedef PERelocationPageT<PE64> PERelocationPage64;
typedef PERelocationItemT<PE64> PERelocationItem64;

LIBPE_NAMESPACE_END