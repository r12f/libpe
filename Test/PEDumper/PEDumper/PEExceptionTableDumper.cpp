#include "stdafx.h"
#include "PEExceptionTableDumper.h"

void PEExceptionTableDumper::DoDump()
{
    LibPEPtr<IPEExceptionTable> exceptionTable;
    if (FAILED(GetPEFile()->GetExceptionTable(&exceptionTable))) {
        return;
    }

    DumpBasicInformation(exceptionTable);

    UINT32 exceptionHandlerEntryCount = exceptionTable->GetExceptionHandlerCount();
    GetOutputElement()->SetAttribute("exception-handler-entry-count", exceptionHandlerEntryCount);

    for (UINT32 exceptionHandlerEntryIndex = 0; exceptionHandlerEntryIndex < exceptionHandlerEntryCount; ++exceptionHandlerEntryIndex) {
        LibPEPtr<IPEExceptionHandlerEntry> exceptionHandlerEntry;
        if (FAILED(exceptionTable->GetExceptionHandlerEntryByIndex(exceptionHandlerEntryIndex, &exceptionHandlerEntry))) {
            continue;
        }

        PEExceptionHandlerEntryDumper()
            .SetExceptionHandlerEntry(exceptionHandlerEntry)
            .SetPEFile(GetPEFile())
            .SetParentElement(GetOutputElement())
            .Run();
    }
}

void PEExceptionHandlerEntryDumper::DoDump()
{
    DumpBasicInformation(_exceptionHandlerEntry);

    LibPEPtr<IPEExceptionHandler> exceptionHandler;
    if (FAILED(_exceptionHandlerEntry->GetExceptionHandler(&exceptionHandler))) {
        return;
    }

    PEExceptionHandlerDumper()
        .SetExceptionHandler(exceptionHandler)
        .SetPEFile(GetPEFile())
        .SetParentElement(GetOutputElement())
        .Run();
}

void PEExceptionHandlerDumper::DoDump()
{
    DumpBasicInformation(_exceptionHandler);
}

