#include "stdafx.h"
#include "Parser/PEParserImpl.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
error_t
PEParserDiskFileT<T>::ParsePEBasicInfo()
{
    LIBPE_ASSERT_RET(NULL != m_pFile, ERR_FAIL);

    error_t nErrCode = ERR_OK;

    nErrCode = SetPEDosHeader((PEDosHeaderT<T> *)m_pLoader->GetBuffer(0, sizeof(PEDosHeaderT<T>)));
    LIBPE_ASSERT_RET(ERR_OK == nErrCode, nErrCode);

    nErrCode = SetPENtHeaders((PENtHeadersT<T> *)m_pLoader->GetBuffer(0, sizeof(PENtHeadersT<T>)));
    LIBPE_ASSERT_RET(ERR_OK == nErrCode, nErrCode);

    nErrCode = SetPEFileHeader((PEFileHeaderT<T> *)m_pLoader->GetBuffer(sizeof(PEDosHeaderT<T>) + sizeof(uint32_t), sizeof(PEFileHeaderT<T>)));
    LIBPE_ASSERT_RET(ERR_OK == nErrCode, nErrCode);

    nErrCode = SetPEOptionalHeader((PEOptionalHeaderT<T> *)m_pLoader->GetBuffer(0, sizeof(PEOptionalHeaderT<T>)));
    LIBPE_ASSERT_RET(ERR_OK == nErrCode, nErrCode);

    return ERR_OK;
}

template <class T>
PEAddressT<T>
PEParserDiskFileT<T>::GetRVAFromRawAddress(PEAddressT<T> nRawAddress)
{
    return nRawAddress;
}

template <class T>
PEAddressT<T>
PEParserDiskFileT<T>::GetVAFromRawAddress(PEAddressT<T> nRawAddress)
{
    return nRawAddress;
}

template <class T>
PEAddressT<T>
PEParserDiskFileT<T>::GetFOAFromRawAddress(PEAddressT<T> nRawAddress)
{
    return nRawAddress;
}

LIBPE_NAMESPACE_END