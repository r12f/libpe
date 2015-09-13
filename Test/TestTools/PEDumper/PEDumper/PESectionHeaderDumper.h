#pragma once

#include "PEElementDumper.h"

class PESectionHeaderListDumper :
    public PEElementDumper
{
public:
    PESectionHeaderListDumper() : PEElementDumper("SectionHeaders") {}

protected:
    void DoDump() override;
};

class PESectionHeaderDumper :
    public PEElementDumper
{
public:
    PESectionHeaderDumper() : PEElementDumper("SectionHeader") {}

    PESectionHeaderDumper & SetSectionHeader(_In_ IPESectionHeader *sectionHeader) {
        _sectionHeader = sectionHeader;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPESectionHeader> _sectionHeader;
};

