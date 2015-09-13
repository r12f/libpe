#include "stdafx.h"
#include "PEImportAddressTableTester.h"

void PEImportAddressTableTester::DoTest()
{
    LibPEPtr<IPEImportAddressTable> importAddressTable;
    GetPEFile()->GetImportAddressTable(&importAddressTable);
    ASSERT_EQ(importAddressTable != nullptr, GetExpectedResult() != nullptr);

    if (importAddressTable == nullptr) {
        return;
    }

    RunTestLoop([&, this]() {
        TestBasicInformation(importAddressTable);
    });

    UINT32 importAddressBlockCount = importAddressTable->GetBlockCount();
    EXPECT_EQ(GetExpectedResult()->UnsignedAttribute("import-address-block-count"), importAddressBlockCount);

    tinyxml2::XMLElement *importAddressBlockExpectedResult = nullptr;
    for (UINT32 importAddressBlockIndex = 0; importAddressBlockIndex < importAddressBlockCount; ++importAddressBlockIndex) {
        LibPEPtr<IPEImportAddressBlock> importAddressBlock;
        ASSERT_HRESULT_SUCCEEDED(importAddressTable->GetBlockByIndex(importAddressBlockIndex, &importAddressBlock));
        ASSERT_NE(importAddressBlock, nullptr);

        PEImportAddressBlockTester()
            .SetImportAddressBlock(importAddressBlock)
            .SetPEFile(GetPEFile())
            .SetExpectedResult(GetExpectedResult(), importAddressBlockExpectedResult, &importAddressBlockExpectedResult)
            .Run();

        ASSERT_NE(importAddressBlockExpectedResult, nullptr);
    }

    ASSERT_TRUE(PEImportAddressBlockTester().IsLastExpectedResult(importAddressBlockExpectedResult));
}

void PEImportAddressBlockTester::DoTest()
{
    RunTestLoop([&, this]() {
        TestBasicInformation(_importAddressBlock);
    });

    UINT32 importAddressItemCount = _importAddressBlock->GetItemCount();
    EXPECT_EQ(GetExpectedResult()->UnsignedAttribute("import-address-item-count"), importAddressItemCount);

    tinyxml2::XMLElement *importAddressItemExpectedResult = nullptr;
    for (UINT32 importAddressItemIndex = 0; importAddressItemIndex < importAddressItemCount; ++importAddressItemIndex) {
        LibPEPtr<IPEImportAddressItem> importAddressItem;
        ASSERT_HRESULT_SUCCEEDED(_importAddressBlock->GetItemByIndex(importAddressItemIndex, &importAddressItem));
        ASSERT_NE(importAddressItem, nullptr);

        PEImportAddressItemTester()
            .SetImportAddressItem(importAddressItem)
            .SetPEFile(GetPEFile())
            .SetExpectedResult(GetExpectedResult(), importAddressItemExpectedResult, &importAddressItemExpectedResult)
            .Run();

        ASSERT_NE(importAddressItemExpectedResult, nullptr);
    }

    ASSERT_TRUE(PEImportAddressItemTester().IsLastExpectedResult(importAddressItemExpectedResult));
}

void PEImportAddressItemTester::DoTest()
{
    RunTestLoop([&, this]() {
        TestBasicInformation(_importAddressItem);
        TestImportAddressItem(_importAddressItem);
    });
}

void PEImportAddressItemTester::TestImportAddressItem(_In_ IPEImportAddressItem *importAddressItem)
{
    BEGIN_TEST_PE_ELEMENT(importAddressItem)
        TEST_RAW_FIELD_SIMPLE(IPEImportAddressItem, ForwarderString)
        TEST_RAW_FIELD_SIMPLE(IPEImportAddressItem, Function)
        TEST_RAW_FIELD_SIMPLE(IPEImportAddressItem, Ordinal)
        TEST_RAW_FIELD_SIMPLE(IPEImportAddressItem, AddressOfData)
    END_TEST_PE_ELEMENT()
}
