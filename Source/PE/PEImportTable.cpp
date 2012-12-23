#include "stdafx.h"
#include "PEImportTable.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
uint32_t
PEImportTableT<T>::GetModuleCount()
{
    return (uint32_t)m_vModules.size();
}

template <class T>
error_t
PEImportTableT<T>::GetModuleByIndex(uint32_t nModuleId, IPEImportModuleT<T> **ppImportModule)
{
    LIBPE_ASSERT_RET(NULL != ppImportModule, ERR_POINTER);

    uint32_t nModuleCount = GetModuleCount();
    LIBPE_ASSERT_RET(nModuleId < nModuleCount, ERR_INVALID_ARG);

    ModuleInfo &oInfo = m_vModules[nModuleId];
    if(NULL == oInfo.m_pImportModule) {
        LIBPE_ASSERT_RET(NULL != m_pParser, ERR_FAIL);
        if(ERR_OK != m_pParser->ParseImportModule(oInfo.m_nImportDescRVA, oInfo.m_nImportDescFOA, oInfo.m_pImportDesc, &oInfo.m_pImportModule) || NULL == oInfo.m_pImportModule) {
            return ERR_FAIL;
        }
    }

    return oInfo.m_pImportModule.CopyTo(ppImportModule);
}

template <class T>
error_t
PEImportTableT<T>::GetModuleByName(const char *pModuleName, IPEImportModuleT<T> **ppImportModule)
{
    return ERR_NOT_IMPL;
}

template <class T>
error_t
PEImportTableT<T>::GetFunctionByName(const char *pModuleName, const char *pFunctionName, IPEImportFunctionT<T> **ppImportFunction)
{
    return ERR_NOT_IMPL;
}

template <class T>
bool_t
PEImportModuleT<T>::IsBound()
{
    LibPERawImportDescriptor(T) *pImportDesc = GetRawStruct();
    LIBPE_ASSERT_RET(NULL != pImportDesc, 0);
    return (0 != pImportDesc->TimeDateStamp);
}


template <class T>
error_t
PEImportModuleT<T>::GetFunctionByIndex(uint32_t nIndex, IPEImportFunctionT<T> **ppFunction)
{
    LIBPE_ASSERT_RET(NULL != ppFunction, ERR_POINTER);

    uint32_t nFunctionCount = GetFunctionCount();
    LIBPE_ASSERT_RET(nIndex < nFunctionCount, ERR_INVALID_ARG);

    FunctionInfo &oInfo = m_vFunctions[nIndex];
    if(NULL == oInfo.m_pFunction) {
        LIBPE_ASSERT_RET(NULL != m_pParser && NULL != m_pFile && NULL != oInfo.m_pThunkData, ERR_FAIL);
        if(ERR_OK != m_pParser->ParseImportFunction(GetRawStruct(), oInfo.m_pThunkData, &oInfo.m_pFunction) || NULL == oInfo.m_pFunction) {
            return ERR_FAIL;
        }
    }

    return oInfo.m_pFunction.CopyTo(ppFunction);
}

template <class T>
error_t
PEImportModuleT<T>::GetFunctionByName(const char *pFunctionName, IPEImportFunctionT<T> **ppFunction)
{
    return ERR_NOT_IMPL;
}

template <class T>
error_t
PEImportModuleT<T>::GetRelatedImportAddressBlock(IPEImportAddressBlockT<T> **ppBlock)
{
    LIBPE_ASSERT_RET(NULL != ppBlock, ERR_POINTER);

    // However, we should parse the related IAT block here. Very useful for bounded module.
    if(NULL == m_pRelatedIABlock) {
        LIBPE_ASSERT_RET(NULL != m_pParser, ERR_FAIL);

        LibPERawImportDescriptor(T) *pImportDescriptor = GetRawStruct();
        LIBPE_ASSERT_RET(NULL != pImportDescriptor, ERR_FAIL);

        LibPEAddressT(T) nImportAddressBlockRVA = pImportDescriptor->FirstThunk;
        if(0 != nImportAddressBlockRVA) {
            LibPEAddressT(T) nImportAddressBlockFOA = m_pParser->GetFOAFromRVA(nImportAddressBlockRVA);
            if(ERR_OK != m_pParser->ParseImportAddressBlock(NULL, nImportAddressBlockRVA, nImportAddressBlockFOA, &m_pRelatedIABlock) || NULL == m_pRelatedIABlock) {
                return ERR_FAIL;
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
uint16_t  
PEImportFunctionT<T>::GetHint()
{
    LibPERawImportByName(T) *pImportByName = GetRawStruct();
    LIBPE_ASSERT_RET(NULL != pImportByName, 0);
    return pImportByName->Hint;
}

template <class T>
LibPEAddressT(T)  
PEImportFunctionT<T>::GetEntry()
{
    return NULL;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEImportTable);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEImportModule);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEImportFunction);

LIBPE_NAMESPACE_END
