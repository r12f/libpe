#include "stdafx.h"
#include "PE/PECertificateTable.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
UINT32
PECertificateTableT<T>::GetCertificateCount()
{
    LIBPE_CHK_HR_RET(EnsureCertificatesParsed(), 0);
    return (UINT32) m_vCertificates.size();
}

template <class T>
HRESULT
PECertificateTableT<T>::GetCertificateByIndex(_In_ UINT32 nIndex, _Outptr_ IPECertificate **ppCertificate)
{
    LIBPE_CHK(NULL != ppCertificate, E_POINTER);
    LIBPE_CHK_HR(EnsureCertificatesParsed());

    UINT32 nCertificateCount = GetCertificateCount();
    LIBPE_CHK(nIndex < nCertificateCount, E_INVALIDARG);

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

    LIBPE_STRICTCHK(NULL != m_pParser);
    LIBPE_CHK_HR(m_pParser->ParseCertificates(this));

    return S_OK;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PECertificateTableT);
LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PECertificateT);

LIBPE_NAMESPACE_END
