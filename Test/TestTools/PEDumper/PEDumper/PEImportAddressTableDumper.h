#pragma once

#include "PEElementDumper.h"

class PEImportAddressTableDumper :
    public PEElementDumper
{
public:
    PEImportAddressTableDumper() : PEElementDumper("ImportAddressTable") {}

    PEImportAddressTableDumper & SetDumpElement(_In_ IPEImportAddressTable *importAddressTable) {
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

    PEImportAddressBlockDumper & SetDumpElement(_In_ IPEImportAddressBlock *importAddressBlock) {
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

    PEImportAddressItemDumper & SetDumpElement(_In_ IPEImportAddressItem *importAddressItem) {
        _importAddressItem = importAddressItem; 
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEImportAddressItem> _importAddressItem;
};
