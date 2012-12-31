#include "stdafx.h"
#include "PEImportTable.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
UINT32
PEImportTableT<T>::GetModuleCount()
{
    return (UINT32)m_vModules.size();
}

template <class T>
HRESULT
PEImportTableT<T>::GetModuleByIndex(UINT32 nModuleId, IPEImportModule **ppImportModule)
{
    LIBPE_ASSERT_RET(NULL != ppImportModule, E_POINTER);

    UINT32 nModuleCount = GetModuleCount();
    LIBPE_ASSERT_RET(nModuleId < nModuleCount, E_INVALIDARG);

    ModuleInfo &oInfo = m_vModules[nModuleId];
    if(NULL == oInfo.m_pImportModule) {
        LIBPE_ASSERT_RET(NULL != m_pParser, E_FAIL);
        if(FAILED(m_pParser->ParseImportModule(oInfo.m_nImportDescRVA, oInfo.m_nImportDescFOA, oInfo.m_pImportDesc, &oInfo.m_pImportModule)) || NULL == oInfo.m_pImportModule) {
            return E_FAIL;
        }
    }

    return oInfo.m_pImportModule.CopyTo(ppImportModule);
}

template <class T>
HRESULT
PEImportTableT<T>::GetModuleByName(const char *pModuleName, IPEImportModule **ppImportModule)
{
    return E_NOTIMPL;
}

template <class T>
HRESULT
PEImportTableT<T>::GetFunctionByName(const char *pModuleName, const char *pFunctionName, IPEImportFunction **ppImportFunction)
{
    return E_NOTIMPL;
}

template <class T>
BOOL
PEImportModuleT<T>::IsBound()
{
    LibPERawImportDescriptor(T) *pImportDesc = GetRawStruct();
    LIBPE_ASSERT_RET(NULL != pImportDesc, 0);
    return (0 != pImportDesc->TimeDateStamp);
}


template <class T>
HRESULT
PEImportModuleT<T>::GetFunctionByIndex(UINT32 nIndex, IPEImportFunction **ppFunction)
{
    LIBPE_ASSERT_RET(NULL != ppFunction, E_POINTER);

    UINT32 nFunctionCount = GetFunctionCount();
    LIBPE_ASSERT_RET(nIndex < nFunctionCount, E_INVALIDARG);

    FunctionInfo &oInfo = m_vFunctions[nIndex];
    if(NULL == oInfo.m_pFunction) {
        LIBPE_ASSERT_RET(NULL != m_pParser && NULL != m_pFile && NULL != oInfo.m_pThunkData, E_FAIL);
        if(FAILED(m_pParser->ParseImportFunction(GetRawStruct(), oInfo.m_pThunkData, &oInfo.m_pFunction)) || NULL == oInfo.m_pFunction) {
            return E_FAIL;
        }
    }

    return oInfo.m_pFunction.CopyTo(ppFunction);
}

template <class T>
HRESULT
PEImportModuleT<T>::GetFunctionByName(const char *pFunctionName, IPEImportFunction **ppFunction)
{
    return E_NOTIMPL;
}

template <class T>
HRESULT
PEImportModuleT<T>::GetRelatedImportAddressBlock(IPEImportAddressBlock **ppBlock)
{
    LIBPE_ASSERT_RET(NULL != ppBlock, E_POINTER);

    // However, we should parse the related IAT block here. Very useful for bounded module.
    if(NULL == m_pRelatedIABlock) {
        LIBPE_ASSERT_RET(NULL != m_pParser, E_FAIL);

        LibPERawImportDescriptor(T) *pImportDescriptor = GetRawStruct();
        LIBPE_ASSERT_RET(NULL != pImportDescriptor, E_FAIL);

        PEAddress nImportAddressBlockRVA = pImportDescriptor->FirstThunk;
        if(0 != nImportAddressBlockRVA) {
            PEAddress nImportAddressBlockFOA = m_pParser->GetFOAFromRVA(nImportAddressBlockRVA);
            if(FAILED(m_pParser->ParseImportAddressBlock(NULL, nImportAddressBlockRVA, nImportAddressBlockFOA, &m_pRelatedIABlock)) || NULL == m_pRelatedIABlock) {
                return E_FAIL;
            }
        }
    }

    return m_pRelatedIABlock.CopyTo(ppBlock);
}

template <class T>
LibPERawThunkData(T) *  
PEImportFunctionT<T>::GetRawThunkData()
{
    return m_pThunkData;
}

template <class T>
const char *  
PEImportFunctionT<T>::GetName()
{
    LibPERawImportByName(T) *pImportByName = GetRawStruct();
    LIBPE_ASSERT_RET(NULL != pImportByName, 0);
    return (const char *)pImportByName->Name;
}

template <class T>
UINT16  
PEImportFunctionT<T>::GetHint()
{
    LibPERawImportByName(T) *pImportByName = GetRawStruct();
    LIBPE_ASSERT_RET(NULL != pImportByName, 0);
    return pImportByName->Hint;
}

template <class T>
PEAddress  
PEImportFunctionT<T>::GetEntry()
{
    return NULL;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEImportTableT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEImportModuleT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEImportFunctionT);

LIBPE_NAMESPACE_END
