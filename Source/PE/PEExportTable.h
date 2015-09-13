#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEExportTableT :
    public IPEExportTable,
    public PEElementT<T>
{
    typedef std::vector<LibPEPtr<IPEExportFunction>> FunctionList;

public:
    PEExportTableT() : m_pFunctionList(NULL), m_pNameList(NULL), m_pNameOrdinalList(NULL) {}
    ~PEExportTableT() override {}

    DECLARE_PE_ELEMENT(LibPERawExportDirectory(T))

    void InnerSetFunctionList(_In_ UINT32 *pFunctionList) { m_pFunctionList = pFunctionList; }
    void InnerSetNameList(_In_ UINT32 *pNameList) { m_pNameList = pNameList; }
    void InnerSetNameOrdinalList(_In_ UINT16 *pNameOrdinalList) { m_pNameOrdinalList = pNameOrdinalList; }

    UINT32 * GetRawFunctionList() { return m_pFunctionList; }
    UINT32 * GetRawNameList() { return m_pNameList; }
    UINT16 * GetRawNameOrdinalList() { return m_pNameOrdinalList; }

    bool PrepareForUsing() {
        LibPERawExportDirectory(T) *pExportDirectory = GetRawStruct();
        LIBPE_CHK(NULL != pExportDirectory, false);
        m_vExportFunctions.resize(pExportDirectory->NumberOfFunctions, NULL);
        return true;
    }

    LIBPE_FIELD_ACCESSOR(UINT32, Characteristics)
    LIBPE_FIELD_ACCESSOR(UINT32, TimeDateStamp)
    LIBPE_FIELD_ACCESSOR(UINT16, MajorVersion)
    LIBPE_FIELD_ACCESSOR(UINT16, MinorVersion)
    LIBPE_FIELD_ACCESSOR(UINT32, Name)
    LIBPE_FIELD_ACCESSOR(UINT32, Base)
    LIBPE_FIELD_ACCESSOR(UINT32, NumberOfFunctions)
    LIBPE_FIELD_ACCESSOR(UINT32, NumberOfNames)
    LIBPE_FIELD_ACCESSOR(UINT32, AddressOfFunctions)
    LIBPE_FIELD_ACCESSOR(UINT32, AddressOfNames)
    LIBPE_FIELD_ACCESSOR(UINT32, AddressOfNameOrdinals)

    UINT32 LIBPE_CALLTYPE GetFunctionCount() override;
    HRESULT LIBPE_CALLTYPE GetFunctionByIndex(_In_ UINT32 nIndex, _Outptr_ IPEExportFunction **ppFunction) override;
    HRESULT LIBPE_CALLTYPE GetFunctionByName(_In_ const char *pFunctionName, _Outptr_ IPEExportFunction **ppFunction) override;

private:
    FunctionList      m_vExportFunctions;
    UINT32            *m_pFunctionList;
    UINT32            *m_pNameList;
    UINT16            *m_pNameOrdinalList;
};

template <class T>
class PEExportFunctionT :
    public IPEExportFunction,
    public PEElementT<T>
{
public:
    PEExportFunctionT() : m_pName(NULL), m_nOrdinal(0) {}
    ~PEExportFunctionT() override {}

    DECLARE_PE_ELEMENT_NO_SIZE(void)

    void InnerSetName(_In_ const char *pName) { m_pName = pName; }
    void InnerSetOrdinal(_In_ UINT16 nOrdinal) { m_nOrdinal = nOrdinal; }

    const char * LIBPE_CALLTYPE GetName() override;
    UINT16 LIBPE_CALLTYPE GetOrdinal() override;

private:
    const char      *m_pName;
    UINT16          m_nOrdinal;
};

typedef PEExportTableT<PE32> PEExportTable32;
typedef PEExportFunctionT<PE32> PEExportFunction32;

typedef PEExportTableT<PE64> PEExportTable64;
typedef PEExportFunctionT<PE64> PEExportFunction64;

LIBPE_NAMESPACE_END