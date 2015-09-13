#include "stdafx.h"
#include "PEImportTableTester.h"

void PEImportTableTester::DoTest()
{
    LibPEPtr<IPEImportTable> importTable;
    GetPEFile()->GetImportTable(&importTable);
    ASSERT_EQ(importTable != nullptr, GetExpectedResult() != nullptr);

    if (importTable == nullptr) {
        return;
    }

    RunTestLoop([&, this]() {
        TestBasicInformation(importTable);
    });

    UINT32 importModuleCount = importTable->GetModuleCount();
    EXPECT_EQ(GetExpectedResult()->UnsignedAttribute("import-module-count"), importModuleCount);

    tinyxml2::XMLElement *importModuleExpectedResult = nullptr;
    for (UINT32 importModuleIndex = 0; importModuleIndex < importModuleCount; ++importModuleIndex) {
        LibPEPtr<IPEImportModule> importModule;
        ASSERT_HRESULT_SUCCEEDED(importTable->GetModuleByIndex(importModuleIndex, &importModule));
        ASSERT_NE(importModule, nullptr);

        PEImportModuleTester()
            .SetImportModule(importModule)
            .SetPEFile(GetPEFile())
            .SetExpectedResult(GetExpectedResult(), importModuleExpectedResult, &importModuleExpectedResult)
            .Run();

        ASSERT_NE(importModuleExpectedResult, nullptr);
    }

    ASSERT_TRUE(PEImportModuleTester().IsLastExpectedResult(importModuleExpectedResult));
}

void PEImportModuleTester::DoTest()
{
    RunTestLoop([&, this]() {
        TestBasicInformation(_importModule);
        TestImportModule(_importModule);
    });

    UINT32 importFunctionCount = _importModule->GetFunctionCount();
    EXPECT_EQ(GetExpectedResult()->UnsignedAttribute("import-function-count"), importFunctionCount);

    tinyxml2::XMLElement *importFunctionExpectedResult = nullptr;
    for (UINT32 importFunctionIndex = 0; importFunctionIndex < importFunctionCount; ++importFunctionIndex) {
        LibPEPtr<IPEImportFunction> importFunction;
        ASSERT_HRESULT_SUCCEEDED(_importModule->GetFunctionByIndex(importFunctionIndex, &importFunction));
        ASSERT_NE(importFunction, nullptr);

        PEImportFunctionTester()
            .SetImportFunction(importFunction)
            .SetPEFile(GetPEFile())
            .SetExpectedResult(GetExpectedResult(), importFunctionExpectedResult, &importFunctionExpectedResult)
            .Run();

        ASSERT_NE(importFunctionExpectedResult, nullptr);
    }

    ASSERT_TRUE(PEImportFunctionTester().IsLastExpectedResult(importFunctionExpectedResult));
}

void PEImportModuleTester::TestImportModule(_In_ IPEImportModule *importModule)
{
    BEGIN_TEST_PE_ELEMENT(importModule)
        TEST_RAW_FIELD_SIMPLE(IPEImportModule, Characteristics)
        TEST_RAW_FIELD_SIMPLE(IPEImportModule, OriginalFirstThunk)
        TEST_RAW_FIELD_SIMPLE(IPEImportModule, TimeDateStamp)
        TEST_RAW_FIELD_SIMPLE(IPEImportModule, ForwarderChain)
        TEST_RAW_FIELD_SIMPLE(IPEImportModule, Name)
        TEST_RAW_FIELD_SIMPLE(IPEImportModule, FirstThunk)
        TEST_API_RESULT_SIMPLE(IPEImportModule, IsBound)
        TEST_API_RESULT_STRING(IPEImportModule, GetName)
    END_TEST_PE_ELEMENT()
}

void PEImportFunctionTester::DoTest()
{
    RunTestLoop([&, this]() {
        TestBasicInformation(_importFunction);
        TestImportFunction(_importFunction);
    });
}

void PEImportFunctionTester::TestImportFunction(_In_ IPEImportFunction *importFunction)
{
    BEGIN_TEST_PE_ELEMENT(importFunction)
        TEST_RAW_FIELD_SIMPLE(IPEImportFunction, ForwarderString)
        TEST_RAW_FIELD_SIMPLE(IPEImportFunction, Function)
        TEST_RAW_FIELD_SIMPLE(IPEImportFunction, Ordinal)
        TEST_RAW_FIELD_SIMPLE(IPEImportFunction, AddressOfData)
    END_TEST_PE_ELEMENT()
}
