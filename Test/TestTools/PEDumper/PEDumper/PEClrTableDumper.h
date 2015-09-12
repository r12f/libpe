#pragma once

#include "PEElementDumper.h"

class PEClrTableDumper :
    public PEElementDumper
{
public:
    PEClrTableDumper() : PEElementDumper("ClrTable") {}

    PEClrTableDumper & SetDumpElement(IPEClrTable *ClrTable) {
        _ClrTable = ClrTable;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEClrTable> _ClrTable;
};