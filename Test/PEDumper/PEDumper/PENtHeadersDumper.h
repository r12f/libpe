#pragma once

#include "PEElementDumper.h"

class PENtHeadersDumper :
    public PEElementDumper
{
public:
    PENtHeadersDumper() : PEElementDumper("NtHeaders") {}

protected:
    void DoDump() override;
};

class PEFileHeaderDumper :
    public PEElementDumper
{
public:
    PEFileHeaderDumper() : PEElementDumper("FileHeader") {}

protected:
    void DoDump() override;
};

class PEOptionalHeaderDumper :
    public PEElementDumper
{
public:
    PEOptionalHeaderDumper() : PEElementDumper("OptionalHeader") {}

protected:
    void DoDump() override;
};

