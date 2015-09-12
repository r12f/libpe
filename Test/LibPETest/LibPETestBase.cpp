#include "stdafx.h"
#include "LibPETestBase.h"
#include "PEParsingTester/PEParsingTester.h"

#ifdef LIBPE_WINOS
#include <Shlwapi.h>
#endif

void LibPETestBase::SetUp()
{
#ifdef LIBPE_WINOS
    file_char_t exePath[MAX_PATH] = { 0 };
    ::GetModuleFileNameW(nullptr, exePath, MAX_PATH);
    ::PathRemoveFileSpecW(exePath);
    ::PathAppendW(exePath, L"..\\..\\..\\..\\Test\\Data");
    _testDataDir = exePath;
#else
#endif
}

void LibPETestBase::TearDown()
{

}

TEST_F(LibPETestBase, X86DllFileTest)
{
    file_t peFilePath = _testDataDir + LIBPE_T("/x86Dll/Test.dll");
    file_t expectedResultFilePath = _testDataDir + LIBPE_T("/x86Dll/FileParsingResult.xml");

    PEParsingTester().RunTest(peFilePath.c_str(), expectedResultFilePath.c_str());
}
