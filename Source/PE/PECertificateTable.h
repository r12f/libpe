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
    ~PECertificateTableT() override {}

    DECLARE_PE_ELEMENT(LibPERawWinCertificate(T))

    HRESULT InnerAddCertificate(_In_ IPECertificate *pCertificate);

    UINT32 LIBPE_CALLTYPE GetCertificateCount() override;
    HRESULT LIBPE_CALLTYPE GetCertificateByIndex(_In_ UINT32 nIndex, _Outptr_ IPECertificate **ppCertificate) override;

protected:
    HRESULT EnsureCertificatesParsed();

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
    ~PECertificateT() override {}

    DECLARE_PE_ELEMENT(LibPERawWinCertificate(T))

    LIBPE_FIELD_ACCESSOR_EX(UINT32, Length, dwLength);
    LIBPE_FIELD_ACCESSOR_EX(UINT16, Revision, wRevision);
    LIBPE_FIELD_ACCESSOR_EX(UINT16, CertificateType, wCertificateType);
    LIBPE_FIELD_ACCESSOR_EX(BYTE *, Certificate, bCertificate);
};

LIBPE_NAMESPACE_END