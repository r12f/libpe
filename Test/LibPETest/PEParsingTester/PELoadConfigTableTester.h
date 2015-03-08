#pragma once

#include "PEElementTester.h"

class PELoadConfigTableTester :
    public PEElementTester
{
public:
    PELoadConfigTableTester() : PEElementTester("LoadConfigTable") {}

protected:
    void DoTest() override;
    void TestLoadConfigTable(IPELoadConfigTable *loadConfigTable);
};
