#pragma once

#include "Parser/PEParserCommon.h"
#include "PE/PEFile.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEElementT :
    public IPEElementT<T>
{
public:
    PEElementT(PEFileT *pFile) : m_pFile(pFile) { LIBPE_ASSERT(NULL != m_pFile); }
    virtual ~PEElementT() {}

    virtual void * LIBPE_CALLTYPE GetRawMemory();

    virtual PEAddressT<T> LIBPE_CALLTYPE GetRVA();
    virtual PEAddressT<T> LIBPE_CALLTYPE GetVA();
    virtual PEAddressT<T> LIBPE_CALLTYPE GetSizeInMemory();

    virtual PEAddressT<T> LIBPE_CALLTYPE GetFOA();
    virtual PEAddressT<T> LIBPE_CALLTYPE GetSizeInFile();

protected:
    PEFileT *m_pFile;
    PEAddressT<T> m_nRVA;
    PEAddressT<T> m_nSizeInMemory;
    PEAddressT<T> m_nSizeInFile;
};

LIBPE_NAMESPACE_END