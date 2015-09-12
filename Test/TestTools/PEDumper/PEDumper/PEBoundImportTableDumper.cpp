#include "stdafx.h"
#include "PEBoundImportTableDumper.h"

void PEBoundImportTableDumper::DoDump()
{
    DumpBasicInformation(_boundImportTable);

    UINT32 boundImportModuleCount = _boundImportTable->GetBoundImportModuleCount();
    GetOutputElement()->SetAttribute("bound-import-module-count", boundImportModuleCount);

    for (UINT32 boundImportModuleIndex = 0; boundImportModuleIndex < boundImportModuleCount; ++boundImportModuleIndex) {
        LibPEPtr<IPEBoundImportModule> boundImportModule;
        if (FAILED(_boundImportTable->GetBoundImportModuleByIndex(boundImportModuleIndex, &boundImportModule))) {
            continue;
        }

        PEBoundImportModuleDumper()
            .SetDumpeElement(boundImportModule)
            .SetPEFile(GetPEFile())
            .SetParentElement(GetOutputElement())
            .Run();
    }
}

void PEBoundImportModuleDumper::DoDump()
{
    DumpBasicInformation(_boundImportModule);

    BEGIN_DUMP_PE_ELEMENT(_boundImportModule)
        DUMP_RAW_FIELD_SIMPLE(TimeDateStamp)
        DUMP_RAW_FIELD_SIMPLE(OffsetModuleName)
        DUMP_RAW_FIELD_SIMPLE(NumberOfModuleForwarderRefs)
    END_DUMP_PE_ELEMENT()

    UINT32 boundForwarderCount = _boundImportModule->GetBoundForwarderCount();
    GetOutputElement()->SetAttribute("bound-forwarder-count", boundForwarderCount);

    for (UINT32 boundForwarderIndex = 0; boundForwarderIndex < boundForwarderCount; ++boundForwarderIndex) {
        LibPEPtr<IPEBoundForwarder> boundForwarder;
        if (FAILED(_boundImportModule->GetBoundForwarderByIndex(boundForwarderIndex, &boundForwarder))) {
            continue;
        }

        PEBoundForwarderDumper()
            .SetDumpElement(boundForwarder)
            .SetPEFile(GetPEFile())
            .SetParentElement(GetOutputElement())
            .Run();
    }
}

void PEBoundForwarderDumper::DoDump()
{
    DumpBasicInformation(_boundForwarder);

    BEGIN_DUMP_PE_ELEMENT(_boundForwarder)
        DUMP_RAW_FIELD_SIMPLE(TimeDateStamp)
        DUMP_RAW_FIELD_SIMPLE(OffsetModuleName)
        DUMP_RAW_FIELD_SIMPLE(Reserved)
    END_DUMP_PE_ELEMENT()
}

