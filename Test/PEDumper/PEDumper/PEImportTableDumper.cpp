#include "stdafx.h"
#include "PEImportTableDumper.h"

void PEImportTableDumper::DoDump()
{
    LibPEPtr<IPEImportTable> importTable;
    if (FAILED(GetPEFile()->GetImportTable(&importTable))) {
        return;
    }

    DumpBasicInformation(importTable);

    UINT32 importModuleCount = importTable->GetModuleCount();
    GetOutputElement()->SetAttribute("import-module-count", importModuleCount);

    for (UINT32 importModuleIndex = 0; importModuleIndex < importModuleCount; ++importModuleIndex) {
        LibPEPtr<IPEImportModule> importModule;
        if (FAILED(importTable->GetModuleByIndex(importModuleIndex, &importModule))) {
            continue;
        }

        PEImportModuleDumper()
            .SetImportModule(importModule)
            .SetPEFile(GetPEFile())
            .SetParentElement(GetOutputElement())
            .Run();
    }
}

void PEImportModuleDumper::DoDump()
{
    GetOutputElement()->SetAttribute("name", _importModule->GetName());

    DumpBasicInformation(_importModule);

    BEGIN_DUMP_PE_ELEMENT(_importModule)
        DUMP_RAW_FIELD_SIMPLE(Characteristics)
        DUMP_RAW_FIELD_SIMPLE(OriginalFirstThunk)
        DUMP_RAW_FIELD_SIMPLE(TimeDateStamp)
        DUMP_RAW_FIELD_SIMPLE(ForwarderChain)
        DUMP_RAW_FIELD_SIMPLE(Name)
        DUMP_RAW_FIELD_SIMPLE(FirstThunk)
        DUMP_API_RESULT_SIMPLE(IsBound)
        DUMP_API_RESULT_SIMPLE(GetName)
    END_DUMP_PE_ELEMENT()

    UINT32 importFunctionCount = _importModule->GetFunctionCount();
    GetOutputElement()->SetAttribute("import-function-count", importFunctionCount);

    for (UINT32 importFunctionIndex = 0; importFunctionIndex < importFunctionCount; ++importFunctionIndex) {
        LibPEPtr<IPEImportFunction> importFunction;
        if (FAILED(_importModule->GetFunctionByIndex(importFunctionIndex, &importFunction))) {
            continue;
        }

        PEImportFunctionDumper()
            .SetImportFunction(importFunction)
            .SetPEFile(GetPEFile())
            .SetParentElement(GetOutputElement())
            .Run();
    }
}

void PEImportFunctionDumper::DoDump()
{
    GetOutputElement()->SetAttribute("name", _importFunction->GetName());

    DumpBasicInformation(_importFunction);

    BEGIN_DUMP_PE_ELEMENT(_importFunction)
        DUMP_RAW_FIELD_SIMPLE(ForwarderString)
        DUMP_RAW_FIELD_SIMPLE(Function)
        DUMP_RAW_FIELD_SIMPLE(Ordinal)
        DUMP_RAW_FIELD_SIMPLE(AddressOfData)
        DUMP_API_RESULT_SIMPLE(GetName)
        DUMP_API_RESULT_SIMPLE(GetOrdinal)
    END_DUMP_PE_ELEMENT()
}

