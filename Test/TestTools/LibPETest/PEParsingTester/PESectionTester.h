#pragma once

#include "PEElementTester.h"

class PESectionListTester :
    public PEElementTester
{
public:
    PESectionListTester() : PEElementTester("Sections") {}

protected:
    void DoTest() override;
};

class PESectionTester :
    public PEElementTester
{
public:
    PESectionTester() : PEElementTester("Section") {}

    PESectionTester & SetSection(_In_ IPESection *section) {
        _section = section;
        return *this;
    }

protected:
    void DoTest() override;
    void TestSection(_In_ IPESection *section);

private:
    LibPEPtr<IPESection> _section;
};
