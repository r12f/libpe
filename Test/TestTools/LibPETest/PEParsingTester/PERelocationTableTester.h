#pragma once

#include "PEElementTester.h"

class PERelocationTableTester :
    public PEElementTester
{
public:
    PERelocationTableTester() : PEElementTester("RelocationTable") {}

protected:
    void DoTest() override;
};

class PERelocationPageTester :
    public PEElementTester
{
public:
    PERelocationPageTester() : PEElementTester("RelocationPage") {}

    PERelocationPageTester & SetRelocationPage(_In_ IPERelocationPage *relocationPage) {
        _relocationPage = relocationPage;
        return *this;
    }

protected:
    void DoTest() override;
    void TestRelocationPage(_In_ IPERelocationPage *relocationPage);

private:
    LibPEPtr<IPERelocationPage> _relocationPage;
};

class PERelocationItemTester :
    public PEElementTester
{
public:
    PERelocationItemTester() : PEElementTester("RelocationItem") {}

    PERelocationItemTester & SetRelocationItem(_In_ IPERelocationItem *relocationItem) {
        _relocationItem = relocationItem;
        return *this;
    }

protected:
    void DoTest() override;
    void TestRelocationItem(_In_ IPERelocationItem *relocationItem);

private:
    LibPEPtr<IPERelocationItem> _relocationItem;
};
