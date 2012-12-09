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

typedef PEImportTableT<PE32> PEImportTable32;
typedef PEImportTableT<PE64> PEImportTable64;

LIBPE_NAMESPACE_END