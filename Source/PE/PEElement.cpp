#include "stdafx.h"
#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
void *
PEElementT<T>::GetRawMemory()
{
    LIBPE_ASSERT_RET(NULL != m_pFile, NULL);
    return m_pFile->GetRawMemory(GetRVA(), GetSizeInMemory());
}

template <class T>
LibPEAddressT(T)
PEElementT<T>::GetRVA()
{
    return m_nRVA;
}

template <class T>
LibPEAddressT(T)
PEElementT<T>::GetVA()
{
    LIBPE_ASSERT_RET(NULL != m_pFile, NULL);
    return m_pFile->GetVAFromRVA(m_nRVA);
}

template <class T>
LibPEAddressT(T)
PEElementT<T>::GetSizeInMemory()
{
    return m_nSizeInMemory;
}

template <class T>
LibPEAddressT(T)
PEElementT<T>::GetFOA()
{
    if(0 == m_nFOA) {
        LIBPE_ASSERT_RET(NULL != m_pFile, NULL);
        m_nFOA = m_pFile->GetFOAFromRVA(m_nRVA);
    }
    return m_nFOA;
}

template <class T>
LibPEAddressT(T)
PEElementT<T>::GetSizeInFile()
{
    return m_nSizeInFile;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEElement);

LIBPE_NAMESPACE_END