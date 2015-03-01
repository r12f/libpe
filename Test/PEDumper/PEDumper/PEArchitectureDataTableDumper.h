#pragma once

#include "PEElementDumper.h"

class PEArchitectureDataTableDumper :
    public PEElementDumper
{
public:
    PEArchitectureDataTableDumper() : PEElementDumper("ArchitectureDataTable") {}

protected:
    void DoDump() override;
};