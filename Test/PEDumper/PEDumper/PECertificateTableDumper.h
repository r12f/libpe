#pragma once

#include "PEElementDumper.h"

class PECertificateTableDumper :
    public PEElementDumper
{
public:
    PECertificateTableDumper() : PEElementDumper("CertificateTable") {}

protected:
    void DoDump() override;
};

class PECertificateDumper :
    public PEElementDumper
{
public:
    PECertificateDumper() : PEElementDumper("Certificate") {}

    PECertificateDumper & SetCertificate(LibPEPtr<IPECertificate> certificate) {
        _certificate = certificate;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPECertificate> _certificate;
};

