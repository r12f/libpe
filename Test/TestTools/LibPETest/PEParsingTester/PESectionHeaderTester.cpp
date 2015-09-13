#include "stdafx.h"
#include "PESectionHeaderTester.h"

void PESectionHeaderListTester::DoTest()
{
    UINT32 sectionHeaderCount = GetPEFile()->GetSectionCount();
    EXPECT_EQ(GetExpectedResult()->UnsignedAttribute("section-header-count"), sectionHeaderCount);

    tinyxml2::XMLElement *sectionHeaderExpectedResult = nullptr;
    for (UINT32 sectionHeaderIndex = 0; sectionHeaderIndex < sectionHeaderCount; ++sectionHeaderIndex) {
        LibPEPtr<IPESectionHeader> sectionHeader;
        ASSERT_HRESULT_SUCCEEDED(GetPEFile()->GetSectionHeader(sectionHeaderIndex, &sectionHeader));
        ASSERT_NE(sectionHeader, nullptr);

        PESectionHeaderTester()
            .SetSectionHeader(sectionHeader)
            .SetPEFile(GetPEFile())
            .SetExpectedResult(GetExpectedResult(), sectionHeaderExpectedResult, &sectionHeaderExpectedResult)
            .Run();

        ASSERT_NE(sectionHeaderExpectedResult, nullptr);
    }

    ASSERT_TRUE(PESectionHeaderTester().IsLastExpectedResult(sectionHeaderExpectedResult));
}

void PESectionHeaderTester::DoTest()
{
    RunTestLoop([&, this]() {
        TestBasicInformation(_sectionHeader);
        TestSectionHeader(_sectionHeader);
    });
}

void PESectionHeaderTester::TestSectionHeader(_In_ IPESectionHeader *sectionHeader)
{
    BEGIN_TEST_PE_ELEMENT(sectionHeader)
        TEST_RAW_FIELD_SIMPLE(IPESectionHeader, PhysicalAddress)
        TEST_RAW_FIELD_SIMPLE(IPESectionHeader, VirtualSize)
        TEST_RAW_FIELD_SIMPLE(IPESectionHeader, VirtualAddress)
        TEST_RAW_FIELD_SIMPLE(IPESectionHeader, SizeOfRawData)
        TEST_RAW_FIELD_SIMPLE(IPESectionHeader, PointerToRawData)
        TEST_RAW_FIELD_SIMPLE(IPESectionHeader, PointerToRelocations)
        TEST_RAW_FIELD_SIMPLE(IPESectionHeader, PointerToLinenumbers)
        TEST_RAW_FIELD_SIMPLE(IPESectionHeader, NumberOfRelocations)
        TEST_RAW_FIELD_SIMPLE(IPESectionHeader, NumberOfLinenumbers)
        TEST_RAW_FIELD_SIMPLE(IPESectionHeader, Characteristics)
    END_TEST_PE_ELEMENT()
}
