#include "stdafx.h"
#include "PE/PEImportModule.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
error_t
PEImportModuleT<T>::GetImportFunctionByIndex(uint32_t nIndex, IPEImportFunctionT<T> **ppFunction)
{
    LIBPE_ASSERT_RET(NULL != ppFunction, ERR_POINTER);

    uint32_t nFunctionCount = GetImportFunctionCount();
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
PEImportModuleT<T>::GetImportFunctionByName(const char *pFunctionName, IPEImportFunctionT<T> **ppFunction)
{
    return ERR_NOT_IMPL;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEImportModule);

LIBPE_NAMESPACE_END