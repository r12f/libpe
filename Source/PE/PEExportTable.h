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

    void SetRawFunctionList(uint32_t *pFunctionList) { m_pFunctionList = pFunctionList; }
    void SetRawNameList(uint32_t *pNameList) { m_pNameList = pNameList; }
    void SetRawNameOrdinalList(uint16_t *pNameOrdinalList) { m_pNameOrdinalList = pNameOrdinalList; }

    uint32_t * GetRawFunctionList() { return m_pFunctionList; }
    uint32_t * GetRawNameList() { return m_pNameList; }
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
    uint32_t            *m_pFunctionList;
    uint32_t            *m_pNameList;
    uint16_t            *m_pNameOrdinalList;
};

template <class T>
class PEExportFunctionT :
    public IPEExportFunctionT<T>,
    public PEElementT<T>
{
public:
    PEExportFunctionT() : m_pName(NULL), m_nHint(0) {}
    virtual ~PEExportFunctionT() {}

    DECLARE_PE_ELEMENT(void)
    LIBPE_SINGLE_THREAD_OBJECT()

    void SetRawName(const char *pName) { m_pName = pName; }
    void SetRawHint(uint16_t nHint) { m_nHint = nHint; }

    virtual const char * LIBPE_CALLTYPE GetName();
    virtual uint16_t LIBPE_CALLTYPE GetHint();

private:
    const char *    m_pName;
    uint16_t        m_nHint;
};

typedef PEExportTableT<PE32> PEExportTable32;
typedef PEExportFunctionT<PE32> PEExportFunction32;

typedef PEExportTableT<PE64> PEExportTable64;
typedef PEExportFunctionT<PE64> PEExportFunction64;

LIBPE_NAMESPACE_END