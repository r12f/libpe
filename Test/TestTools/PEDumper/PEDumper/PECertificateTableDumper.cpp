#include "stdafx.h"
#include "PECertificateTableDumper.h"

void PECertificateTableDumper::DoDump()
{
    DumpBasicInformation(_certificateTable);

    UINT32 certificateCount = _certificateTable->GetCertificateCount();
    GetOutputElement()->SetAttribute("certificate-count", certificateCount);

    for (UINT32 certificateIndex = 0; certificateIndex < certificateCount; ++certificateIndex) {
        LibPEPtr<IPECertificate> certificate;
        if (FAILED(_certificateTable->GetCertificateByIndex(certificateIndex, &certificate))) {
            continue;
        }

        PECertificateDumper()
            .SetDumpElement(certificate)
            .SetPEFile(GetPEFile())
            .SetParentElement(GetOutputElement())
            .Run();
    }
}

void PECertificateDumper::DoDump()
{
    DumpBasicInformation(_certificate);

    BEGIN_DUMP_PE_ELEMENT(_certificate)
        DUMP_RAW_FIELD_SIMPLE(Length)
        DUMP_RAW_FIELD_SIMPLE(Revision)
        DUMP_RAW_FIELD_SIMPLE(CertificateType)

        // TODO: Adding buffer dumper
        // DUMP_RAW_FIELD_SIMPLE(Certificate)
    END_DUMP_PE_ELEMENT()
}
