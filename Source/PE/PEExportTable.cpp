#include "stdafx.h"
#include "PE/PEExportTable.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
uint32_t
PEExportTableT<T>::GetExportFunctionCount()
{
    return (uint32_t)m_vExportFunctions.size();
}

template <class T>
error_t
PEExportTableT<T>::GetExportFunctionByIndex(uint32_t nIndex, IPEExportFunctionT<T> **ppFunction)
{
    LIBPE_ASSERT_RET(NULL != ppFunction, ERR_POINTER);

    uint32_t nFunctionCount = GetExportFunctionCount();
    LIBPE_ASSERT_RET(nIndex < nFunctionCount, ERR_INVALID_ARG);

    if(NULL == m_vExportFunctions[nIndex]) {
        LIBPE_ASSERT_RET(NULL != m_pParser, ERR_FAIL);
        if(ERR_OK != m_pParser->ParseExportFunction(this, nIndex, &m_vExportFunctions[nIndex]) || NULL == m_vExportFunctions[nIndex]) {
            return ERR_FAIL;
        }
    }

    return m_vExportFunctions[nIndex].CopyTo(ppFunction);
}

template <class T>
error_t
PEExportTableT<T>::GetExportFunctionByName(const char *pFunctionName, IPEExportFunctionT<T> **ppFunction)
{
    return ERR_NOT_IMPL;
}

template <class T>
const char *
PEExportFunctionT<T>::GetName()
{
    return m_pName;
}

template <class T>
uint16_t
PEExportFunctionT<T>::GetHint()
{
    return m_nHint;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEExportTable);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEExportFunction);

LIBPE_NAMESPACE_END