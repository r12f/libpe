#pragma once

#include "PEElementDumper.h"

class PEImportAddressTableDumper :
    public PEElementDumper
{
public:
    PEImportAddressTableDumper() : PEElementDumper("ImportAddressTable") {}

protected:
    void DoDump() override;
};

class PEImportAddressBlockDumper :
    public PEElementDumper
{
public:
    PEImportAddressBlockDumper() : PEElementDumper("ImportAddressBlock") {}

    PEImportAddressBlockDumper & SetImportAddressBlock(IPEImportAddressBlock *importAddressBlock) {
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

    PEImportAddressItemDumper & SetImportAddressItem(IPEImportAddressItem *importAddressItem) {
        _importAddressItem = importAddressItem; 
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEImportAddressItem> _importAddressItem;
};
