#pragma once

#include "PEElementTester.h"

class PEImportAddressTableTester :
    public PEElementTester
{
public:
    PEImportAddressTableTester() : PEElementTester("ImportAddressTable") {}

protected:
    void DoTest() override;
};

class PEImportAddressBlockTester :
    public PEElementTester
{
public:
    PEImportAddressBlockTester() : PEElementTester("ImportAddressBlock") {}

    PEImportAddressBlockTester & SetImportAddressBlock(_In_ IPEImportAddressBlock *importAddressBlock) {
        _importAddressBlock = importAddressBlock;
        return *this;
    }

protected:
    void DoTest() override;

private:
    LibPEPtr<IPEImportAddressBlock> _importAddressBlock;
};

class PEImportAddressItemTester :
    public PEElementTester
{
public:
    PEImportAddressItemTester() : PEElementTester("ImportAddressItem") {}

    PEImportAddressItemTester & SetImportAddressItem(_In_ IPEImportAddressItem *importAddressItem) {
        _importAddressItem = importAddressItem;
        return *this;
    }

protected:
    void DoTest() override;
    void TestImportAddressItem(_In_ IPEImportAddressItem *importAddressItem);

private:
    LibPEPtr<IPEImportAddressItem> _importAddressItem;
};
