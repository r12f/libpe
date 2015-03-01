#pragma once

#include "PEElementDumper.h"

class PELoadConfigTableDumper :
    public PEElementDumper
{
public:
    PELoadConfigTableDumper() : PEElementDumper("LoadConfigTable") {}

protected:
    void DoDump() override;
};