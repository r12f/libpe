#include "stdafx.h"
#include "PE/PEBoundImportTable.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
UINT32
PEBoundImportTableT<T>::GetBoundImportModuleCount()
{
    LIBPE_CHK_HR_RET(EnsureBoundImportModulesParsed(), 0);
    return (UINT32)m_vBoundImportModules.size();
}

template <class T>
HRESULT
PEBoundImportTableT<T>::GetBoundImportModuleByIndex(_In_ UINT32 nIndex, _Outptr_ IPEBoundImportModule **ppBoundImportModule)
{
    LIBPE_CHK(NULL != ppBoundImportModule, E_POINTER);
    LIBPE_CHK_HR(EnsureBoundImportModulesParsed());

    UINT32 nBoundImportModuleCount = GetBoundImportModuleCount();
    LIBPE_CHK(nIndex < nBoundImportModuleCount, E_INVALIDARG);

    return m_vBoundImportModules[nIndex].CopyTo(ppBoundImportModule);
}

template <class T>
HRESULT
PEBoundImportTableT<T>::EnsureBoundImportModulesParsed()
{
    if (m_bIsBoundImportModulesParsed) {
        return S_OK;
    }

    m_bIsBoundImportModulesParsed = true;

    LIBPE_STRICTCHK(NULL != m_pParser);
    LIBPE_CHK_HR(m_pParser->ParseBoundImportModules(this));

    return S_OK;
}

template <class T>
UINT32 
PEBoundImportModuleT<T>::GetBoundForwarderCount()
{
    return 0;
}

template <class T>
HRESULT 
PEBoundImportModuleT<T>::GetBoundForwarderByIndex(_In_ UINT32 nIndex, _Outptr_ IPEBoundForwarder **ppBoundForwarder)
{
    return S_OK;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEBoundImportTableT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEBoundImportModuleT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEBoundForwarderT);

LIBPE_NAMESPACE_END
