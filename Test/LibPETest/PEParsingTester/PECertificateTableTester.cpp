#include "stdafx.h"
#include "PECertificateTableTester.h"

void PECertificateTableTester::DoTest()
{
    LibPEPtr<IPECertificateTable> certificateTable;
    GetPEFile()->GetCertificateTable(&certificateTable);
    ASSERT_EQ(certificateTable != nullptr, GetExpectedResult() != nullptr);

    if (certificateTable == nullptr) {
        return;
    }

    RunTestLoop([&, this]() {
        TestBasicInformation(certificateTable);
    });

    UINT32 certificateCount = certificateTable->GetCertificateCount();
    EXPECT_EQ(GetExpectedResult()->UnsignedAttribute("certificate-count"), certificateCount);

    tinyxml2::XMLElement *certificateExpectedResult = nullptr;
    for (UINT32 certificateIndex = 0; certificateIndex < certificateCount; ++certificateIndex) {
        LibPEPtr<IPECertificate> certificate;
        ASSERT_HRESULT_SUCCEEDED(certificateTable->GetCertificateByIndex(certificateIndex, &certificate));
        ASSERT_NE(certificate, nullptr);

        PECertificateTester()
            .SetCertificate(certificate)
            .SetPEFile(GetPEFile())
            .SetExpectedResult(GetExpectedResult(), certificateExpectedResult, &certificateExpectedResult)
            .Run();

        ASSERT_NE(certificateExpectedResult, nullptr);
    }

    ASSERT_TRUE(PECertificateTester().IsLastExpectedResult(certificateExpectedResult));
}

void PECertificateTester::DoTest()
{
    RunTestLoop([&, this]() {
        TestBasicInformation(_certificate);
        TestCertificate(_certificate);
    });
}

void PECertificateTester::TestCertificate(IPECertificate *certificate)
{
    BEGIN_TEST_PE_ELEMENT(certificate)
        TEST_RAW_FIELD_SIMPLE(IPECertificate, Length)
        TEST_RAW_FIELD_SIMPLE(IPECertificate, Revision)
        TEST_RAW_FIELD_SIMPLE(IPECertificate, CertificateType)
        // TEST_RAW_FIELD_SIMPLE(IPECertificate, Certificate);
    END_TEST_PE_ELEMENT()
}
