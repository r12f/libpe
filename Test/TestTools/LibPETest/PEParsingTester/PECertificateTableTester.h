#pragma once

#include "PEElementTester.h"

class PECertificateTableTester :
    public PEElementTester
{
public:
    PECertificateTableTester() : PEElementTester("CertificateTable") {}

protected:
    void DoTest() override;
};

class PECertificateTester :
    public PEElementTester
{
public:
    PECertificateTester() : PEElementTester("Certificate") {}

    PECertificateTester & SetCertificate(IPECertificate *certificate) {
        _certificate = certificate;
        return *this;
    }

protected:
    void DoTest() override;
    void TestCertificate(IPECertificate *certificate);

private:
    LibPEPtr<IPECertificate> _certificate;
};
