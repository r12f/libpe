#include "stdafx.h"
#include "PEExceptionTableTester.h"

void PEExceptionTableTester::DoTest()
{
    LibPEPtr<IPEExceptionTable> exceptionTable;
    GetPEFile()->GetExceptionTable(&exceptionTable);
    ASSERT_EQ(exceptionTable != nullptr, GetExpectedResult() != nullptr);

    if (exceptionTable == nullptr) {
        return;
    }

    RunTestLoop([&, this]() {
        TestBasicInformation(exceptionTable);
    });

    UINT32 exceptionHandlerEntryCount = exceptionTable->GetExceptionHandlerCount();
    EXPECT_EQ(GetExpectedResult()->UnsignedAttribute("exception-handler-entry-count"), exceptionHandlerEntryCount);

    tinyxml2::XMLElement *exceptionHandlerEntryExpectedResult = nullptr;
    for (UINT32 exceptionHandlerEntryIndex = 0; exceptionHandlerEntryIndex < exceptionHandlerEntryCount; ++exceptionHandlerEntryIndex) {
        LibPEPtr<IPEExceptionHandlerEntry> exceptionHandlerEntry;
        ASSERT_HRESULT_SUCCEEDED(exceptionTable->GetExceptionHandlerEntryByIndex(exceptionHandlerEntryIndex, &exceptionHandlerEntry));
        ASSERT_NE(exceptionHandlerEntry, nullptr);

        PEExceptionHandlerEntryTester()
            .SetExceptionHandlerEntry(exceptionHandlerEntry)
            .SetPEFile(GetPEFile())
            .SetExpectedResult(GetExpectedResult(), exceptionHandlerEntryExpectedResult, &exceptionHandlerEntryExpectedResult)
            .Run();

        ASSERT_NE(exceptionHandlerEntryExpectedResult, nullptr);
    }

    ASSERT_TRUE(PEExceptionHandlerEntryTester().IsLastExpectedResult(exceptionHandlerEntryExpectedResult));
}

void PEExceptionHandlerEntryTester::DoTest()
{
    RunTestLoop([&, this]() {
        TestBasicInformation(_exceptionHandlerEntry);
    });

    LibPEPtr<IPEExceptionHandler> exceptionHandler;
    ASSERT_HRESULT_SUCCEEDED(_exceptionHandlerEntry->GetExceptionHandler(&exceptionHandler));
    ASSERT_NE(exceptionHandler, nullptr);

    PEExceptionHandlerTester()
        .SetExceptionHandler(exceptionHandler)
        .SetPEFile(GetPEFile())
        .SetExpectedResult(GetExpectedResult())
        .Run();
}

void PEExceptionHandlerTester::DoTest()
{
    RunTestLoop([&, this]() {
        TestBasicInformation(_exceptionHandler);
    });
}
