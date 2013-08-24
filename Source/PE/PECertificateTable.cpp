#include "stdafx.h"
#include "PE/PECertificateTable.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
UINT32
PECertificateTableT<T>::GetCertificateCount()
{
    LIBPE_ASSERT_RET(SUCCEEDED(EnsureCertificatesParsed()), 0);
    return (UINT32) m_vCertificates.size();
}

template <class T>
HRESULT
PECertificateTableT<T>::GetCertificateByIndex(UINT32 nIndex, IPECertificate **ppCertificate)
{
    LIBPE_ASSERT_RET(NULL != ppCertificate, E_POINTER);
    LIBPE_ASSERT_RET(SUCCEEDED(EnsureCertificatesParsed()), E_FAIL);

    UINT32 nCertificateCount = GetCertificateCount();
    LIBPE_ASSERT_RET(nIndex < nCertificateCount, E_INVALIDARG);

    return m_vCertificates[nIndex].CopyTo(ppCertificate);
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PECertificateTableT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PECertificateT);

LIBPE_NAMESPACE_END
