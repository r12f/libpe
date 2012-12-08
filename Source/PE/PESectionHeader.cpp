#include "stdafx.h"
#include "PE/PESectionHeader.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
error_t
PESectionHeaderT<T>::GetSection(IPESectionT<T> **ppSection)
{
    if(NULL != m_pSection) {
        return m_pSection.CopyTo(ppSection);
    }

    LIBPE_ASSERT_RET(NULL != m_pSectionHeader, ERR_FAIL);
    if(ERR_OK != m_pParser->ParseSection(m_pSectionHeader, &m_pSection) || NULL == m_pSection) {
        return ERR_FAIL;
    }

    return m_pSection.CopyTo(ppSection);
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PESectionHeader);

LIBPE_NAMESPACE_END