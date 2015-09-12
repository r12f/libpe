#include "stdafx.h"
#include "PEGlobalPointerTableTester.h"

void PEGlobalPointerTableTester::DoTest()
{
    LibPEPtr<IPEGlobalPointerTable> globalPointerTable;
    GetPEFile()->GetGlobalPointerTable(&globalPointerTable);
    ASSERT_EQ(globalPointerTable != nullptr, GetExpectedResult() != nullptr);

    if (globalPointerTable == nullptr) {
        return;
    }

    RunTestLoop([&, this]() {
        TestBasicInformation(globalPointerTable);
        TestGlobalPointerTable(globalPointerTable);
    });
}

void PEGlobalPointerTableTester::TestGlobalPointerTable(IPEGlobalPointerTable *globalPointerTable)
{
    BEGIN_TEST_PE_ELEMENT(globalPointerTable)
        TEST_RAW_FIELD_SIMPLE(IPEGlobalPointerTable, GlobalPointerRVA)
    END_TEST_PE_ELEMENT()
}
