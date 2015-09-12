#include "stdafx.h"
#include "PEExportTableTester.h"

void PEExportTableTester::DoTest()
{
    LibPEPtr<IPEExportTable> exportTable;
    GetPEFile()->GetExportTable(&exportTable);
    ASSERT_EQ(exportTable != nullptr, GetExpectedResult() != nullptr);

    if (exportTable == nullptr) {
        return;
    }

    RunTestLoop([&, this]() {
        TestBasicInformation(exportTable);
        TestExportTable(exportTable);
    });

    UINT32 exportFunctionCount = exportTable->GetFunctionCount();
    EXPECT_EQ(GetExpectedResult()->UnsignedAttribute("export-function-count"), exportFunctionCount);

    tinyxml2::XMLElement *exportFunctionExpectedResult = nullptr;
    for (UINT32 exportFunctionIndex = 0; exportFunctionIndex < exportFunctionCount; ++exportFunctionIndex) {
        LibPEPtr<IPEExportFunction> exportFunction;
        ASSERT_HRESULT_SUCCEEDED(exportTable->GetFunctionByIndex(exportFunctionIndex, &exportFunction));
        ASSERT_NE(exportFunction, nullptr);

        PEExportFunctionTester()
            .SetExportFunction(exportFunction)
            .SetPEFile(GetPEFile())
            .SetExpectedResult(GetExpectedResult(), exportFunctionExpectedResult, &exportFunctionExpectedResult)
            .Run();

        ASSERT_NE(exportFunctionExpectedResult, nullptr);
    }

    ASSERT_TRUE(PEExportFunctionTester().IsLastExpectedResult(exportFunctionExpectedResult));
}

void PEExportTableTester::TestExportTable(IPEExportTable *exportTable)
{
    BEGIN_TEST_PE_ELEMENT(exportTable)
        TEST_RAW_FIELD_SIMPLE(IPEExportTable, Characteristics)
        TEST_RAW_FIELD_SIMPLE(IPEExportTable, TimeDateStamp)
        TEST_RAW_FIELD_SIMPLE(IPEExportTable, MajorVersion)
        TEST_RAW_FIELD_SIMPLE(IPEExportTable, MinorVersion)
        TEST_RAW_FIELD_SIMPLE(IPEExportTable, Name)
        TEST_RAW_FIELD_SIMPLE(IPEExportTable, Base)
        TEST_RAW_FIELD_SIMPLE(IPEExportTable, NumberOfFunctions)
        TEST_RAW_FIELD_SIMPLE(IPEExportTable, NumberOfNames)
        TEST_RAW_FIELD_SIMPLE(IPEExportTable, AddressOfFunctions)
        TEST_RAW_FIELD_SIMPLE(IPEExportTable, AddressOfNames)
        TEST_RAW_FIELD_SIMPLE(IPEExportTable, AddressOfNameOrdinals)
    END_TEST_PE_ELEMENT()
}

void PEExportFunctionTester::DoTest()
{
    RunTestLoop([&, this]() {
        TestBasicInformation(_exportFunction);
        TestExportFunction(_exportFunction);
    });
}

void PEExportFunctionTester::TestExportFunction(IPEExportFunction *exportFunction)
{
    BEGIN_TEST_PE_ELEMENT(exportFunction)
        TEST_API_RESULT_STRING(IPEExportFunction, GetName)
        TEST_API_RESULT_SIMPLE(IPEExportFunction, GetOrdinal)
    END_TEST_PE_ELEMENT()
}
