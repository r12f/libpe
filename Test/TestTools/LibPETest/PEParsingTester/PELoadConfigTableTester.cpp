#include "stdafx.h"
#include "PELoadConfigTableTester.h"

void PELoadConfigTableTester::DoTest()
{
    LibPEPtr<IPELoadConfigTable> loadConfigTable;
    GetPEFile()->GetLoadConfigTable(&loadConfigTable);
    ASSERT_EQ(loadConfigTable != nullptr, GetExpectedResult() != nullptr);

    if (loadConfigTable == nullptr) {
        return;
    }

    RunTestLoop([&, this]() {
        TestBasicInformation(loadConfigTable);
        TestLoadConfigTable(loadConfigTable);
    });
}

void PELoadConfigTableTester::TestLoadConfigTable(_In_ IPELoadConfigTable *loadConfigTable)
{
    BEGIN_TEST_PE_ELEMENT(loadConfigTable)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, Size)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, TimeDateStamp)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, MajorVersion)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, MinorVersion)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, GlobalFlagsClear)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, GlobalFlagsSet)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, CriticalSectionDefaultTimeout)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, DeCommitFreeBlockThreshold)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, DeCommitTotalFreeThreshold)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, LockPrefixTable)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, MaximumAllocationSize)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, VirtualMemoryThreshold)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, ProcessAffinityMask)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, ProcessHeapFlags)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, CSDVersion)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, Reserved1)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, EditList)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, SecurityCookie)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, SEHandlerTable)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, SEHandlerCount)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, GuardCFCheckFunctionPointer)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, Reserved2)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, GuardCFFunctionTable)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, GuardCFFunctionCount)
        TEST_RAW_FIELD_SIMPLE(IPELoadConfigTable, GuardFlags)
    END_TEST_PE_ELEMENT()
}
