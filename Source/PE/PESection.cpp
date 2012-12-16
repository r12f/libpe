#include "stdafx.h"
#include "PE/PESection.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
const char *
PESectionT<T>::GetName()
{
    LIBPE_ASSERT_RET(NULL != m_pSectionHeader, "");
    return (const char *)m_pSectionHeader->Name;
}

template <class T>
error_t
PESectionT<T>::GetRelocations()
{
    return ERR_NOT_IMPL;
}

template <class T>
error_t
PESectionT<T>::GetLineNumbers()
{
    return ERR_NOT_IMPL;
}

template <class T>
uint32_t
PESectionT<T>::GetCharacteristics()
{
    LIBPE_ASSERT_RET(NULL != m_pSectionHeader, 0);
    return m_pSectionHeader->Characteristics;
}

template <class T>
error_t
PESectionT<T>::SetName(const char *pName)
{
    return ERR_OK;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PESection);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEExtraData);

LIBPE_NAMESPACE_END