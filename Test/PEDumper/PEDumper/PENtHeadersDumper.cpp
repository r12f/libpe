#include "stdafx.h"
#include "PENtHeadersDumper.h"

void PENtHeadersDumper::DoDump()
{
    LibPEPtr<IPENtHeaders> ntHeaders;
    if (FAILED(GetPEFile()->GetNtHeaders(&ntHeaders))) {
        return;
    }

    DumpBasicInformation(ntHeaders);

    BEGIN_DUMP_PE_ELEMENT(ntHeaders)
        DUMP_RAW_FIELD_SIMPLE(Signature)
    END_DUMP_PE_ELEMENT()

    PEFileHeaderDumper()
        .SetPEFile(GetPEFile())
        .SetParentElement(GetOutputElement())
        .Run();

    PEOptionalHeaderDumper()
        .SetPEFile(GetPEFile())
        .SetParentElement(GetOutputElement())
        .Run();
}

void PEFileHeaderDumper::DoDump()
{
    LibPEPtr<IPEFileHeader> fileHeader;
    if (FAILED(GetPEFile()->GetFileHeader(&fileHeader))) {
        return;
    }

    DumpBasicInformation(fileHeader);

    BEGIN_DUMP_PE_ELEMENT(fileHeader)
        DUMP_RAW_FIELD_SIMPLE(Machine)
        DUMP_RAW_FIELD_SIMPLE(NumberOfSections)
        DUMP_RAW_FIELD_SIMPLE(TimeDateStamp)
        DUMP_RAW_FIELD_SIMPLE(PointerToSymbolTable)
        DUMP_RAW_FIELD_SIMPLE(NumberOfSymbols)
        DUMP_RAW_FIELD_SIMPLE(SizeOfOptionalHeader)
        DUMP_RAW_FIELD_SIMPLE(Characteristics)
    END_DUMP_PE_ELEMENT()
}

void PEOptionalHeaderDumper::DoDump()
{
    LibPEPtr<IPEOptionalHeader> optionalHeader;
    if (FAILED(GetPEFile()->GetOptionalHeader(&optionalHeader))) {
        return;
    }

    DumpBasicInformation(optionalHeader);

    BEGIN_DUMP_PE_ELEMENT(optionalHeader)
        DUMP_RAW_FIELD_SIMPLE(Magic)
        DUMP_RAW_FIELD_SIMPLE(MajorLinkerVersion)
        DUMP_RAW_FIELD_SIMPLE(MinorLinkerVersion)
        DUMP_RAW_FIELD_SIMPLE(SizeOfCode)
        DUMP_RAW_FIELD_SIMPLE(SizeOfInitializedData)
        DUMP_RAW_FIELD_SIMPLE(SizeOfUninitializedData)
        DUMP_RAW_FIELD_SIMPLE(AddressOfEntryPoint)
        DUMP_RAW_FIELD_SIMPLE(BaseOfCode)
        DUMP_RAW_FIELD_SIMPLE(ImageBase)
        DUMP_RAW_FIELD_SIMPLE(SectionAlignment)
        DUMP_RAW_FIELD_SIMPLE(FileAlignment)
        DUMP_RAW_FIELD_SIMPLE(MajorOperatingSystemVersion)
        DUMP_RAW_FIELD_SIMPLE(MinorOperatingSystemVersion)
        DUMP_RAW_FIELD_SIMPLE(MajorImageVersion)
        DUMP_RAW_FIELD_SIMPLE(MinorImageVersion)
        DUMP_RAW_FIELD_SIMPLE(MajorSubsystemVersion)
        DUMP_RAW_FIELD_SIMPLE(MinorSubsystemVersion)
        DUMP_RAW_FIELD_SIMPLE(Win32VersionValue)
        DUMP_RAW_FIELD_SIMPLE(SizeOfImage)
        DUMP_RAW_FIELD_SIMPLE(SizeOfHeaders)
        DUMP_RAW_FIELD_SIMPLE(CheckSum)
        DUMP_RAW_FIELD_SIMPLE(Subsystem)
        DUMP_RAW_FIELD_SIMPLE(DllCharacteristics)
        DUMP_RAW_FIELD_SIMPLE(SizeOfStackReserve)
        DUMP_RAW_FIELD_SIMPLE(SizeOfStackCommit)
        DUMP_RAW_FIELD_SIMPLE(SizeOfHeapReserve)
        DUMP_RAW_FIELD_SIMPLE(SizeOfHeapCommit)
        DUMP_RAW_FIELD_SIMPLE(LoaderFlags)
        DUMP_RAW_FIELD_SIMPLE(NumberOfRvaAndSizes)
    END_DUMP_PE_ELEMENT()
}
