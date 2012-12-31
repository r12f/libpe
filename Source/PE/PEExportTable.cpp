#include "stdafx.h"
#include "PE/PEExportTable.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
UINT32
PEExportTableT<T>::GetFunctionCount()
{
    return (UINT32)m_vExportFunctions.size();
}

template <class T>
HRESULT
PEExportTableT<T>::GetFunctionByIndex(UINT32 nIndex, IPEExportFunction **ppFunction)
{
    LIBPE_ASSERT_RET(NULL != ppFunction, E_POINTER);

    UINT32 nFunctionCount = GetFunctionCount();
    LIBPE_ASSERT_RET(nIndex < nFunctionCount, E_INVALIDARG);

    if(NULL == m_vExportFunctions[nIndex]) {
        LIBPE_ASSERT_RET(NULL != m_pParser, E_FAIL);
        if(FAILED(m_pParser->ParseExportFunction(this, nIndex, &m_vExportFunctions[nIndex])) || NULL == m_vExportFunctions[nIndex]) {
            return E_FAIL;
        }
    }

    return m_vExportFunctions[nIndex].CopyTo(ppFunction);
}

template <class T>
HRESULT
PEExportTableT<T>::GetFunctionByName(const char *pFunctionName, IPEExportFunction **ppFunction)
{
    return E_NOTIMPL;
}

template <class T>
const char *
PEExportFunctionT<T>::GetName()
{
    return m_pName;
}

template <class T>
UINT16
PEExportFunctionT<T>::GetHint()
{
    return m_nHint;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEExportTableT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEExportFunctionT);

LIBPE_NAMESPACE_END