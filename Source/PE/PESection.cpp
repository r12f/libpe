#include "stdafx.h"
#include "PE/PESection.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
error_t
PESectionHeaderT<T>::GetSection(IPESection **ppSection)
{
    if(NULL == m_pSection) {
        LIBPE_ASSERT_RET(NULL != m_pParser, ERR_FAIL);
        if(ERR_OK != m_pParser->ParseSection(GetRawStruct(), &m_pSection) || NULL == m_pSection) {
            return ERR_FAIL;
        }
    }

    return m_pSection.CopyTo(ppSection);
}

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

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PESectionHeaderT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PESectionT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEOverlayT);

LIBPE_NAMESPACE_END