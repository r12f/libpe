#include "stdafx.h"
#include "PEDebugInfoTableTester.h"

void PEDebugInfoTableTester::DoTest()
{
    LibPEPtr<IPEDebugInfoTable> debugInfoTable;
    GetPEFile()->GetDebugInfoTable(&debugInfoTable);
    ASSERT_EQ(debugInfoTable != nullptr, GetExpectedResult() != nullptr);

    if (debugInfoTable == nullptr) {
        return;
    }

    RunTestLoop([&, this]() {
        TestBasicInformation(debugInfoTable);
        TestDebugInfoTable(debugInfoTable);
    });
}

void PEDebugInfoTableTester::TestDebugInfoTable(_In_ IPEDebugInfoTable *debugInfoTable)
{
    BEGIN_TEST_PE_ELEMENT(debugInfoTable)
        TEST_RAW_FIELD_SIMPLE(IPEDebugInfoTable, Characteristics)
        TEST_RAW_FIELD_SIMPLE(IPEDebugInfoTable, TimeDateStamp)
        TEST_RAW_FIELD_SIMPLE(IPEDebugInfoTable, MajorVersion)
        TEST_RAW_FIELD_SIMPLE(IPEDebugInfoTable, MinorVersion)
        TEST_RAW_FIELD_SIMPLE(IPEDebugInfoTable, Type)
        TEST_RAW_FIELD_SIMPLE(IPEDebugInfoTable, SizeOfData)
        TEST_RAW_FIELD_SIMPLE(IPEDebugInfoTable, AddressOfRawData)
        TEST_RAW_FIELD_SIMPLE(IPEDebugInfoTable, PointerToRawData)
    END_TEST_PE_ELEMENT()
}
