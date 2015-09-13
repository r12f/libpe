#pragma once

#include "PEElementDumper.h"

class PESectionListDumper :
    public PEElementDumper
{
public:
    PESectionListDumper() : PEElementDumper("Sections") {}

protected:
    void DoDump() override;
};

class PESectionDumper :
    public PEElementDumper
{
public:
    PESectionDumper() : PEElementDumper("Section") {}

    PESectionDumper & SetSection(_In_ IPESection *section) {
        _section = section;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPESection> _section;
};
