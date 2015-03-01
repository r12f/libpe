#include "stdafx.h"
#include "PEElementDumper.h"

PEElementDumper::PEElementDumper(const char *elementName)
    : _elementName(elementName)
    , _parentElement(nullptr)
{
}

PEElementDumper::~PEElementDumper()
{
}

void
PEElementDumper::Run()
{
    BeginDump();
    DoDump();
}

void
PEElementDumper::BeginDump()
{
    _outputElement =  CreateChildXmlElement(_elementName.c_str(), _parentElement);
    _outputAPIResults = CreateChildXmlElement("APIResults", _outputElement);
}

void 
PEElementDumper::DumpBasicInformation(IPEElement *peElement)
{
    BEGIN_DUMP_PE_ELEMENT(peElement)
        DUMP_API_RESULT_SIMPLE(GetRVA)
        DUMP_API_RESULT_SIMPLE(GetVA)
        DUMP_API_RESULT_SIMPLE(GetSizeInMemory)
        DUMP_API_RESULT_SIMPLE(GetFOA)
        DUMP_API_RESULT_SIMPLE(GetSizeInFile)
        DUMP_API_RESULT_SIMPLE(GetRawOffset)
        DUMP_API_RESULT_SIMPLE(GetRawSize)
    END_DUMP_PE_ELEMENT()
}

tinyxml2::XMLElement *
PEElementDumper::CreateChildXmlElement(const char *tagName, tinyxml2::XMLElement *parentElement)
{
    auto newElement = parentElement->GetDocument()->NewElement(tagName);
    parentElement->InsertEndChild(newElement);
    return newElement;
}
