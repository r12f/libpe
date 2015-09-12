#include "stdafx.h"
#include "PESectionDumper.h"

void PESectionListDumper::DoDump()
{
    UINT32 sectionCount = GetPEFile()->GetSectionCount();
    GetOutputElement()->SetAttribute("section-count", sectionCount);

    for (UINT32 sectionIndex = 0; sectionIndex < sectionCount; ++sectionIndex) {
        LibPEPtr<IPESection> section;
        if (FAILED(GetPEFile()->GetSection(sectionIndex, &section))) {
            continue;
        }

        PESectionDumper()
            .SetSection(section)
            .SetPEFile(GetPEFile())
            .SetParentElement(GetOutputElement())
            .Run();
    }
}

void PESectionDumper::DoDump()
{
    DumpBasicInformation(_section);

    BEGIN_DUMP_PE_ELEMENT(_section)
        DUMP_API_RESULT_SIMPLE(GetName)
        DUMP_API_RESULT_SIMPLE(GetCharacteristics)
    END_DUMP_PE_ELEMENT()
}
