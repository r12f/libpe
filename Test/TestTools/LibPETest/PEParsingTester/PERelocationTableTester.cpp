#include "stdafx.h"
#include "PERelocationTableTester.h"

void PERelocationTableTester::DoTest()
{
    LibPEPtr<IPERelocationTable> relocationTable;
    GetPEFile()->GetRelocationTable(&relocationTable);
    ASSERT_EQ(relocationTable != nullptr, GetExpectedResult() != nullptr);

    if (relocationTable == nullptr) {
        return;
    }

    RunTestLoop([&, this]() {
        TestBasicInformation(relocationTable);
    });

    UINT32 relocationPageCount = relocationTable->GetPageCount();
    EXPECT_EQ(GetExpectedResult()->UnsignedAttribute("relocation-page-count"), relocationPageCount);

    tinyxml2::XMLElement *relocationPageExpectedResult = nullptr;
    for (UINT32 relocationPageIndex = 0; relocationPageIndex < relocationPageCount; ++relocationPageIndex) {
        LibPEPtr<IPERelocationPage> relocationPage;
        ASSERT_HRESULT_SUCCEEDED(relocationTable->GetPageByIndex(relocationPageIndex, &relocationPage));
        ASSERT_NE(relocationPage, nullptr);

        PERelocationPageTester()
            .SetRelocationPage(relocationPage)
            .SetPEFile(GetPEFile())
            .SetExpectedResult(GetExpectedResult(), relocationPageExpectedResult, &relocationPageExpectedResult)
            .Run();

        ASSERT_NE(relocationPageExpectedResult, nullptr);
    }

    ASSERT_TRUE(PERelocationPageTester().IsLastExpectedResult(relocationPageExpectedResult));
}

void PERelocationPageTester::DoTest()
{
    RunTestLoop([&, this]() {
        TestBasicInformation(_relocationPage);
        TestRelocationPage(_relocationPage);
    });

    UINT32 relocationItemCount = _relocationPage->GetItemCount();
    EXPECT_EQ(GetExpectedResult()->UnsignedAttribute("relocation-item-count"), relocationItemCount);

    tinyxml2::XMLElement *relocationItemExpectedResult = nullptr;
    for (UINT32 relocationItemIndex = 0; relocationItemIndex < relocationItemCount; ++relocationItemIndex) {
        LibPEPtr<IPERelocationItem> relocationItem;
        ASSERT_HRESULT_SUCCEEDED(_relocationPage->GetItemByIndex(relocationItemIndex, &relocationItem));
        ASSERT_NE(relocationItem, nullptr);

        PERelocationItemTester()
            .SetRelocationItem(relocationItem)
            .SetPEFile(GetPEFile())
            .SetExpectedResult(GetExpectedResult(), relocationItemExpectedResult, &relocationItemExpectedResult)
            .Run();

        ASSERT_NE(relocationItemExpectedResult, nullptr);
    }

    ASSERT_TRUE(PERelocationItemTester().IsLastExpectedResult(relocationItemExpectedResult));
}

void PERelocationPageTester::TestRelocationPage(IPERelocationPage *relocationPage)
{
    BEGIN_TEST_PE_ELEMENT(relocationPage)
        TEST_RAW_FIELD_SIMPLE(IPERelocationPage, VirtualAddress)
        TEST_RAW_FIELD_SIMPLE(IPERelocationPage, SizeOfBlock)
        TEST_API_RESULT_SIMPLE(IPERelocationPage, GetPageRVA)
    END_TEST_PE_ELEMENT()
}

void PERelocationItemTester::DoTest()
{
    RunTestLoop([&, this]() {
        TestBasicInformation(_relocationItem);
        TestRelocationItem(_relocationItem);
    });
}

void PERelocationItemTester::TestRelocationItem(IPERelocationItem *relocationItem)
{
    BEGIN_TEST_PE_ELEMENT(relocationItem)
        TEST_API_RESULT_SIMPLE(IPERelocationItem, GetAddressRVA)
        TEST_API_RESULT_SIMPLE(IPERelocationItem, GetAddressContent)
    END_TEST_PE_ELEMENT()
}
