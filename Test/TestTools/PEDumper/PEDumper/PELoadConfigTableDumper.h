#pragma once

#include "PEElementDumper.h"

class PELoadConfigTableDumper :
    public PEElementDumper
{
public:
    PELoadConfigTableDumper() : PEElementDumper("LoadConfigTable") {}

    PELoadConfigTableDumper & SetDumpElement(IPELoadConfigTable *loadConfigTable) {
        _loadConfigTable = loadConfigTable;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPELoadConfigTable> _loadConfigTable;
};