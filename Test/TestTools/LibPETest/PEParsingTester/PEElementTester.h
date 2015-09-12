#pragma once

class PEElementTester
{
public:
    PEElementTester(const char *elementName);
    virtual ~PEElementTester();

    PEElementTester & SetPEFile(IPEFile *peFile);

    PEElementTester & SetExpectedResult(
        tinyxml2::XMLElement *parentExpectedResult,
        tinyxml2::XMLElement *lastExpectedResult = nullptr,
        tinyxml2::XMLElement **currentExpectedResult = nullptr);

    bool IsLastExpectedResult(tinyxml2::XMLElement *expectedResult);

    void Run();

protected:
    bool BeginTest();
    virtual void DoTest() = 0;

    void RunTestLoop(std::function<void ()> callback);
    void BeginTestSingleAPI(tinyxml2::XMLElement *expectedAPIResult);
    void EndTestSingleAPI();

    template <class Element, class Data>
    void TestAPIResultSimpleT(Element *element, Data (LIBPE_CALLTYPE Element::*func)(), const char *apiName)
    {
        if (strcmp(apiName, GetAPINameOfCurrentTest()) != 0) {
            return;
        }

        Data v;
        ASSERT_EQ(tinyxml2::XML_SUCCESS, GetExpectedAPIResultOfCurrentTest()->QueryAttribute("value", &v));
        EXPECT_EQ(v, (element->*func)());
    }

    template <class Element>
    void TestAPIResultStringT(Element *element, const char * (LIBPE_CALLTYPE Element::*func)(), const char *apiName)
    {
        if (strcmp(apiName, GetAPINameOfCurrentTest()) != 0) {
            return;
        }

        const char *v = GetExpectedAPIResultOfCurrentTest()->Attribute("value");
        EXPECT_STRCASEEQ(v, (element->*func)());
    }

    template <class Element, class Data>
    void TestRawFieldArrayT(
        Element *element,
        UINT32(LIBPE_CALLTYPE Element::*funcGetCount)(),
        Data(LIBPE_CALLTYPE Element::*funcGetItem)(UINT32),
        const char *apiName)
    {
        if (strcmp(apiName, GetAPINameOfCurrentTest()) != 0) {
            return;
        }

        UINT32 expectedArrayItemCount = 0;
        ASSERT_EQ(tinyxml2::XML_SUCCESS, GetExpectedAPIResultOfCurrentTest()->QueryUnsignedAttribute("count", &expectedArrayItemCount));
        ASSERT_EQ(expectedArrayItemCount, (element->*funcGetCount)());

        tinyxml2::XMLElement *expectedArrayItemXmlNode = GetExpectedAPIResultOfCurrentTest()->FirstChildElement("Item");
        for (UINT32 arrayItemIndex = 0; arrayItemIndex < expectedArrayItemCount; ++arrayItemIndex) {
            ASSERT_NE(expectedArrayItemXmlNode, nullptr);

            Data item = (element->*funcGetItem)(arrayItemIndex);
            Data expectedItem;
            ASSERT_NE(tinyxml2::XML_SUCCESS, expectedArrayItemXmlNode->QueryAttribute("value", &expectedItem));
            EXPECT_EQ(expectedItem, item);

            expectedArrayItemXmlNode = expectedArrayItemXmlNode->NextSiblingElement("Item");
        }
    }

    void TestBasicInformation(IPEElement *peElement);

    const char * GetElementName() const { return _elementName.c_str(); }

    // These fields below can only be used in RunTest function, because they are fetched
    // from parameters of this function, and we saved them so that they could be used easier.
    IPEFile * GetPEFile() { return _peFile; }
    tinyxml2::XMLElement * GetParentExpectedResult() { return _parentExpectedResult; }
    tinyxml2::XMLElement * GetExpectedResult() { return _expectedResult; }
    tinyxml2::XMLElement * GetExpectedAPIResults() { return _expectedAPIResults; }

    tinyxml2::XMLElement * GetExpectedAPIResultOfCurrentTest() { return _expectedAPIResultOfCurrentTest; }
    const char * GetAPINameOfCurrentTest() { return _apiNameOfCurrentTest; }

private:
    std::string _elementName;

    // These fields below can only be used in RunTest function, because they are fetched
    // from parameters of this function, and we saved them so that they could be used easier.
    IPEFile *_peFile;
    tinyxml2::XMLElement *_parentExpectedResult;
    tinyxml2::XMLElement *_expectedResult;
    tinyxml2::XMLElement *_expectedAPIResults;

    tinyxml2::XMLElement *_expectedAPIResultOfCurrentTest;
    const char *_apiNameOfCurrentTest;
};

#define BEGIN_TEST_PE_ELEMENT(peElement)                                                        \
    do {                                                                                        \
        auto __peElement = peElement;

#define END_TEST_PE_ELEMENT()                                                                   \
        } while (0);

#define TEST_API_RESULT_SIMPLE(peElementType, apiName)                                          \
    do {                                                                                        \
        TestAPIResultSimpleT((peElementType *)__peElement, &peElementType::apiName, #apiName);  \
    } while (0);

#define TEST_API_RESULT_STRING(peElementType, apiName)                                          \
    do {                                                                                        \
        TestAPIResultStringT((peElementType *)__peElement, &peElementType::apiName, #apiName);  \
    } while (0);

#define TEST_RAW_FIELD_SIMPLE(peElementType, fieldName)                                         \
     TEST_API_RESULT_SIMPLE(peElementType, GetField ## fieldName)

#define TEST_RAW_FIELD_ARRAY(peElementType, fieldName)                                          \
    do {                                                                                        \
        TestRawFieldArrayT(                                                                     \
            (peElementType *)__peElement,                                                       \
            &peElementType::GetField ## fieldName ## ElementCount,                              \
            &peElementType::GetField ## fieldName, #fieldName);                                 \
    } while (0);
