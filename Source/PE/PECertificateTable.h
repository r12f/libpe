#pragma once

#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class PECertificateTableT :
    public IPECertificateTable,
    public PEElementT<T>
{
    typedef std::vector<LibPEPtr<IPECertificate>> CertificateList;

public:
    PECertificateTableT() : m_bIsCertificatesParsed(false) {}
    virtual ~PECertificateTableT() {}

    DECLARE_PE_ELEMENT(LibPERawWinCertificate(T))

    HRESULT InnerAddCertificate(IPECertificate *pCertificate) {
        LIBPE_ASSERT_RET(NULL != pCertificate, E_INVALIDARG);

        HRESULT hr = S_OK;

        LIBPE_HR_TRY_BEGIN(hr) {
            m_vCertificates.push_back(pCertificate);
        } LIBPE_HR_TRY_END()

        return hr;
    }

    virtual UINT32 LIBPE_CALLTYPE GetCertificateCount();
    virtual HRESULT LIBPE_CALLTYPE GetCertificateByIndex(UINT32 nIndex, IPECertificate **ppCertificate);

protected:
    HRESULT EnsureCertificatesParsed() {
        if (m_bIsCertificatesParsed) {
            return S_OK;
        }

        m_bIsCertificatesParsed = true;

        LIBPE_ASSERT_RET(NULL != m_pParser, E_UNEXPECTED);
        LIBPE_ASSERT_RET(SUCCEEDED(m_pParser->ParseCertificates(this)), E_FAIL);

        return S_OK;
    }

private:
    bool m_bIsCertificatesParsed;
    CertificateList m_vCertificates;
};

template <class T>
class PECertificateT :
    public IPECertificate,
    public PEElementT<T>
{
public:
    PECertificateT() {}
    virtual ~PECertificateT() {}

    DECLARE_PE_ELEMENT(LibPERawWinCertificate(T))

    LIBPE_FIELD_ACCESSOR_EX(UINT32, Length, dwLength);
    LIBPE_FIELD_ACCESSOR_EX(UINT16, Revision, wRevision);
    LIBPE_FIELD_ACCESSOR_EX(UINT16, CertificateType, wCertificateType);
    LIBPE_FIELD_ACCESSOR_EX(BYTE *, Certificate, bCertificate);
};

LIBPE_NAMESPACE_END