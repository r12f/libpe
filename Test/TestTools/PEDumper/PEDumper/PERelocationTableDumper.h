#pragma once

#include "PEElementDumper.h"

class PERelocationTableDumper :
    public PEElementDumper
{
public:
    PERelocationTableDumper() : PEElementDumper("RelocationTable") {}

    PERelocationTableDumper & SetDumpElement(_In_ IPERelocationTable *relocationTable) {
        _relocationTable = relocationTable;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPERelocationTable> _relocationTable;
};

class PERelocationPageDumper :
    public PEElementDumper
{
public:
    PERelocationPageDumper() : PEElementDumper("RelocationPage") {}

    PERelocationPageDumper & SetDumpElement(_In_ IPERelocationPage *relocationPage) {
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

    PERelocationItemDumper & SetDumpElement(_In_ IPERelocationItem *relocationItem) {
        _relocationItem = relocationItem; 
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPERelocationItem> _relocationItem;
};
