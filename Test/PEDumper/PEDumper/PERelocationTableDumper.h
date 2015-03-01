#pragma once

#include "PEElementDumper.h"

class PERelocationTableDumper :
    public PEElementDumper
{
public:
    PERelocationTableDumper() : PEElementDumper("RelocationTable") {}

protected:
    void DoDump() override;
};

class PERelocationPageDumper :
    public PEElementDumper
{
public:
    PERelocationPageDumper() : PEElementDumper("RelocationPage") {}

    PERelocationPageDumper & SetRelocationPage(IPERelocationPage *relocationPage) {
        _relocationPage = relocationPage;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPERelocationPage> _relocationPage;
};

class PERelocationItemDumper :
    public PEElementDumper
{
public:
    PERelocationItemDumper() : PEElementDumper("RelocationItem") {}

    PERelocationItemDumper & SetRelocationItem(IPERelocationItem *relocationItem) {
        _relocationItem = relocationItem; 
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPERelocationItem> _relocationItem;
};
