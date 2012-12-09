#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEImportFunctionT :
    public IPEImportFunctionT<T>,
    public PEElementT<T>
{
public:
    PEImportFunctionT()
        : m_pThunkData(NULL), m_pImportByName(NULL), m_pFunctionName(NULL), m_nOrdinal(0)
    {}
    virtual ~PEImportFunctionT() {}

    DECLARE_PE_ELEMENT(LibPERawImportByName(T))
    LIBPE_SINGLE_THREAD_OBJECT()

    void SetRawThunkData(LibPERawThunkData(T) *pThunkData) { m_pThunkData = pThunkData; }
    void SetRawImportByName(LibPERawImportByName(T) *pImportByName) { m_pImportByName = pImportByName; }
    void SetRawName(const char *pFunctionName) { m_pFunctionName = pFunctionName; }
    void SetRawOrdinal(uint16_t nOrdinal) { m_nOrdinal = nOrdinal; }

    virtual LibPERawThunkData(T) * LIBPE_CALLTYPE GetRawThunkData();
    virtual const char * LIBPE_CALLTYPE GetName();
    virtual uint16_t LIBPE_CALLTYPE GetOrdinal();
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetEntry();

private:
    LibPERawThunkData(T) *m_pThunkData;
    LibPERawImportByName(T) *m_pImportByName;
    const char *m_pFunctionName;
    uint16_t m_nOrdinal;
};

typedef PEImportFunctionT<PE32> PEImportFunction32;
typedef PEImportFunctionT<PE64> PEImportFunction64;

LIBPE_NAMESPACE_END