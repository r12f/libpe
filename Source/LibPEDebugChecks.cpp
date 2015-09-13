#include "stdafx.h"
#include "LibPEDebugChecks.h"

LIBPE_NAMESPACE_BEGIN

void DebugCrash(LPCSTR fileName, int lineNumber, LPCSTR functionName, LPCSTR format, ...)
{
#ifdef DEBUG
    va_list argList;
    va_start(argList, format);

    if (_VCrtDbgReportA(_CRT_ASSERT, nullptr, fileName, lineNumber, functionName, format, argList) == 1) {
        __debugbreak();
    }

    va_end(argList);
#else
    UNREFERENCED_PARAMETER(format);
    UNREFERENCED_PARAMETER(fileName);
    UNREFERENCED_PARAMETER(lineNumber);
    UNREFERENCED_PARAMETER(functionName);
    __debugbreak();
#endif
}

LIBPE_NAMESPACE_END
