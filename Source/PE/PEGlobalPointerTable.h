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
    ~PEGlobalPointerTableT() override {}

    DECLARE_PE_ELEMENT(LibPERawDataDirectoryT(T))

    LIBPE_FIELD_ACCESSOR_EX(PEAddress, GlobalPointerRVA, VirtualAddress);

    HRESULT LIBPE_CALLTYPE GetRelatedSection(IPESection **ppSection) override;
};

LIBPE_NAMESPACE_END