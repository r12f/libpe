#pragma once

class PEElementDumper
{
public:
    PEElementDumper(const char *elementName);
    virtual ~PEElementDumper();

    PEElementDumper & SetPEFile(LibPE::IPEFile *peFile) { _peFile = peFile; return *this; }
    PEElementDumper & SetParentElement(tinyxml2::XMLElement *parentElement) { _parentElement = parentElement; return *this; }

    void Run();

protected:
    void BeginDump();
    virtual void DoDump() = 0;

    void DumpBasicInformation(IPEElement *pElement);

    LibPE::IPEFile * GetPEFile() { return _peFile; }
    tinyxml2::XMLElement * GetOutputElement() { return _outputElement; }
    tinyxml2::XMLElement * GetOutputAPIResults() { return _outputAPIResults; }

    tinyxml2::XMLElement * CreateChildXmlElement(const char *tagName, tinyxml2::XMLElement *parentElement);

private:
    std::string _elementName;
    LibPEPtr<IPEFile> _peFile;
    tinyxml2::XMLElement *_parentElement;
    tinyxml2::XMLElement *_outputElement;
    tinyxml2::XMLElement *_outputAPIResults;
};

#define BEGIN_DUMP_PE_ELEMENT(peElement)                                                        \
    do {                                                                                        \
        auto __peElement = peElement;

#define END_DUMP_PE_ELEMENT()                                                                   \
    } while (0);

#define DUMP_API_RESULT_SIMPLE(resultName)                                                      \
    do {                                                                                        \
        auto rawFieldElement = CreateChildXmlElement("Result", GetOutputAPIResults());          \
        rawFieldElement->SetAttribute("name", #resultName);                                     \
        rawFieldElement->SetAttribute("value", __peElement->resultName());                      \
    } while (0);

#define DUMP_RAW_FIELD_SIMPLE(fieldName)    DUMP_API_RESULT_SIMPLE(GetField ## fieldName)

#define DUMP_RAW_FIELD_ARRAY(fieldName)                                                         \
    do {                                                                                        \
        auto rawFieldElement = CreateChildXmlElement("Field", GetOutputAPIResults());           \
        rawFieldElement->SetAttribute("name", "GetField" #fieldName);                           \
                                                                                                \
        UINT32 elementCount = __peElement->GetField ## fieldName ## ElementCount();             \
        rawFieldElement->SetAttribute("count", elementCount);                                   \
                                                                                                \
        for (UINT32 elementIndex = 0; elementIndex < elementCount; ++elementIndex) {            \
            auto rawArrayItemElement = CreateChildXmlElement("Item", rawFieldElement);          \
            auto rawArrayItem = __peElement->GetField ## fieldName(elementIndex);               \
            rawArrayItemElement->SetAttribute("value", rawArrayItem);                           \
        }                                                                                       \
    } while (0);
