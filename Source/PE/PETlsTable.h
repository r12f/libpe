#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PETlsTableT :
    public IPETlsTable,
    public PEElementT<T>
{
    typedef std::vector<PEAddress> CallbackRVAList;

public:
    PETlsTableT() : m_bIsTlsCallbacksParsed(false) {}
    virtual ~PETlsTableT() {}

    DECLARE_PE_ELEMENT(LibPERawTlsDirectory(T))

    HRESULT InnerAddCallbackRVA(PEAddress nRVA);

    LIBPE_FIELD_ACCESSOR(UINT64, StartAddressOfRawData);
    LIBPE_FIELD_ACCESSOR(UINT64, EndAddressOfRawData);
    LIBPE_FIELD_ACCESSOR(UINT64, AddressOfIndex);
    LIBPE_FIELD_ACCESSOR(UINT64, AddressOfCallBacks);
    LIBPE_FIELD_ACCESSOR(UINT32, SizeOfZeroFill);
    LIBPE_FIELD_ACCESSOR(UINT32, Characteristics);
    LIBPE_FIELD_ACCESSOR(UINT32, Reserved0);
    LIBPE_FIELD_ACCESSOR(UINT32, Alignment);
    LIBPE_FIELD_ACCESSOR(UINT32, Reserved1);

    virtual UINT32 LIBPE_CALLTYPE GetCallbackCount();
    virtual PEAddress LIBPE_CALLTYPE GetCallbackRVAByIndex(UINT32 nIndex);

protected:
    HRESULT EnsureTlsCallbacksParsed();

private:
    bool m_bIsTlsCallbacksParsed;
    CallbackRVAList m_vCallbackRVAs;
};

LIBPE_NAMESPACE_END