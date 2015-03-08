#pragma once

#include "PEElementTester.h"

class PEDebugInfoTableTester :
    public PEElementTester
{
public:
    PEDebugInfoTableTester() : PEElementTester("DebugInfoTable") {}

protected:
    void DoTest() override;
    void TestDebugInfoTable(IPEDebugInfoTable *debugInfoTable);
};
