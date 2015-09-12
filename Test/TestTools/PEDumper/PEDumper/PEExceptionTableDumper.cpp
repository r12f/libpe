#include "stdafx.h"
#include "PEExceptionTableDumper.h"

void PEExceptionTableDumper::DoDump()
{
    DumpBasicInformation(_exceptionTable);

    UINT32 exceptionHandlerEntryCount = _exceptionTable->GetExceptionHandlerCount();
    GetOutputElement()->SetAttribute("exception-handler-entry-count", exceptionHandlerEntryCount);

    for (UINT32 exceptionHandlerEntryIndex = 0; exceptionHandlerEntryIndex < exceptionHandlerEntryCount; ++exceptionHandlerEntryIndex) {
        LibPEPtr<IPEExceptionHandlerEntry> exceptionHandlerEntry;
        if (FAILED(_exceptionTable->GetExceptionHandlerEntryByIndex(exceptionHandlerEntryIndex, &exceptionHandlerEntry))) {
            continue;
        }

        PEExceptionHandlerEntryDumper()
            .SetDumpElement(exceptionHandlerEntry)
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
        .SetDumpElement(exceptionHandler)
        .SetPEFile(GetPEFile())
        .SetParentElement(GetOutputElement())
        .Run();
}

void PEExceptionHandlerDumper::DoDump()
{
    DumpBasicInformation(_exceptionHandler);
}

