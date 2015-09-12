#include "stdafx.h"
#include "PEDumper/PEDumper.h"

#define TEST_DLL_NAME               LIBPE_T("Test.dll")
#define FILE_PARSING_RESULT_NAME    LIBPE_T("References.xml")

file_t GetTestDataDir();

struct TestCase {
    const file_char_t *testCaseName;
    const file_char_t *testFileName;
};

int wmain(int /*argc*/, wchar_t* /*argv*/[])
{
    static const TestCase testCases[] = {
        { LIBPE_T("RegularDllX86"), LIBPE_T("Test.dll") },
        { LIBPE_T("RegularExeX86"), LIBPE_T("Test.exe") },
        { LIBPE_T("RegularDllX64"), LIBPE_T("Test.dll") },
        { LIBPE_T("RegularExeX64"), LIBPE_T("Test.exe") },
    };

    static const UINT32 testCaseCount = sizeof(testCases) / sizeof(testCases[0]);
    
    for (UINT32 testCaseIndex = 0; testCaseIndex < testCaseCount; ++testCaseIndex) {
        file_t testCaseDir = GetTestDataDir() + LIBPE_T("/") + testCases[testCaseIndex].testCaseName;
        file_t testFilePath = testCaseDir + LIBPE_T("/") + testCases[testCaseIndex].testFileName;
        file_t fileParsingResultFilePath = testCaseDir + LIBPE_T("/") + FILE_PARSING_RESULT_NAME;

        PEDumper dumper;
        dumper.Dump(testFilePath.c_str(), fileParsingResultFilePath.c_str());
    }

    return 0;
}

#ifdef LIBPE_WINOS
#include <Shlwapi.h>

file_t GetTestDataDir()
{
    file_char_t exePath[MAX_PATH] = { 0 };
    ::GetModuleFileNameW(nullptr, exePath, MAX_PATH);
    ::PathRemoveFileSpecW(exePath);
    ::PathAppendW(exePath, L"..\\..\\..\\..\\Test\\TestCases");
    return exePath;
}
#endif