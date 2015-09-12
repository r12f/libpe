#pragma once

#include "PEElementTester.h"

class PESectionHeaderListTester :
    public PEElementTester
{
public:
    PESectionHeaderListTester() : PEElementTester("SectionHeaders") {}

protected:
    void DoTest() override;
};

class PESectionHeaderTester :
    public PEElementTester
{
public:
    PESectionHeaderTester() : PEElementTester("SectionHeader") {}

    PESectionHeaderTester & SetSectionHeader(IPESectionHeader *sectionHeader) {
        _sectionHeader = sectionHeader;
        return *this;
    }

protected:
    void DoTest() override;
    void TestSectionHeader(IPESectionHeader *sectionHeader);

private:
    LibPEPtr<IPESectionHeader> _sectionHeader;
};
