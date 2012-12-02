#pragma once

#include "Parser/PEParserCommon.h"
#include "PE/PEFile.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEElementT :
    public IPEElementT<T>
{
public:
    PEElementT(PEFileT<T> *pFilee);
    virtual ~PEElementT();

    void SetBasicElementInfo(PEAddressT<T> nRVA, PEAddressT<T> nSizeInMemory, PEAddressT<T> nSizeInFile);

    // Override IPEElementT<T>
    virtual void * LIBPE_CALLTYPE GetRawMemory();

    virtual PEAddressT<T> LIBPE_CALLTYPE GetRVA();
    virtual PEAddressT<T> LIBPE_CALLTYPE GetVA();
    virtual PEAddressT<T> LIBPE_CALLTYPE GetSizeInMemory();

    virtual PEAddressT<T> LIBPE_CALLTYPE GetFOA();
    virtual PEAddressT<T> LIBPE_CALLTYPE GetSizeInFile();

protected:
    PEFileT<T> *m_pFile;
    PEAddressT<T> m_nRVA;
    PEAddressT<T> m_nSizeInMemory;
    PEAddressT<T> m_nSizeInFile;
};

typedef PEElementT<PE32> PEElement32;
typedef PEElementT<PE64> PEElement64;

LIBPE_NAMESPACE_END