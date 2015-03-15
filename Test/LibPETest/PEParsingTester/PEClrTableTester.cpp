#include "stdafx.h"
#include "PEClrTableTester.h"

void PEClrTableTester::DoTest()
{
    LibPEPtr<IPEClrTable> clrTable;
    GetPEFile()->GetClrTable(&clrTable);
    ASSERT_EQ(clrTable != nullptr, GetExpectedResult() != nullptr);

    if (clrTable == nullptr) {
        return;
    }

    RunTestLoop([&, this]() {
        TestBasicInformation(clrTable);
    });
}
