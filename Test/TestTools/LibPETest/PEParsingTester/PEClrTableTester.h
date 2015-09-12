#pragma once

#include "PEElementTester.h"

class PEClrTableTester :
    public PEElementTester
{
public:
    PEClrTableTester() : PEElementTester("ClrTable") {}

protected:
    void DoTest() override;
};
