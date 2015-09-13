#include "stdafx.h"
#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
void
PEElementT<T>::InnerSetBase(_In_ PEFileT<T> *pFile, _In_ PEParserT<T> *pParser)
{
    m_pFile = pFile;
    m_pParser = pParser;
}

template <class T>
void
PEElementT<T>::InnerSetRawMemory(_In_ void *pRawBuffer)
{
    m_pRawBuffer = pRawBuffer;
}

template <class T>
void
PEElementT<T>::InnerSetMemoryInfo(_In_ PEAddress nRVA, _In_ PEAddress nVA, _In_ PEAddress nSizeInMemory)
{
    m_nRVA = nRVA;
    m_nVA = nVA;
    m_nSizeInMemory = nSizeInMemory;
}

template <class T>
void
PEElementT<T>::InnerSetFileInfo(_In_ PEAddress nFOA, _In_ PEAddress nSizeInFile)
{
    m_nFOA = nFOA;
    m_nSizeInFile = nSizeInFile;
}

template <class T>
void *
PEElementT<T>::GetRawMemory()
{
    if(NULL == m_pRawBuffer) {
        LIBPE_CHK(NULL != m_pParser, NULL);
        m_pRawBuffer = m_pParser->GetRawMemory(GetRawOffset(), GetRawSize());
    }

    return m_pRawBuffer;
}

template <class T>
PEAddress
PEElementT<T>::GetRawOffset()
{
    LIBPE_CHK(NULL != m_pParser, LIBPE_INVALID_ADDRESS);
    return m_pParser->IsRawAddressVA() ? GetRVA() : GetFOA();
}

template <class T>
PEAddress
PEElementT<T>::GetRawSize()
{
    LIBPE_CHK(NULL != m_pParser, LIBPE_INVALID_SIZE);
    return m_pParser->IsRawAddressVA() ? GetSizeInMemory() : GetSizeInFile();
}

template <class T>
PEAddress
PEElementT<T>::GetRVA()
{
    if(LIBPE_INVALID_ADDRESS == m_nRVA) {
        LIBPE_CHK(NULL != m_pParser, LIBPE_INVALID_ADDRESS);
        m_nRVA = m_pParser->GetRVAFromFOA(m_nFOA);
    }
    return m_nRVA;
}

template <class T>
PEAddress
PEElementT<T>::GetVA()
{
    if(LIBPE_INVALID_ADDRESS == m_nVA) {
        LIBPE_CHK(NULL != m_pParser, LIBPE_INVALID_ADDRESS);
        m_nVA = m_pParser->GetVAFromRVA(m_nRVA);
    }
    return m_nVA;
}

template <class T>
PEAddress
PEElementT<T>::GetFOA()
{
    if(LIBPE_INVALID_ADDRESS == m_nFOA) {
        LIBPE_CHK(NULL != m_pParser, LIBPE_INVALID_ADDRESS);
        m_nFOA = m_pParser->GetFOAFromRVA(m_nRVA);
    }
    return m_nFOA;
}

template <class T>
HRESULT 
PEElementT<T>::EnsureDataReady()
{
    if (!m_bIsDelayedDataLoaded) {
        m_bIsDelayedDataLoaded = true;
        m_nLoadDelayedDataResult = LoadDelayedData();
    }

    return m_nLoadDelayedDataResult;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEElementT);

LIBPE_NAMESPACE_END