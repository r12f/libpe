#pragma once

#include "PEElementDumper.h"

class PECertificateTableDumper :
    public PEElementDumper
{
public:
    PECertificateTableDumper() : PEElementDumper("CertificateTable") {}

    PECertificateTableDumper & SetDumpElement(_In_ IPECertificateTable *certificateTable) {
        _certificateTable = certificateTable;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPECertificateTable> _certificateTable;
};

class PECertificateDumper :
    public PEElementDumper
{
public:
    PECertificateDumper() : PEElementDumper("Certificate") {}

    PECertificateDumper & SetDumpElement(LibPEPtr<IPECertificate> certificate) {
        _certificate = certificate;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPECertificate> _certificate;
};

