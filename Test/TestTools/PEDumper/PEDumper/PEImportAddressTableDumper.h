#pragma once

#include "PEElementDumper.h"

class PEImportAddressTableDumper :
    public PEElementDumper
{
public:
    PEImportAddressTableDumper() : PEElementDumper("ImportAddressTable") {}

    PEImportAddressTableDumper & SetDumpElement(IPEImportAddressTable *importAddressTable) {
        _importAddressTable = importAddressTable;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEImportAddressTable> _importAddressTable;
};

class PEImportAddressBlockDumper :
    public PEElementDumper
{
public:
    PEImportAddressBlockDumper() : PEElementDumper("ImportAddressBlock") {}

    PEImportAddressBlockDumper & SetDumpElement(IPEImportAddressBlock *importAddressBlock) {
        _importAddressBlock = importAddressBlock;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEImportAddressBlock> _importAddressBlock;
};

class PEImportAddressItemDumper :
    public PEElementDumper
{
public:
    PEImportAddressItemDumper() : PEElementDumper("ImportAddressItem") {}

    PEImportAddressItemDumper & SetDumpElement(IPEImportAddressItem *importAddressItem) {
        _importAddressItem = importAddressItem; 
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEImportAddressItem> _importAddressItem;
};
