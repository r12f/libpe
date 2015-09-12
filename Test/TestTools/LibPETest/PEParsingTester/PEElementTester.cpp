#include "stdafx.h"
#include "PEElementTester.h"

PEElementTester::PEElementTester(const char *elementName)
    : _elementName(elementName)
{
}

PEElementTester::~PEElementTester()
{
}

PEElementTester & PEElementTester::SetPEFile(IPEFile *peFile)
{
    _peFile = peFile;
    return *this;
}

PEElementTester & PEElementTester::SetExpectedResult(tinyxml2::XMLElement *parentExpectedResult, tinyxml2::XMLElement *lastExpectedResult, tinyxml2::XMLElement **currentExpectedResult)
{
    _parentExpectedResult = parentExpectedResult;

    if (lastExpectedResult == nullptr) {
        _expectedResult = _parentExpectedResult->FirstChildElement(_elementName.c_str());
    }
    else {
        _expectedResult = lastExpectedResult->NextSiblingElement(_elementName.c_str());
    }

    if (currentExpectedResult != nullptr) {
        *currentExpectedResult = _expectedResult;
    }

    return *this;
}

bool PEElementTester::IsLastExpectedResult(tinyxml2::XMLElement *expectedResult)
{
    // If expected result is nullptr, it should be the last onw, because the list should be empty.
    if (expectedResult == nullptr) {
        return true;
    }

    return expectedResult->NextSiblingElement(_elementName.c_str()) == nullptr;
}

void PEElementTester::Run()
{
    ASSERT_EQ(BeginTest(), true);
    DoTest();
}

bool PEElementTester::BeginTest()
{
    if (_parentExpectedResult == nullptr) {
        return false;
    }

    // The expected result can be missing, because this element might not exist.
    if (_expectedResult != nullptr) {
        // The APIResults will always exist, because all the elements have RVA, FOA getters at least.
        _expectedAPIResults = _expectedResult->FirstChildElement("APIResults");
        if (_expectedAPIResults == nullptr) {
            return false;
        }
    }
    
    return true;
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
