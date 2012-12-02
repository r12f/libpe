#include "stdafx.h"
#include "Parser/PEParserImpl.h"
#include "PE/PEFile.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
PEParserT<T> *
PEParserT<T>::Create(PEParserType nType)
{
    switch(nType) {
    case PE_PARSER_TYPE_DISK_FILE:
        return new PEParserDiskFileT<T>;
    }

    return NULL;
}

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
PEParserDiskFileT<T>::GetAddressFromRVA(PEAddressT<T> nRVA)
{
    return nRVA;
}

template <class T>
PEAddressT<T>
PEParserDiskFileT<T>::GetAddressFromVA(PEAddressT<T> nVA)
{
    return nVA;
}

template <class T>
PEAddressT<T>
PEParserDiskFileT<T>::GetAddressFromFOA(PEAddressT<T> nFOA)
{
    return nFOA;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParser32, Create);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS_FUNCTION(PEParser64, Create);

LIBPE_NAMESPACE_END