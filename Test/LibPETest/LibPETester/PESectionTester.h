#pragma once

#include "PEElementTester.h"
#include "PEElementListItemTester.h"

class PESectionTester :
    public PEElementListItemTester
{
public:
    PESectionTester() : PEElementListItemTester("Section") {}

protected:
    void DoTest() override
    {
        LibPEPtr<IPESection> section;
        ASSERT_HRESULT_SUCCEEDED(GetPEFile()->GetSection(GetItemIndex(), &section));
        ASSERT_NE(section, nullptr);

        RunTestLoop([&, this]() {
            TestBasicInformation(section);
            TestSection(section);
        });
    }

    void TestSection(IPESection *section)
    {
        BEGIN_TEST_PE_ELEMENT(section)
            TEST_API_RESULT_SIMPLE(IPESection, GetName)
            TEST_API_RESULT_SIMPLE(IPESection, GetCharacteristics)
        END_TEST_PE_ELEMENT()
    }
};

class PESectionListTester :
    public PEElementTester
{
public:
    PESectionListTester() : PEElementTester("Sections") {}

protected:
    void DoTest() override
    {
        UINT32 sectionCount = GetPEFile()->GetSectionCount();
        EXPECT_EQ(GetExpectedElement()->UnsignedAttribute("count"), sectionCount);

        for (UINT32 sectionIndex = 0; sectionIndex < sectionCount; ++sectionIndex) {
            _sectionTester.Run(GetPEFile(), GetExpectedElement(), sectionIndex);
        }
    }

private:
    PESectionTester _sectionTester;
};
