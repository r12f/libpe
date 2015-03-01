#pragma once

#include "PEElementTester.h"

class PEFileHeaderTester :
    public PEElementTester
{
public:
    PEFileHeaderTester() : PEElementTester("FileHeader") {}

protected:
    void DoTest() override
    {
        LibPEPtr<IPEFileHeader> fileHeader;
        ASSERT_HRESULT_SUCCEEDED(GetPEFile()->GetFileHeader(&fileHeader));
        ASSERT_NE(fileHeader, nullptr);

        RunTestLoop([&, this]() {
            TestBasicInformation(fileHeader);
            TestFileHeader(fileHeader);
        });
    }

    void TestFileHeader(IPEFileHeader *fileHeader)
    {
        BEGIN_TEST_PE_ELEMENT(fileHeader)
            TEST_RAW_FIELD_SIMPLE(IPEFileHeader, Machine)
            TEST_RAW_FIELD_SIMPLE(IPEFileHeader, NumberOfSections)
            TEST_RAW_FIELD_SIMPLE(IPEFileHeader, TimeDateStamp)
            TEST_RAW_FIELD_SIMPLE(IPEFileHeader, PointerToSymbolTable)
            TEST_RAW_FIELD_SIMPLE(IPEFileHeader, NumberOfSymbols)
            TEST_RAW_FIELD_SIMPLE(IPEFileHeader, SizeOfOptionalHeader)
            TEST_RAW_FIELD_SIMPLE(IPEFileHeader, Characteristics)
        END_TEST_PE_ELEMENT()
    }
};

class PEOptionalHeaderTester :
    public PEElementTester
{
public:
    PEOptionalHeaderTester() : PEElementTester("OptionalHeader") {}

protected:
    void DoTest() override
    {
        LibPEPtr<IPEOptionalHeader> optionalHeader;
        ASSERT_HRESULT_SUCCEEDED(GetPEFile()->GetOptionalHeader(&optionalHeader));
        ASSERT_NE(optionalHeader, nullptr);

        RunTestLoop([&, this]() {
            TestBasicInformation(optionalHeader);
            TestOptionalHeader(optionalHeader);
        });
    }

    void TestOptionalHeader(IPEOptionalHeader *optionalHeader)
    {
        BEGIN_TEST_PE_ELEMENT(optionalHeader)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, Magic)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, MajorLinkerVersion)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, MinorLinkerVersion)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, SizeOfCode)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, SizeOfInitializedData)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, SizeOfUninitializedData)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, AddressOfEntryPoint)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, BaseOfCode)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, ImageBase)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, SectionAlignment)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, FileAlignment)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, MajorOperatingSystemVersion)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, MinorOperatingSystemVersion)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, MajorImageVersion)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, MinorImageVersion)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, MajorSubsystemVersion)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, MinorSubsystemVersion)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, Win32VersionValue)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, SizeOfImage)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, SizeOfHeaders)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, CheckSum)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, Subsystem)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, DllCharacteristics)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, SizeOfStackReserve)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, SizeOfStackCommit)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, SizeOfHeapReserve)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, SizeOfHeapCommit)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, LoaderFlags)
            TEST_RAW_FIELD_SIMPLE(IPEOptionalHeader, NumberOfRvaAndSizes)
        END_TEST_PE_ELEMENT()
    }
};

class PENtHeadersTester :
    public PEElementTester
{
public:
    PENtHeadersTester() : PEElementTester("NtHeaders") {}

protected:
    void DoTest() override
    {
        LibPEPtr<IPENtHeaders> ntHeaders;
        ASSERT_HRESULT_SUCCEEDED(GetPEFile()->GetNtHeaders(&ntHeaders));
        ASSERT_NE(ntHeaders, nullptr);

        RunTestLoop([&, this]() {
            TestBasicInformation(ntHeaders);
            TestNtHeaders(ntHeaders);
        });

        _fileHeaderTester.Run(GetPEFile(), GetExpectedElement());
        _optionalHeaderTester.Run(GetPEFile(), GetExpectedElement());
    }

    void TestNtHeaders(IPENtHeaders *ntHeaders)
    {
        BEGIN_TEST_PE_ELEMENT(ntHeaders)
            TEST_RAW_FIELD_SIMPLE(IPENtHeaders, Signature)
        END_TEST_PE_ELEMENT()
    }

private:
    PEFileHeaderTester _fileHeaderTester;
    PEOptionalHeaderTester _optionalHeaderTester;
};