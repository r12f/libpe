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

    PERelocationPageTester & SetRelocationPage(IPERelocationPage *relocationPage) {
        _relocationPage = relocationPage;
        return *this;
    }

protected:
    void DoTest() override;
    void TestRelocationPage(IPERelocationPage *relocationPage);

private:
    LibPEPtr<IPERelocationPage> _relocationPage;
};

class PERelocationItemTester :
    public PEElementTester
{
public:
    PERelocationItemTester() : PEElementTester("RelocationItem") {}

    PERelocationItemTester & SetRelocationItem(IPERelocationItem *relocationItem) {
        _relocationItem = relocationItem;
        return *this;
    }

protected:
    void DoTest() override;
    void TestRelocationItem(IPERelocationItem *relocationItem);

private:
    LibPEPtr<IPERelocationItem> _relocationItem;
};
