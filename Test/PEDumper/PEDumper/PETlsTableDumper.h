#pragma once

#include "PEElementDumper.h"

class PETlsTableDumper :
    public PEElementDumper
{
public:
    PETlsTableDumper() : PEElementDumper("TlsTable") {}

protected:
    void DoDump() override;
};
