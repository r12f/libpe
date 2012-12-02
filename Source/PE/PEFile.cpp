#include "stdafx.h"
#include "PEFile.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
PEFileT<T>::PEFileT(DataLoader *pRawFile)
    : PEBase(pRawFile)
    , m_pRawFile(pRawFile)
    , m_pDosHeader(NULL)
    , m_pFileHeader(NULL)
    , m_pNtHeader(NULL)
    , m_pOptionalHeader(NULL)
{

}

template <class T>
PEFileT<T>::~PEFileT()
{

}

LIBPE_NAMESPACE_END