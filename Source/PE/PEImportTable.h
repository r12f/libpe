#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEImportTableT :
    public IPEImportTable,
    public PEElementT<T>
{
    struct ModuleInfo {
        PEAddress                       m_nImportDescRVA;
        PEAddress                       m_nImportDescFOA;
        LibPERawImportDescriptor(T)     *m_pImportDesc;
        LibPEPtr<IPEImportModule>       m_pImportModule;
    };
    typedef std::vector<ModuleInfo> ModuleList;

public:
    PEImportTableT() {}
    virtual ~PEImportTableT() {}

    DECLARE_PE_ELEMENT(LibPERawImportDescriptor(T))

    void InnerAddImportDescriptor(PEAddress nImportDescRVA, PEAddress nImportDescFOA, LibPERawImportDescriptor(T) *pImportDesc) {
        LIBPE_ASSERT_RET_VOID(0 != nImportDescRVA && 0 != nImportDescFOA && NULL != pImportDesc);
        ModuleInfo oInfo;
        oInfo.m_nImportDescRVA = nImportDescRVA;
        oInfo.m_nImportDescFOA = nImportDescFOA;
        oInfo.m_pImportDesc = pImportDesc;
        m_vModules.push_back(oInfo);
        return;
    }

    virtual UINT32 LIBPE_CALLTYPE GetModuleCount();
    virtual HRESULT LIBPE_CALLTYPE GetModuleByIndex(UINT32 nModuleId, IPEImportModule **ppImportModule);
    virtual HRESULT LIBPE_CALLTYPE GetModuleByName(const char *pModuleName, IPEImportModule **ppImportModule);
    virtual HRESULT LIBPE_CALLTYPE GetFunctionByName(const char *pModuleName, const char *pFunctionName, IPEImportFunction **ppImportFunction);

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
    virtual ~PEImportModuleT() {}

    DECLARE_PE_ELEMENT(LibPERawImportDescriptor(T))

    void InnerAddImportFunction(IPEImportFunction *pFunction) {
        LIBPE_ASSERT_RET_VOID(NULL != pFunction);
        m_vFunctions.push_back(pFunction);
    }

    void InnerSetName(const char *pName) { m_pName = pName; }

    LIBPE_FIELD_ACCESSOR(UINT32, Characteristics)
    LIBPE_FIELD_ACCESSOR(UINT32, OriginalFirstThunk)
    LIBPE_FIELD_ACCESSOR(UINT32, TimeDateStamp)
    LIBPE_FIELD_ACCESSOR(UINT32, ForwarderChain)
    LIBPE_FIELD_ACCESSOR(UINT32, Name)
    LIBPE_FIELD_ACCESSOR(UINT32, FirstThunk)

    virtual BOOL LIBPE_CALLTYPE IsBound();
    virtual const char * LIBPE_CALLTYPE GetName() { return m_pName; }
    virtual UINT32 LIBPE_CALLTYPE GetFunctionCount();
    virtual HRESULT LIBPE_CALLTYPE GetFunctionByIndex(UINT32 nIndex, IPEImportFunction **ppFunction);
    virtual HRESULT LIBPE_CALLTYPE GetFunctionByName(const char *pFunctionName, IPEImportFunction **ppFunction);
    virtual HRESULT LIBPE_CALLTYPE GetRelatedImportAddressBlock(IPEImportAddressBlock **ppBlock);

protected:
    HRESULT EnsureFunctionParsed() {
        if (m_bIsFunctionParsed) {
            return S_OK;
        }

        // If failed, do not parse twice.
        m_bIsFunctionParsed = true;

        LIBPE_ASSERT_RET(NULL != m_pParser && NULL != m_pFile, E_FAIL);
        LIBPE_ASSERT_RET(SUCCEEDED(m_pParser->ParseImportFunctionsInModule(this)), E_FAIL);

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
    PEImportFunctionT() : m_bIsImportByNameParsed(false), m_pImportByName(NULL), m_nImportByNameRVA(0), m_nImportByNameFOA(0), m_nImportByNameSize(0) {}
    virtual ~PEImportFunctionT() {}

    DECLARE_PE_ELEMENT(LibPERawThunkData(T))

    void InnerSetRawImportByName(LibPERawImportByName(T) *pImportByName, PEAddress nRVA, PEAddress nFOA, PEAddress nSize) {
        m_pImportByName = pImportByName;
        m_nImportByNameRVA = nRVA;
        m_nImportByNameFOA = nFOA;
        m_nImportByNameSize = nSize;
    }

    LIBPE_FIELD_ACCESSOR_EX(PEAddress, ForwarderString, u1.ForwarderString)
    LIBPE_FIELD_ACCESSOR_EX(PEAddress, Function, u1.Function)
    LIBPE_FIELD_ACCESSOR_EX(PEAddress, Ordinal, u1.Ordinal)
    LIBPE_FIELD_ACCESSOR_EX(PEAddress, AddressOfData, u1.AddressOfData)

    virtual PERawImportByName * GetRawImportByName();
    virtual PEAddress GetRawImportByNameRVA();
    virtual PEAddress GetRawImportByNameFOA();
    virtual PEAddress GetRawImportByNameSize();

    virtual const char * GetName();
    virtual UINT16 GetOrdinal();
    
protected:
    HRESULT EnsureImportByNameParsed() {
        if (m_bIsImportByNameParsed) {
            return S_OK;
        }

        // If failed, do not parse twice.
        m_bIsImportByNameParsed = true;

        LIBPE_ASSERT_RET(NULL != m_pParser && NULL != m_pFile, E_FAIL);
        LIBPE_ASSERT_RET(SUCCEEDED(m_pParser->ParseImportFunction(this)), E_FAIL);

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