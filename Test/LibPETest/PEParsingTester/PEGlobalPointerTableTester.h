#pragma once

#include "PEElementTester.h"

class PEGlobalPointerTableTester :
    public PEElementTester
{
public:
    PEGlobalPointerTableTester() : PEElementTester("GlobalPointerTable") {}

protected:
    void DoTest() override;
    void TestGlobalPointerTable(IPEGlobalPointerTable *globalPointerTable);
};
