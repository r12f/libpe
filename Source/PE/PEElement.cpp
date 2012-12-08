#include "stdafx.h"
#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
void *
PEElementT<T>::GetRawMemory()
{
    LIBPE_ASSERT_RET(NULL != m_pParser, NULL);
    return m_pParser->GetRawMemory(GetRawAddress(), GetRawSize());
}

template <class T>
LibPEAddressT(T)
PEElementT<T>::GetRawAddress()
{
    LIBPE_ASSERT_RET(NULL != m_pParser, 0);
    return m_pParser->IsRawAddressVA() ? GetRVA() : GetFOA();
}

template <class T>
LibPEAddressT(T)
PEElementT<T>::GetRawSize()
{
    LIBPE_ASSERT_RET(NULL != m_pParser, 0);
    return m_pParser->IsRawAddressVA() ? GetSizeInMemory() : GetSizeInFile();
}

template <class T>
LibPEAddressT(T)
PEElementT<T>::GetVA()
{
    if(0 == m_nVA) {
        LIBPE_ASSERT_RET(NULL != m_pParser, NULL);
        m_nVA = m_pParser->GetVAFromRVA(m_nRVA);
    }
    return m_nVA;
}

template <class T>
LibPEAddressT(T)
PEElementT<T>::GetFOA()
{
    if(0 == m_nFOA) {
        LIBPE_ASSERT_RET(NULL != m_pParser, NULL);
        m_nFOA = m_pParser->GetFOAFromRVA(m_nRVA);
    }
    return m_nFOA;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEElement);

LIBPE_NAMESPACE_END