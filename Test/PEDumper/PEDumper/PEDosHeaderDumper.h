#pragma once

#include "PEElementDumper.h"

class PEDosHeaderDumper :
    public PEElementDumper
{
public:
    PEDosHeaderDumper() : PEElementDumper("DosHeader") {}

protected:
    void DoDump() override;
};