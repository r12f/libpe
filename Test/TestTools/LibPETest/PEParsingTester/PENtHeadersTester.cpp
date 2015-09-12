#include "stdafx.h"
#include "PENtHeadersTester.h"

void PENtHeadersTester::DoTest()
{
    // NtHeaders is an optional structure for PE files.
    LibPEPtr<IPENtHeaders> ntHeaders;
    GetPEFile()->GetNtHeaders(&ntHeaders);
    ASSERT_EQ(ntHeaders != nullptr, GetExpectedResult() != nullptr);

    if (ntHeaders == nullptr) {
        return;
    }

    RunTestLoop([&, this]() {
        TestBasicInformation(ntHeaders);
        TestNtHeaders(ntHeaders);
    });

    PEFileHeaderTester()
        .SetPEFile(GetPEFile())
        .SetExpectedResult(GetExpectedResult())
        .Run();

    PEOptionalHeaderTester()
        .SetPEFile(GetPEFile())
        .SetExpectedResult(GetExpectedResult())
        .Run();
}

void PENtHeadersTester::TestNtHeaders(IPENtHeaders *ntHeaders)
{
    BEGIN_TEST_PE_ELEMENT(ntHeaders)
        TEST_RAW_FIELD_SIMPLE(IPENtHeaders, Signature)
    END_TEST_PE_ELEMENT()
}

void PEFileHeaderTester::DoTest()
{
    // If NtHeaders exists, FileHeader must exist.
    LibPEPtr<IPEFileHeader> fileHeader;
    ASSERT_HRESULT_SUCCEEDED(GetPEFile()->GetFileHeader(&fileHeader));
    ASSERT_NE(fileHeader, nullptr);

    RunTestLoop([&, this]() {
        TestBasicInformation(fileHeader);
        TestFileHeader(fileHeader);
    });
}

void PEFileHeaderTester::TestFileHeader(IPEFileHeader *fileHeader)
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

void PEOptionalHeaderTester::DoTest()
{
    // If NtHeaders exists, OptionalHeader must exist.
    LibPEPtr<IPEOptionalHeader> optionalHeader;
    ASSERT_HRESULT_SUCCEEDED(GetPEFile()->GetOptionalHeader(&optionalHeader));
    ASSERT_NE(optionalHeader, nullptr);

    RunTestLoop([&, this]() {
        TestBasicInformation(optionalHeader);
        TestOptionalHeader(optionalHeader);
    });
}

void PEOptionalHeaderTester::TestOptionalHeader(IPEOptionalHeader *optionalHeader)
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
