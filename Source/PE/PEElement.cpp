#include "stdafx.h"
#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
PEElementT<T>::PEElementT(PEFileT<T> *pFile)
    : m_pFile(pFile)
    , m_nRVA(0)
    , m_nSizeInMemory(0)
    , m_nSizeInFile(0)
{
    LIBPE_ASSERT(NULL != m_pFile);
}

template <class T>
PEElementT<T>::~PEElementT()
{

}

template <class T>
void
PEElementT<T>::SetBasicElementInfo(PEAddressT<T> nRVA, PEAddressT<T> nSizeInMemory, PEAddressT<T> nSizeInFile)
{
    m_nRVA = nRVA;
    m_nSizeInMemory = nSizeInMemory;
    m_nSizeInFile = nSizeInFile;
}

template <class T>
void *
PEElementT<T>::GetRawMemory()
{
    LIBPE_ASSERT_RET(NULL != m_pFile, NULL);
    return m_pFile->GetRawMemory(GetRVA(), GetSizeInMemory());
}

template <class T>
PEAddressT<T>
PEElementT<T>::GetRVA()
{
    return m_nRVA;
}

template <class T>
PEAddressT<T>
PEElementT<T>::GetVA()
{
    LIBPE_ASSERT_RET(NULL != m_pFile, NULL);
    return m_pFile->GetVAFromRVA(m_nRVA);
}

template <class T>
PEAddressT<T>
PEElementT<T>::GetSizeInMemory()
{
    return m_nSizeInMemory;
}

template <class T>
PEAddressT<T>
PEElementT<T>::GetFOA()
{
    LIBPE_ASSERT_RET(NULL != m_pFile, NULL);
    return m_pFile->GetFOAFromRVA(m_nRVA);
}

template <class T>
PEAddressT<T>
PEElementT<T>::GetSizeInFile()
{
    return m_nSizeInFile;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEElement);

LIBPE_NAMESPACE_END