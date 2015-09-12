#pragma once

#include "PEElementDumper.h"

class PEArchitectureDataTableDumper :
    public PEElementDumper
{
public:
    PEArchitectureDataTableDumper() : PEElementDumper("ArchitectureDataTable") {}

    PEArchitectureDataTableDumper & SetDumpElement(IPEArchitectureDataTable *architectureDataTable) {
        _architectureDataTable = architectureDataTable;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEArchitectureDataTable> _architectureDataTable;
};