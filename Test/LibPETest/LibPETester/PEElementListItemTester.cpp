#include "stdafx.h"
#include "PEElementListItemTester.h"

void
PEElementListItemTester::Run(IPEFile *peFile, tinyxml2::XMLElement *expectedParentElement, UINT32 itemIndex)
{
    ASSERT_EQ(BeginTest(peFile, expectedParentElement, itemIndex), true);
    DoTest();
    EndTest();
}

bool
PEElementListItemTester::BeginTest(IPEFile *peFile, tinyxml2::XMLElement *expectedParentElement, UINT32 itemIndex)
{
    if (expectedParentElement == nullptr) {
        return false;
    }

    tinyxml2::XMLElement *expectedElement = expectedParentElement->FirstChildElement(GetElementName());
    for (UINT32 expectedElementIndex = 0; expectedElement != nullptr && expectedElementIndex < itemIndex; ++expectedElementIndex) {
        expectedElement = expectedElement->NextSiblingElement(GetElementName());
    }

    if (expectedElement == nullptr) {
        return false;
    }

    if (!PEElementTester::BeginTest(peFile, expectedParentElement, expectedElement)) {
        return false;
    }

    _itemIndex = itemIndex;

    return true;
}

void
PEElementListItemTester::EndTest()
{
    PEElementTester::EndTest();
    _itemIndex = 0;
}
