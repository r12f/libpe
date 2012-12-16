#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PESectionHeaderT :
    public IPESectionHeaderT<T>,
    public PEElementT<T>
{
public:
    PESectionHeaderT() {}
    virtual ~PESectionHeaderT() {}

    LIBPE_SINGLE_THREAD_OBJECT()
    DECLARE_PE_ELEMENT(LibPERawSectionHeaderT(T))

    virtual error_t LIBPE_CALLTYPE GetSection(IPESectionT<T> **ppSection);

private:
    LibPEPtr<IPESectionT<T>>    m_pSection;
};

typedef PESectionHeaderT<PE32> PESectionHeader32;
typedef PESectionHeaderT<PE64> PESectionHeader64;

LIBPE_NAMESPACE_END