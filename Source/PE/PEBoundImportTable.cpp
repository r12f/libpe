#include "stdafx.h"
#include "PE/PEBoundImportTable.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
HRESULT
PEBoundImportTableT<T>::InnerAddBoundImportModule(IPEBoundImportModule *pBoundImportModule)
{
    LIBPE_CHK(NULL != pBoundImportModule, E_INVALIDARG);

    HRESULT hr = S_OK;

    LIBPE_HR_TRY_BEGIN(hr) {
        m_vBoundImportModules.push_back(pBoundImportModule);
    } LIBPE_HR_TRY_END();

    return hr;
}

template <class T>
UINT32
PEBoundImportTableT<T>::GetBoundImportModuleCount()
{
    LIBPE_CHK_HR_RET(EnsureBoundImportModulesParsed(), 0);
    return (UINT32)m_vBoundImportModules.size();
}

template <class T>
HRESULT
PEBoundImportTableT<T>::GetBoundImportModuleByIndex(UINT32 nIndex, IPEBoundImportModule **ppBoundImportModule)
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

    LIBPE_CHK(NULL != m_pParser, E_UNEXPECTED);
    LIBPE_CHK_HR(m_pParser->ParseBoundImportModules(this));

    return S_OK;
}

template <class T>
HRESULT 
PEBoundImportModuleT<T>::InnerAddBoundForwarder(IPEBoundForwarder *pBoundForwarder)
{
    LIBPE_CHK(NULL != pBoundForwarder, E_INVALIDARG);

    HRESULT hr = S_OK;

    LIBPE_HR_TRY_BEGIN(hr) {
        m_vBoundForwarders.push_back(pBoundForwarder);
    } LIBPE_HR_TRY_END();

    return hr;
}

template <class T>
UINT32 
PEBoundImportModuleT<T>::GetBoundForwarderCount()
{
    return 0;
}

template <class T>
HRESULT 
PEBoundImportModuleT<T>::GetBoundForwarderByIndex(UINT32 nIndex, IPEBoundForwarder **ppBoundForwarder)
{
    return S_OK;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEBoundImportTableT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEBoundImportModuleT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEBoundForwarderT);

LIBPE_NAMESPACE_END
