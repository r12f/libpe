#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEExportTableT :
    public IPEExportTableT<T>,
    public PEElementT<T>
{
    typedef std::vector<LibPEPtr<IPEExportFunctionT<T>>> FunctionList;

public:
    PEExportTableT() : m_pFunctionList(NULL), m_pNameList(NULL), m_pNameOrdinalList(NULL) {}
    virtual ~PEExportTableT() {}

    DECLARE_PE_ELEMENT(LibPERawExportDirectory(T))
    LIBPE_SINGLE_THREAD_OBJECT()

    void SetRawFunctionList(LibPEAddressT(T) *pFunctionList) { m_pFunctionList = pFunctionList; }
    void SetRawNameList(LibPEAddressT(T) *pNameList) { m_pNameList = pNameList; }
    void SetRawNameOrdinalList(uint16_t *pNameOrdinalList) { m_pNameOrdinalList = pNameOrdinalList; }

    LibPEAddressT(T) * GetRawFunctionList() { return m_pFunctionList; }
    LibPEAddressT(T) * GetRawNameList() { return m_pNameList; }
    uint16_t * GetRawNameOrdinalList() { return m_pNameOrdinalList; }

    bool_t PrepareForUsing() {
        LibPERawExportDirectory(T) *pExportDirectory = GetRawStruct();
        LIBPE_ASSERT_RET(NULL != pExportDirectory, false);
        m_vExportFunctions.resize(pExportDirectory->NumberOfFunctions, NULL);
        return true;
    }

    virtual uint32_t LIBPE_CALLTYPE GetExportFunctionCount();
    virtual error_t LIBPE_CALLTYPE GetExportFunctionByIndex(uint32_t nIndex, IPEExportFunctionT<T> **ppFunction);
    virtual error_t LIBPE_CALLTYPE GetExportFunctionByName(const char *pFunctionName, IPEExportFunctionT<T> **ppFunction);

private:
    FunctionList        m_vExportFunctions;
    LibPEAddressT(T)    *m_pFunctionList;
    LibPEAddressT(T)    *m_pNameList;
    uint16_t            *m_pNameOrdinalList;
};

typedef PEExportTableT<PE32> PEExportTable32;
typedef PEExportTableT<PE64> PEExportTable64;

LIBPE_NAMESPACE_END