#pragma once

#include "PEElementTester.h"
#include "PEElementListItemTester.h"

class PESectionHeaderTester :
    public PEElementListItemTester
{
public:
    PESectionHeaderTester() : PEElementListItemTester("SectionHeader") {}

protected:
    void DoTest() override
    {
        LibPEPtr<IPESectionHeader> sectionHeader;
        ASSERT_HRESULT_SUCCEEDED(GetPEFile()->GetSectionHeader(GetItemIndex(), &sectionHeader));
        ASSERT_NE(sectionHeader, nullptr);

        RunTestLoop([&, this]() {
            TestBasicInformation(sectionHeader);
            TestSectionHeader(sectionHeader);
        });
    }

    void TestSectionHeader(IPESectionHeader *sectionHeader)
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
};

class PESectionHeaderListTester :
    public PEElementTester
{
public:
    PESectionHeaderListTester() : PEElementTester("SectionHeaders") {}

protected:
    void DoTest() override
    {
        UINT32 sectionCount = GetPEFile()->GetSectionCount();
        EXPECT_EQ(GetExpectedElement()->UnsignedAttribute("count"), sectionCount);

        for (UINT32 sectionIndex = 0; sectionIndex < sectionCount; ++sectionIndex) {
            _sectionHeaderTester.Run(GetPEFile(), GetExpectedElement(), sectionIndex);
        }
    }

private:
    PESectionHeaderTester _sectionHeaderTester;
};