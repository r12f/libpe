#include "stdafx.h"
#include "PEResourceTableDumper.h"

void PEResourceTableDumper::DoDump()
{
    LibPEPtr<IPEResourceTable> resourceTable;
    if (FAILED(GetPEFile()->GetResourceTable(&resourceTable))) {
        return;
    }

    DumpBasicInformation(resourceTable);

    LibPEPtr<IPEResourceDirectory> resourceDirectory;
    if (FAILED(resourceTable->GetRootDirectory(&resourceDirectory))) {
        return;
    }

    PEResourceDirectoryDumper()
        .SetResourceDirectory(resourceDirectory)
        .SetPEFile(GetPEFile())
        .SetParentElement(GetOutputElement())
        .Run();
}

void PEResourceDirectoryDumper::DoDump()
{
    DumpBasicInformation(_resourceDirectory);

    BEGIN_DUMP_PE_ELEMENT(_resourceDirectory)
        DUMP_RAW_FIELD_SIMPLE(Characteristics)
        DUMP_RAW_FIELD_SIMPLE(TimeDateStamp)
        DUMP_RAW_FIELD_SIMPLE(MajorVersion)
        DUMP_RAW_FIELD_SIMPLE(MinorVersion)
        DUMP_RAW_FIELD_SIMPLE(NumberOfNamedEntries)
        DUMP_RAW_FIELD_SIMPLE(NumberOfIdEntries)
    END_DUMP_PE_ELEMENT()

    UINT32 resourceDirectoryEntryCount = _resourceDirectory->GetEntryCount();
    GetOutputElement()->SetAttribute("resource-directory-entry-count", resourceDirectoryEntryCount);

    for (UINT32 resourceDirectoryEntryIndex = 0; resourceDirectoryEntryIndex < resourceDirectoryEntryCount; ++resourceDirectoryEntryIndex) {
        LibPEPtr<IPEResourceDirectoryEntry> resourceDirectoryEntry;
        if (FAILED(_resourceDirectory->GetEntryByIndex(resourceDirectoryEntryIndex, &resourceDirectoryEntry))) {
            continue;
        }

        PEResourceDirectoryEntryDumper()
            .SetResourceDirectoryEntry(resourceDirectoryEntry)
            .SetPEFile(GetPEFile())
            .SetParentElement(GetOutputElement())
            .Run();
    }
}

void PEResourceDirectoryEntryDumper::DoDump()
{
    // GetOutputElement()->SetAttribute("name", _resourceDirectoryEntry->GetName());

    DumpBasicInformation(_resourceDirectoryEntry);

    BEGIN_DUMP_PE_ELEMENT(_resourceDirectoryEntry)
        DUMP_RAW_FIELD_SIMPLE(NameOffset)
        DUMP_RAW_FIELD_SIMPLE(NameIsString)
        DUMP_RAW_FIELD_SIMPLE(Name)
        DUMP_RAW_FIELD_SIMPLE(Id)
        DUMP_RAW_FIELD_SIMPLE(OffsetToData)
        DUMP_RAW_FIELD_SIMPLE(OffsetToDirectory)
        DUMP_RAW_FIELD_SIMPLE(DataIsDirectory)
        DUMP_API_RESULT_SIMPLE(IsNameId)
        DUMP_API_RESULT_SIMPLE(GetId)
        DUMP_API_RESULT_SIMPLE(IsNameString)
        // DUMP_API_RESULT_SIMPLE(GetName)
        DUMP_API_RESULT_SIMPLE(IsEntryDirectory)
        DUMP_API_RESULT_SIMPLE(IsEntryDataEntry)
    END_DUMP_PE_ELEMENT()

    if (_resourceDirectoryEntry->IsEntryDirectory()) {
        LibPEPtr<IPEResourceDirectory> resourceDirectory;
        if (SUCCEEDED(_resourceDirectoryEntry->GetDirectory(&resourceDirectory))) {
            PEResourceDirectoryDumper()
                .SetResourceDirectory(resourceDirectory)
                .SetPEFile(GetPEFile())
                .SetParentElement(GetOutputElement())
                .Run();
        }
    }
    else if (_resourceDirectoryEntry->IsEntryDataEntry()) {
        LibPEPtr<IPEResourceDataEntry> resourceDataEntry;
        if (SUCCEEDED(_resourceDirectoryEntry->GetDataEntry(&resourceDataEntry))) {
            PEResourceDataEntryDumper()
                .SetResourceDataEntry(resourceDataEntry)
                .SetPEFile(GetPEFile())
                .SetParentElement(GetOutputElement())
                .Run();
        }
    }
}

void PEResourceDataEntryDumper::DoDump()
{
    DumpBasicInformation(_resourceDataEntry);

    BEGIN_DUMP_PE_ELEMENT(_resourceDataEntry)
        DUMP_RAW_FIELD_SIMPLE(OffsetToData)
        DUMP_RAW_FIELD_SIMPLE(Size)
        DUMP_RAW_FIELD_SIMPLE(CodePage)
        DUMP_RAW_FIELD_SIMPLE(Reserved)
    END_DUMP_PE_ELEMENT()

    LibPEPtr<IPEResource> resource;
    if (FAILED(_resourceDataEntry->GetResource(&resource))) {
        return;
    }

    PEResourceDumper()
        .SetResource(resource)
        .SetPEFile(GetPEFile())
        .SetParentElement(GetOutputElement())
        .Run();
}

void PEResourceDumper::DoDump()
{
    DumpBasicInformation(_resource);
}