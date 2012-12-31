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
        LibPEPtr<IPEImportModule>   m_pImportModule;
    };
    typedef std::vector<ModuleInfo> ModuleList;

public:
    PEImportTableT() {}
    virtual ~PEImportTableT() {}

    DECLARE_PE_ELEMENT(LibPERawImportDescriptor(T))
    LIBPE_SINGLE_THREAD_OBJECT()

    void AddImportDescriptor(PEAddress nImportDescRVA, PEAddress nImportDescFOA, LibPERawImportDescriptor(T) *pImportDesc) {
        LIBPE_ASSERT_RET_VOID(0 != nImportDescRVA && 0 != nImportDescFOA && NULL != pImportDesc);
        ModuleInfo oInfo;
        oInfo.m_nImportDescRVA = nImportDescRVA;
        oInfo.m_nImportDescFOA = nImportDescFOA;
        oInfo.m_pImportDesc = pImportDesc;
        m_vModules.push_back(oInfo);
        return;
    }

    virtual uint32_t LIBPE_CALLTYPE GetModuleCount();
    virtual error_t LIBPE_CALLTYPE GetModuleByIndex(uint32_t nModuleId, IPEImportModule **ppImportModule);
    virtual error_t LIBPE_CALLTYPE GetModuleByName(const char *pModuleName, IPEImportModule **ppImportModule);
    virtual error_t LIBPE_CALLTYPE GetFunctionByName(const char *pModuleName, const char *pFunctionName, IPEImportFunction **ppImportFunction);

private:
    ModuleList m_vModules;
};

template <class T>
class PEImportModuleT :
    public IPEImportModule,
    public PEElementT<T>
{
    struct FunctionInfo {
        LibPERawThunkData(T)            *m_pThunkData;
        LibPEPtr<IPEImportFunction> m_pFunction;
    };
    typedef std::vector<FunctionInfo> FunctionList;

public:
    PEImportModuleT() {}
    virtual ~PEImportModuleT() {}

    DECLARE_PE_ELEMENT(LibPERawImportDescriptor(T))
    LIBPE_SINGLE_THREAD_OBJECT()

    void AddImportFunctionThunk(LibPERawThunkData(T) *pThunk) {
        FunctionInfo oInfo;
        oInfo.m_pThunkData = pThunk;
        m_vFunctions.push_back(oInfo);
    }

    void InnerSetName(const char *pName) { m_pName = pName; }

    virtual bool_t LIBPE_CALLTYPE IsBound();
    virtual const char * LIBPE_CALLTYPE GetName() { return m_pName; }
    virtual uint32_t LIBPE_CALLTYPE GetFunctionCount() { return (uint32_t)m_vFunctions.size(); }
    virtual error_t LIBPE_CALLTYPE GetFunctionByIndex(uint32_t nIndex, IPEImportFunction **ppFunction);
    virtual error_t LIBPE_CALLTYPE GetFunctionByName(const char *pFunctionName, IPEImportFunction **ppFunction);
    virtual error_t LIBPE_CALLTYPE GetRelatedImportAddressBlock(IPEImportAddressBlock **ppBlock);

private:
    const char                          *m_pName;
    FunctionList                        m_vFunctions;
    LibPEPtr<IPEImportAddressBlock> m_pRelatedIABlock;
};

template <class T>
class PEImportFunctionT :
    public IPEImportFunction,
    public PEElementT<T>
{
public:
    PEImportFunctionT() : m_pThunkData(NULL) {}
    virtual ~PEImportFunctionT() {}

    DECLARE_PE_ELEMENT(LibPERawImportByName(T))
    LIBPE_SINGLE_THREAD_OBJECT()

    void InnerSetThunkData(LibPERawThunkData(T) *pThunkData) { m_pThunkData = pThunkData; }
    void InnerSetOrdinal(uint16_t nOrdinal) { m_nOrdinal = nOrdinal; }

    virtual LibPERawThunkData(T) * LIBPE_CALLTYPE GetRawThunkData();
    virtual const char * LIBPE_CALLTYPE GetName();
    virtual uint16_t LIBPE_CALLTYPE GetHint();
    virtual PEAddress LIBPE_CALLTYPE GetEntry();

private:
    LibPERawThunkData(T) *m_pThunkData;
};

typedef PEImportTableT<PE32> PEImportTable32;
typedef PEImportModuleT<PE32> PEImportModule32;
typedef PEImportFunctionT<PE32> PEImportFunction32;

typedef PEImportTableT<PE64> PEImportTable64;
typedef PEImportModuleT<PE64> PEImportModule64;
typedef PEImportFunctionT<PE64> PEImportFunction64;

LIBPE_NAMESPACE_END