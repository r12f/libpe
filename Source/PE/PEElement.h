#pragma once

#include "Parser/PEParserCommon.h"
#include "Parser/DataLoader.h"
#include "PE/PEFile.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEElementT :
    public IPEElementT<T>
{
public:
    PEElementT()
        : m_pFile(NULL), m_pRawBuffer(NULL), m_nRVA(0), m_nVA(0), m_nSizeInMemory(0), m_nFOA(0), m_nSizeInFile(0)
    {}

    virtual ~PEElementT() {}

    void InnerSetBase(PEFileT<T> *pFile, PEParserT<T> *pParser)
    {
        m_pFile = pFile;
        m_pParser = pParser;
    }

    void InnerSetRawMemory(void *pRawBuffer)
    {
        m_pRawBuffer = pRawBuffer;
    }
    
    void InnerSetMemoryInfo(LibPEAddressT(T) nRVA, LibPEAddressT(T) nVA, LibPEAddressT(T) nSizeInMemory)
    {
        m_nRVA = nRVA;
        m_nVA = nVA;
        m_nSizeInMemory = nSizeInMemory;
    }

    void InnerSetFileInfo(LibPEAddressT(T) nFOA, LibPEAddressT(T) nSizeInFile)
    {
        m_nFOA = nFOA;
        m_nSizeInFile = nSizeInFile;
    }

    // Override IPEElementT<T>
    virtual void * LIBPE_CALLTYPE GetRawMemory();
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetRawOffset();
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetRawSize();

    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetRVA() { return m_nRVA; }
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetVA();
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetSizeInMemory() { return m_nSizeInMemory; }

    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetFOA();
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetSizeInFile() { return m_nSizeInFile; }

protected:
    LibPEPtr<PEParserT<T>>  m_pParser;
    PEFileT<T>              *m_pFile;
    void                    *m_pRawBuffer;
    LibPEAddressT(T)        m_nRVA;
    LibPEAddressT(T)        m_nVA;
    LibPEAddressT(T)        m_nSizeInMemory;
    LibPEAddressT(T)        m_nFOA;
    LibPEAddressT(T)        m_nSizeInFile;
};

typedef PEElementT<PE32> PEElement32;
typedef PEElementT<PE64> PEElement64;

#define DECLARE_PE_ELEMENT(struct_type)     \
    virtual void * LIBPE_CALLTYPE GetRawMemory() { return PEElementT<T>::GetRawMemory(); }                  \
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetRawOffset() { return PEElementT<T>::GetRawOffset(); }      \
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetRawSize() { return PEElementT<T>::GetRawSize(); }            \
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetRVA() { return PEElementT<T>::GetRVA(); }                    \
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetVA() { return PEElementT<T>::GetVA(); }                      \
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetSizeInMemory() { return PEElementT<T>::GetSizeInMemory(); }  \
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetFOA() { return PEElementT<T>::GetFOA(); }                    \
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetSizeInFile(){ return PEElementT<T>::GetSizeInFile(); }       \
    virtual struct_type * LIBPE_CALLTYPE GetRawStruct() { return (struct_type *)PEElementT<T>::GetRawMemory(); }

LIBPE_NAMESPACE_END