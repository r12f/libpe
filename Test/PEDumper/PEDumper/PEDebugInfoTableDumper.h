#pragma once

#include "PEElementDumper.h"

class PEDebugInfoTableDumper :
    public PEElementDumper
{
public:
    PEDebugInfoTableDumper() : PEElementDumper("DebugInfoTable") {}

protected:
    void DoDump() override;
};