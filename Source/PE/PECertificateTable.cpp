#include "stdafx.h"
#include "PE/PECertificateTable.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
HRESULT
PECertificateTableT<T>::InnerAddCertificate(IPECertificate *pCertificate)
{
    LIBPE_ASSERT_RET(NULL != pCertificate, E_INVALIDARG);

    HRESULT hr = S_OK;

    LIBPE_HR_TRY_BEGIN(hr) {
        m_vCertificates.push_back(pCertificate);
    } LIBPE_HR_TRY_END();

    return hr;
}

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

template <class T>
HRESULT
PECertificateTableT<T>::EnsureCertificatesParsed()
{
    if (m_bIsCertificatesParsed) {
        return S_OK;
    }

    m_bIsCertificatesParsed = true;

    LIBPE_ASSERT_RET(NULL != m_pParser, E_UNEXPECTED);
    LIBPE_ASSERT_RET(SUCCEEDED(m_pParser->ParseCertificates(this)), E_FAIL);

    return S_OK;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PECertificateTableT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PECertificateT);

LIBPE_NAMESPACE_END
