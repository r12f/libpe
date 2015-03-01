#pragma once

#include "PEElementDumper.h"

class PEGlobalPointerTableDumper :
    public PEElementDumper
{
public:
    PEGlobalPointerTableDumper() : PEElementDumper("GlobalPointerTable") {}

protected:
    void DoDump() override;
};