#include "stdafx.h"
#include "PEImportTable.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
uint32_t
PEImportTableT<T>::GetImportModuleCount()
{
    return (uint32_t)m_vModules.size();
}

template <class T>
error_t
PEImportTableT<T>::GetImportModuleByIndex(uint32_t nModuleId, IPEImportModuleT<T> **ppImportModule)
{
    LIBPE_ASSERT_RET(NULL != ppImportModule, ERR_POINTER);

    uint32_t nModuleCount = GetImportModuleCount();
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
PEImportTableT<T>::GetImportModuleByName(const char *pModuleName, IPEImportModuleT<T> **ppImportModule)
{
    return ERR_NOT_IMPL;
}


template <class T>
error_t
PEImportTableT<T>::GetImportFunctionByName(const char *pModuleName, const char *pFunctionName, IPEImportFunctionT<T> **ppImportFunction)
{
    return ERR_NOT_IMPL;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEImportTable);

LIBPE_NAMESPACE_END
