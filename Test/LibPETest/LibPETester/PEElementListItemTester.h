#pragma once

#include "PEElementTester.h"

class PEElementListItemTester :
    protected PEElementTester
{
public:
    PEElementListItemTester(const char *elementName) : PEElementTester(elementName) {}
    virtual ~PEElementListItemTester() {}

    void Run(IPEFile *peFile, tinyxml2::XMLElement *expectedParentElement, UINT32 itemIndex);

protected:
    UINT32 GetItemIndex() const { return _itemIndex; }

private:
    bool BeginTest(IPEFile *peFile, tinyxml2::XMLElement *expectedParentElement, UINT32 itemIndex);
    void EndTest();

private:
    UINT32 _itemIndex;
};
