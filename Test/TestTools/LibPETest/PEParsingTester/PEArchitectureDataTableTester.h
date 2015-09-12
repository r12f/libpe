#pragma once

#include "PEElementTester.h"

class PEArchitectureDataTableTester :
    public PEElementTester
{
public:
    PEArchitectureDataTableTester() : PEElementTester("ArchitectureDataTable") {}

protected:
    void DoTest() override;
};
