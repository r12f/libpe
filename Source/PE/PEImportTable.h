#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEImportTableT :
    public IPEImportTable,
    public PEElementT<T>
{
    typedef std::vector<LibPEPtr<IPEImportModule>> ModuleList;

public:
    PEImportTableT() {}
    ~PEImportTableT() override {}

    DECLARE_PE_ELEMENT(LibPERawImportDescriptor(T))

    void InnerAddImportModule(_In_ IPEImportModule *pImportModule) { m_vModules.push_back(pImportModule); }

    UINT32 LIBPE_CALLTYPE GetModuleCount() override;
    HRESULT LIBPE_CALLTYPE GetModuleByIndex(_In_ UINT32 nModuleId, _Outptr_ IPEImportModule **ppImportModule) override;
    HRESULT LIBPE_CALLTYPE GetModuleByName(_In_ const char *pModuleName, _Outptr_ IPEImportModule **ppImportModule) override;
    HRESULT LIBPE_CALLTYPE GetFunctionByName(_In_ const char *pModuleName, const char *pFunctionName, _Outptr_ IPEImportFunction **ppImportFunction) override;

protected:
    HRESULT LoadDelayedData() override;

private:
    ModuleList m_vModules;
};

template <class T>
class PEImportModuleT :
    public IPEImportModule,
    public PEElementT<T>
{
    typedef std::vector<LibPEPtr<IPEImportFunction>> FunctionList;

public:
    PEImportModuleT() : m_pName(NULL), m_bIsFunctionParsed(false) {}
    ~PEImportModuleT() override {}

    DECLARE_PE_ELEMENT(LibPERawImportDescriptor(T))

    void InnerAddImportFunction(_In_ IPEImportFunction *pFunction) { m_vFunctions.push_back(pFunction); }
    void InnerSetName(_In_ const char *pName) { m_pName = pName; }

    LIBPE_FIELD_ACCESSOR(UINT32, Characteristics)
    LIBPE_FIELD_ACCESSOR(UINT32, OriginalFirstThunk)
    LIBPE_FIELD_ACCESSOR(UINT32, TimeDateStamp)
    LIBPE_FIELD_ACCESSOR(UINT32, ForwarderChain)
    LIBPE_FIELD_ACCESSOR(UINT32, Name)
    LIBPE_FIELD_ACCESSOR(UINT32, FirstThunk)

    BOOL LIBPE_CALLTYPE IsBound() override;
    const char * LIBPE_CALLTYPE GetName() override { return m_pName; }
    UINT32 LIBPE_CALLTYPE GetFunctionCount() override;
    HRESULT LIBPE_CALLTYPE GetFunctionByIndex(_In_ UINT32 nIndex, _Outptr_ IPEImportFunction **ppFunction) override;
    HRESULT LIBPE_CALLTYPE GetFunctionByName(_In_ const char *pFunctionName, _Outptr_ IPEImportFunction **ppFunction) override;
    HRESULT LIBPE_CALLTYPE GetRelatedImportAddressBlock(_Outptr_ IPEImportAddressBlock **ppBlock) override;

protected:
    HRESULT EnsureFunctionParsed() {
        if (m_bIsFunctionParsed) {
            return S_OK;
        }

        // If failed, do not parse twice.
        m_bIsFunctionParsed = true;

        LIBPE_CHK(NULL != m_pParser && NULL != m_pFile, E_FAIL);
        LIBPE_CHK_HR(m_pParser->ParseAllImportFunctions(this));

        return S_OK;
    }

private:
    const char                          *m_pName;
    bool                                m_bIsFunctionParsed;
    FunctionList                        m_vFunctions;
    LibPEPtr<IPEImportAddressBlock>     m_pRelatedIABlock;
};

template <class T>
class PEImportFunctionT :
    public IPEImportFunction,
    public PEElementT<T>
{
public:
    PEImportFunctionT()
        : m_bIsImportByNameParsed(false)
        , m_pImportByName(NULL)
        , m_nImportByNameRVA(LIBPE_INVALID_ADDRESS)
        , m_nImportByNameFOA(LIBPE_INVALID_ADDRESS)
        , m_nImportByNameSize(LIBPE_INVALID_SIZE)
    {}
    ~PEImportFunctionT() override {}

    DECLARE_PE_ELEMENT(LibPERawThunkData(T))

    void InnerSetRawImportByName(LibPERawImportByName(T) *pImportByName, _In_ PEAddress nRVA, _In_ PEAddress nFOA, _In_ PEAddress nSize) {
        LIBPE_CHK_RET_VOID(LIBPE_INVALID_ADDRESS != nRVA);
        LIBPE_CHK_RET_VOID(LIBPE_INVALID_ADDRESS != nFOA);
        LIBPE_CHK_RET_VOID(LIBPE_INVALID_SIZE != nSize);

        m_pImportByName = pImportByName;
        m_nImportByNameRVA = nRVA;
        m_nImportByNameFOA = nFOA;
        m_nImportByNameSize = nSize;
    }

    LIBPE_FIELD_ACCESSOR_EX(PEAddress, ForwarderString, u1.ForwarderString)
    LIBPE_FIELD_ACCESSOR_EX(PEAddress, Function, u1.Function)
    LIBPE_FIELD_ACCESSOR_EX(PEAddress, Ordinal, u1.Ordinal)
    LIBPE_FIELD_ACCESSOR_EX(PEAddress, AddressOfData, u1.AddressOfData)

    PERawImportByName * LIBPE_CALLTYPE GetRawImportByName() override;
    PEAddress GetRawImportByNameRVA();
    PEAddress GetRawImportByNameFOA();
    PEAddress GetRawImportByNameSize();

    const char * LIBPE_CALLTYPE GetName() override;
    UINT16 LIBPE_CALLTYPE GetOrdinal() override;
    
protected:
    HRESULT EnsureImportByNameParsed() {
        if (m_bIsImportByNameParsed) {
            return S_OK;
        }

        // If failed, do not parse twice.
        m_bIsImportByNameParsed = true;

        LIBPE_CHK(NULL != m_pParser && NULL != m_pFile, E_FAIL);
        LIBPE_CHK_HR(m_pParser->ParseImportFunction(this));

        return S_OK;
    }

private:
    bool m_bIsImportByNameParsed;
    LibPERawImportByName(T) *m_pImportByName;
    PEAddress m_nImportByNameRVA;
    PEAddress m_nImportByNameFOA;
    PEAddress m_nImportByNameSize;
};

typedef PEImportTableT<PE32> PEImportTable32;
typedef PEImportModuleT<PE32> PEImportModule32;
typedef PEImportFunctionT<PE32> PEImportFunction32;

typedef PEImportTableT<PE64> PEImportTable64;
typedef PEImportModuleT<PE64> PEImportModule64;
typedef PEImportFunctionT<PE64> PEImportFunction64;

LIBPE_NAMESPACE_END