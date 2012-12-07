#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PESectionT :
    public IPESectionT<T>,
    public PEElementT<T>
{
public:
    PESectionT() : m_pSectionHeader(NULL) {}
    virtual ~PESectionT() {}

    LIBPE_SINGLE_THREAD_OBJECT()
    DECLARE_PE_ELEMENT()

    void SetRawSectionHeader(typename PETrait<T>::RawSectionHeader *pSectionHeader) { m_pSectionHeader = pSectionHeader; }

    virtual error_t LIBPE_CALLTYPE GetName(char *pName, int32_t nMaxSize);
    virtual error_t LIBPE_CALLTYPE SetName(const char *pName);

private:
    typename PETrait<T>::RawSectionHeader  *m_pSectionHeader;
};

typedef PESectionT<PE32> PESection32;
typedef PESectionT<PE64> PESection64;

LIBPE_NAMESPACE_END
