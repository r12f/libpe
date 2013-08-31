#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEDebugInfoTableT :
    public IPEDebugInfoTable,
    public PEElementT<T>
{
public:
    PEDebugInfoTableT() {}
    virtual ~PEDebugInfoTableT() {}

    DECLARE_PE_ELEMENT(LibPERawDebugDirectory(T))

    LIBPE_FIELD_ACCESSOR(UINT32, Characteristics);
    LIBPE_FIELD_ACCESSOR(UINT32, TimeDateStamp);
    LIBPE_FIELD_ACCESSOR(UINT16, MajorVersion);
    LIBPE_FIELD_ACCESSOR(UINT16, MinorVersion);
    LIBPE_FIELD_ACCESSOR(UINT32, Type);
    LIBPE_FIELD_ACCESSOR(UINT32, SizeOfData);
    LIBPE_FIELD_ACCESSOR(UINT32, AddressOfRawData);
    LIBPE_FIELD_ACCESSOR(UINT32, PointerToRawData);
};

LIBPE_NAMESPACE_END