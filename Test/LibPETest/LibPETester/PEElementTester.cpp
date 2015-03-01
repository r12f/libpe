#include "stdafx.h"
#include "PEElementTester.h"

PEElementTester::PEElementTester(const char *elementName)
    : _elementName(elementName)
{
}

PEElementTester::~PEElementTester()
{
}

void PEElementTester::Run(IPEFile *peFile, tinyxml2::XMLElement *expectedParentElement)
{
    ASSERT_EQ(BeginTest(peFile, expectedParentElement), true);
    DoTest();
    EndTest();
}

bool PEElementTester::BeginTest(IPEFile *peFile, tinyxml2::XMLElement *expectedParentElement, tinyxml2::XMLElement *expectedElement)
{
    _peFile = peFile;

    _expectedParentElement = expectedParentElement;
    if (_expectedParentElement == nullptr) {
        return false;
    }

    if (expectedElement == nullptr) {
        _expectedElement = _expectedParentElement->FirstChildElement(_elementName.c_str());
    } else {
        _expectedElement = expectedElement;
    }

    if (_expectedElement == nullptr) {
        return false;
    }

    _expectedAPIResults = _expectedElement->FirstChildElement("APIResults");
    if (_expectedAPIResults == nullptr) {
        return false;
    }
    
    return true;
}

void PEElementTester::EndTest()
{
    _peFile = nullptr;
    _expectedParentElement = nullptr;
    _expectedElement = nullptr;
    _expectedAPIResults = nullptr;
}

void PEElementTester::RunTestLoop(std::function<void()> callback)
{
    auto expectedAPIResult = GetExpectedAPIResults()->FirstChildElement("Result");
    while (expectedAPIResult != nullptr) {
        BeginTestSingleAPI(expectedAPIResult);
        callback();
        EndTestSingleAPI();

        expectedAPIResult = expectedAPIResult->NextSiblingElement("Result");
    }
}

void PEElementTester::BeginTestSingleAPI(tinyxml2::XMLElement *expectedAPIResult)
{
    _expectedAPIResultOfCurrentTest = expectedAPIResult;
    _apiNameOfCurrentTest = _expectedAPIResultOfCurrentTest->Attribute("name");
}

void PEElementTester::EndTestSingleAPI()
{
    _expectedAPIResultOfCurrentTest = nullptr;
}

void PEElementTester::TestBasicInformation(IPEElement *peElement)
{
    BEGIN_TEST_PE_ELEMENT(peElement)
        TEST_API_RESULT_SIMPLE(IPEElement, GetRVA)
        TEST_API_RESULT_SIMPLE(IPEElement, GetVA)
        TEST_API_RESULT_SIMPLE(IPEElement, GetSizeInMemory)
        TEST_API_RESULT_SIMPLE(IPEElement, GetFOA)
        TEST_API_RESULT_SIMPLE(IPEElement, GetSizeInFile)
        TEST_API_RESULT_SIMPLE(IPEElement, GetRawOffset)
        TEST_API_RESULT_SIMPLE(IPEElement, GetRawSize)
    END_TEST_PE_ELEMENT()
}
