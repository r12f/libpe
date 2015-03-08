#pragma once

#include "PEElementTester.h"

class PENtHeadersTester :
    public PEElementTester
{
public:
    PENtHeadersTester() : PEElementTester("NtHeaders") {}

protected:
    void DoTest() override;
    void TestNtHeaders(IPENtHeaders *ntHeaders);
};

class PEFileHeaderTester :
    public PEElementTester
{
public:
    PEFileHeaderTester() : PEElementTester("FileHeader") {}

protected:
    void DoTest() override;
    void TestFileHeader(IPEFileHeader *fileHeader);
};

class PEOptionalHeaderTester :
    public PEElementTester
{
public:
    PEOptionalHeaderTester() : PEElementTester("OptionalHeader") {}

protected:
    void DoTest() override;
    void TestOptionalHeader(IPEOptionalHeader *optionalHeader);
};
