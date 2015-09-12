#include "stdafx.h"
#include "PESectionHeaderDumper.h"

void PESectionHeaderListDumper::DoDump()
{
    UINT32 sectionCount = GetPEFile()->GetSectionCount();
    GetOutputElement()->SetAttribute("section-header-count", sectionCount);

    for (UINT32 sectionIndex = 0; sectionIndex < sectionCount; ++sectionIndex) {
        LibPEPtr<IPESectionHeader> sectionHeader;
        if (FAILED(GetPEFile()->GetSectionHeader(sectionIndex, &sectionHeader))) {
            continue;
        }

        PESectionHeaderDumper()
            .SetSectionHeader(sectionHeader)
            .SetPEFile(GetPEFile())
            .SetParentElement(GetOutputElement())
            .Run();
    }
}

void PESectionHeaderDumper::DoDump()
{
    DumpBasicInformation(_sectionHeader);

    BEGIN_DUMP_PE_ELEMENT(_sectionHeader)
        DUMP_RAW_FIELD_SIMPLE(PhysicalAddress)
        DUMP_RAW_FIELD_SIMPLE(VirtualSize)
        DUMP_RAW_FIELD_SIMPLE(VirtualAddress)
        DUMP_RAW_FIELD_SIMPLE(SizeOfRawData)
        DUMP_RAW_FIELD_SIMPLE(PointerToRawData)
        DUMP_RAW_FIELD_SIMPLE(PointerToRelocations)
        DUMP_RAW_FIELD_SIMPLE(PointerToLinenumbers)
        DUMP_RAW_FIELD_SIMPLE(NumberOfRelocations)
        DUMP_RAW_FIELD_SIMPLE(NumberOfLinenumbers)
        DUMP_RAW_FIELD_SIMPLE(Characteristics)
    END_DUMP_PE_ELEMENT()
}
