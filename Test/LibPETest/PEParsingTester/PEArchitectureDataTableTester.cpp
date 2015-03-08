#include "stdafx.h"
#include "PEArchitectureDataTableTester.h"

void PEArchitectureDataTableTester::DoTest()
{
    LibPEPtr<IPEArchitectureDataTable> architectureDataTable;
    GetPEFile()->GetArchitectureDataTable(&architectureDataTable);
    EXPECT_EQ(architectureDataTable, nullptr);
}
