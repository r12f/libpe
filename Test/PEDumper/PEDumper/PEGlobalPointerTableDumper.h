#pragma once

#include "PEElementDumper.h"

class PEGlobalPointerTableDumper :
    public PEElementDumper
{
public:
    PEGlobalPointerTableDumper() : PEElementDumper("GlobalPointerTable") {}

    PEGlobalPointerTableDumper & SetDumpElement(IPEGlobalPointerTable *globalPointerTable) {
        _globalPointerTable = globalPointerTable;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEGlobalPointerTable> _globalPointerTable;
};