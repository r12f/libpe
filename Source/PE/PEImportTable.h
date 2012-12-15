#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PEImportTableT :
    public IPEImportTableT<T>,
    public PEElementT<T>
{
    struct ModuleInfo {
        LibPEAddressT(T)                m_nImportDescRVA;
        LibPEAddressT(T)                m_nImportDescFOA;
        LibPERawImportDescriptor(T)     *m_pImportDesc;
        LibPEPtr<IPEImportModuleT<T>>   m_pImportModule;
    };
    typedef std::vector<ModuleInfo> ModuleList;

public:
    PEImportTableT() {}
    virtual ~PEImportTableT() {}

    DECLARE_PE_ELEMENT(LibPERawImportDescriptor(T))
    LIBPE_SINGLE_THREAD_OBJECT()

    void AddImportDescriptor(LibPEAddressT(T) nImportDescRVA, LibPEAddressT(T) nImportDescFOA, LibPERawImportDescriptor(T) *pImportDesc) {
        LIBPE_ASSERT_RET_VOID(0 != nImportDescRVA && 0 != nImportDescFOA && NULL != pImportDesc);
        ModuleInfo oInfo;
        oInfo.m_nImportDescRVA = nImportDescRVA;
        oInfo.m_nImportDescFOA = nImportDescFOA;
        oInfo.m_pImportDesc = pImportDesc;
        m_vModules.push_back(oInfo);
        return;
    }

    virtual uint32_t LIBPE_CALLTYPE GetImportModuleCount();
    virtual error_t LIBPE_CALLTYPE GetImportModuleByIndex(uint32_t nModuleId, IPEImportModuleT<T> **ppImportModule);
    virtual error_t LIBPE_CALLTYPE GetImportModuleByName(const char *pModuleName, IPEImportModuleT<T> **ppImportModule);
    virtual error_t LIBPE_CALLTYPE GetImportFunctionByName(const char *pModuleName, const char *pFunctionName, IPEImportFunctionT<T> **ppImportFunction);

private:
    ModuleList m_vModules;
};

template <class T>
class PEImportModuleT :
    public IPEImportModuleT<T>,
    public PEElementT<T>
{
    struct FunctionInfo {
        LibPERawThunkData(T)            *m_pThunkData;
        LibPEPtr<IPEImportFunctionT<T>> m_pFunction;
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

    void SetRawName(const char *pName) { m_pName = pName; }

    virtual const char * LIBPE_CALLTYPE GetName() { return m_pName; }
    virtual uint32_t LIBPE_CALLTYPE GetImportFunctionCount() { return (uint32_t)m_vFunctions.size(); }
    virtual error_t LIBPE_CALLTYPE GetImportFunctionByIndex(uint32_t nIndex, IPEImportFunctionT<T> **ppFunction);
    virtual error_t LIBPE_CALLTYPE GetImportFunctionByName(const char *pFunctionName, IPEImportFunctionT<T> **ppFunction);

private:
    const char                      *m_pName;
    FunctionList                    m_vFunctions;
};

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
    virtual uint16_t LIBPE_CALLTYPE GetHint();
    virtual LibPEAddressT(T) LIBPE_CALLTYPE GetEntry();

private:
    LibPERawThunkData(T) *m_pThunkData;
    LibPERawImportByName(T) *m_pImportByName;
    const char *m_pFunctionName;
    uint16_t m_nOrdinal;
};

typedef PEImportTableT<PE32> PEImportTable32;
typedef PEImportModuleT<PE32> PEImportModule32;
typedef PEImportFunctionT<PE32> PEImportFunction32;

typedef PEImportTableT<PE64> PEImportTable64;
typedef PEImportModuleT<PE64> PEImportModule64;
typedef PEImportFunctionT<PE64> PEImportFunction64;

LIBPE_NAMESPACE_END