#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEGlobalPointerTableT :
    public IPEGlobalPointerTable,
    public PEElementT<T>
{
public:
    PEGlobalPointerTableT() {}
    virtual ~PEGlobalPointerTableT() {}

    DECLARE_PE_ELEMENT(LibPERawDataDirectoryT(T))

    LIBPE_FIELD_ACCESSOR_EX(PEAddress, GlobalPointerRVA, VirtualAddress);

    virtual HRESULT LIBPE_CALLTYPE GetRelatedSection(IPESection **ppSection);
};

LIBPE_NAMESPACE_END