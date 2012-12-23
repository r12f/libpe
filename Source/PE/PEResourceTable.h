#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEResourceTableT :
    public IPEResourceTableT<T>,
    public PEElementT<T>
{
public:
    PEResourceTableT() {}
    virtual ~PEResourceTableT() {}

    DECLARE_PE_ELEMENT(LibPERawResourceDirectory(T))
    LIBPE_SINGLE_THREAD_OBJECT()

    virtual 
};