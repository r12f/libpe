#include "stdafx.h"
#include "PE/PESection.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
HRESULT
PESectionHeaderT<T>::GetSection(_Outptr_ IPESection **ppSection)
{
    if(NULL == m_pSection) {
        LIBPE_STRICTCHK(NULL != m_pParser);
        if(FAILED(m_pParser->ParseSection(GetRawStruct(), &m_pSection)) || NULL == m_pSection) {
            return E_FAIL;
        }
    }

    return m_pSection.CopyTo(ppSection);
}

template <class T>
const char *
PESectionT<T>::GetName()
{
    LIBPE_CHK(NULL != m_pSectionHeader, NULL);
    return (const char *)m_pSectionHeader->Name;
}

template <class T>
HRESULT
PESectionT<T>::GetRelocations()
{
    return E_NOTIMPL;
}

template <class T>
HRESULT
PESectionT<T>::GetLineNumbers()
{
    return E_NOTIMPL;
}

template <class T>
UINT32
PESectionT<T>::GetCharacteristics()
{
    LIBPE_CHK(NULL != m_pSectionHeader, 0);
    return m_pSectionHeader->Characteristics;
}

template <class T>
HRESULT
PESectionT<T>::SetName(_In_ const char *pName)
{
    return S_OK;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PESectionHeaderT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PESectionT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEOverlayT);

LIBPE_NAMESPACE_END