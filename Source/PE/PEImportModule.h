#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

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

typedef PEImportModuleT<PE32> PEImportModule32;
typedef PEImportModuleT<PE64> PEImportModule64;

LIBPE_NAMESPACE_END