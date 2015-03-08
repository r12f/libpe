#include "stdafx.h"
#include "PESectionTester.h"

void PESectionListTester::DoTest()
{
    UINT32 sectionCount = GetPEFile()->GetSectionCount();
    EXPECT_EQ(GetExpectedResult()->UnsignedAttribute("section-count"), sectionCount);

    tinyxml2::XMLElement *sectionExpectedResult = nullptr;
    for (UINT32 sectionIndex = 0; sectionIndex < sectionCount; ++sectionIndex) {
        LibPEPtr<IPESection> section;
        ASSERT_HRESULT_SUCCEEDED(GetPEFile()->GetSection(sectionIndex, &section));
        ASSERT_NE(section, nullptr);

        PESectionTester()
            .SetSection(section)
            .SetPEFile(GetPEFile())
            .SetExpectedResult(GetExpectedResult(), sectionExpectedResult, &sectionExpectedResult)
            .Run();

        ASSERT_NE(sectionExpectedResult, nullptr);
    }

    ASSERT_TRUE(PESectionTester().IsLastExpectedResult(sectionExpectedResult));
}

void PESectionTester::DoTest()
{
    RunTestLoop([&, this]() {
        TestBasicInformation(_section);
        TestSection(_section);
    });
}

void PESectionTester::TestSection(IPESection *section)
{
    BEGIN_TEST_PE_ELEMENT(section)
        TEST_API_RESULT_STRING(IPESection, GetName)
        TEST_API_RESULT_SIMPLE(IPESection, GetCharacteristics)
    END_TEST_PE_ELEMENT()
}
