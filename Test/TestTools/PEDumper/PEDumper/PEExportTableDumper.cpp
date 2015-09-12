#include "stdafx.h"
#include "PEExportTableDumper.h"

void PEExportTableDumper::DoDump()
{
    DumpBasicInformation(_exportTable);

    BEGIN_DUMP_PE_ELEMENT(_exportTable)
        DUMP_RAW_FIELD_SIMPLE(Characteristics)
        DUMP_RAW_FIELD_SIMPLE(TimeDateStamp)
        DUMP_RAW_FIELD_SIMPLE(MajorVersion)
        DUMP_RAW_FIELD_SIMPLE(MinorVersion)
        DUMP_RAW_FIELD_SIMPLE(Name)
        DUMP_RAW_FIELD_SIMPLE(Base)
        DUMP_RAW_FIELD_SIMPLE(NumberOfFunctions)
        DUMP_RAW_FIELD_SIMPLE(NumberOfNames)
        DUMP_RAW_FIELD_SIMPLE(AddressOfFunctions)
        DUMP_RAW_FIELD_SIMPLE(AddressOfNames)
        DUMP_RAW_FIELD_SIMPLE(AddressOfNameOrdinals)
    END_DUMP_PE_ELEMENT()

    UINT32 exportFunctionCount = _exportTable->GetFunctionCount();
    GetOutputElement()->SetAttribute("export-function-count", exportFunctionCount);

    for (UINT32 exportFunctionIndex = 0; exportFunctionIndex < exportFunctionCount; ++exportFunctionIndex) {
        LibPEPtr<IPEExportFunction> exportFunction;
        if (FAILED(_exportTable->GetFunctionByIndex(exportFunctionIndex, &exportFunction))) {
            continue;
        }

        PEExportFunctionDumper()
            .SetDumpElement(exportFunction)
            .SetPEFile(GetPEFile())
            .SetParentElement(GetOutputElement())
            .Run();
    }
}

void PEExportFunctionDumper::DoDump()
{
    GetOutputElement()->SetAttribute("name", _exportFunction->GetName());

    DumpBasicInformation(_exportFunction);

    BEGIN_DUMP_PE_ELEMENT(_exportFunction)
        DUMP_API_RESULT_SIMPLE(GetName)
        DUMP_API_RESULT_SIMPLE(GetOrdinal)
    END_DUMP_PE_ELEMENT()
}
