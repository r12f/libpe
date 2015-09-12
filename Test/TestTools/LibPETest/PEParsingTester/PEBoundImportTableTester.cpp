#include "stdafx.h"
#include "PEBoundImportTableTester.h"

void PEBoundImportTableTester::DoTest()
{
    LibPEPtr<IPEBoundImportTable> boundImportTable;
    GetPEFile()->GetBoundImportTable(&boundImportTable);
    ASSERT_EQ(boundImportTable != nullptr, GetExpectedResult() != nullptr);

    if (boundImportTable == nullptr) {
        return;
    }

    RunTestLoop([&, this]() {
        TestBasicInformation(boundImportTable);
    });

    UINT32 boundImportModuleCount = boundImportTable->GetBoundImportModuleCount();
    EXPECT_EQ(GetExpectedResult()->UnsignedAttribute("import-module-count"), boundImportModuleCount);

    tinyxml2::XMLElement *boundImportModuleExpectedResult = nullptr;
    for (UINT32 boundImportModuleIndex = 0; boundImportModuleIndex < boundImportModuleCount; ++boundImportModuleIndex) {
        LibPEPtr<IPEBoundImportModule> boundImportModule;
        ASSERT_HRESULT_SUCCEEDED(boundImportTable->GetBoundImportModuleByIndex(boundImportModuleIndex, &boundImportModule));
        ASSERT_NE(boundImportModule, nullptr);

        PEBoundImportModuleTester()
            .SetBoundImportModule(boundImportModule)
            .SetPEFile(GetPEFile())
            .SetExpectedResult(GetExpectedResult(), boundImportModuleExpectedResult, &boundImportModuleExpectedResult)
            .Run();

        ASSERT_NE(boundImportModuleExpectedResult, nullptr);
    }

    ASSERT_TRUE(PEBoundImportModuleTester().IsLastExpectedResult(boundImportModuleExpectedResult));
}

void PEBoundImportModuleTester::DoTest()
{
    RunTestLoop([&, this]() {
        TestBasicInformation(_boundImportModule);
        TestBoundImportModule(_boundImportModule);
    });

    UINT32 boundForwarderCount = _boundImportModule->GetBoundForwarderCount();
    EXPECT_EQ(GetExpectedResult()->UnsignedAttribute("bound-forwarder-count"), boundForwarderCount);

    tinyxml2::XMLElement *boundForwarderExpectedResult = nullptr;
    for (UINT32 boundForwarderIndex = 0; boundForwarderIndex < boundForwarderCount; ++boundForwarderIndex) {
        LibPEPtr<IPEBoundForwarder> boundForwarder;
        ASSERT_HRESULT_SUCCEEDED(_boundImportModule->GetBoundForwarderByIndex(boundForwarderIndex, &boundForwarder));
        ASSERT_NE(boundForwarder, nullptr);

        PEBoundForwarderTester()
            .SetBoundForwarder(boundForwarder)
            .SetPEFile(GetPEFile())
            .SetExpectedResult(GetExpectedResult(), boundForwarderExpectedResult, &boundForwarderExpectedResult)
            .Run();

        ASSERT_NE(boundForwarderExpectedResult, nullptr);
    }

    ASSERT_TRUE(PEBoundForwarderTester().IsLastExpectedResult(boundForwarderExpectedResult));
}

void PEBoundImportModuleTester::TestBoundImportModule(IPEBoundImportModule *boundImportModule)
{
    BEGIN_TEST_PE_ELEMENT(boundImportModule)
        TEST_RAW_FIELD_SIMPLE(IPEBoundImportModule, TimeDateStamp)
        TEST_RAW_FIELD_SIMPLE(IPEBoundImportModule, OffsetModuleName)
        TEST_RAW_FIELD_SIMPLE(IPEBoundImportModule, NumberOfModuleForwarderRefs)
    END_TEST_PE_ELEMENT()
}

void PEBoundForwarderTester::DoTest()
{
    RunTestLoop([&, this]() {
        TestBasicInformation(_boundForwarder);
        TestBoundForwarder(_boundForwarder);
    });
}

void PEBoundForwarderTester::TestBoundForwarder(IPEBoundForwarder *boundForwarder)
{
    BEGIN_TEST_PE_ELEMENT(boundForwarder)
        TEST_RAW_FIELD_SIMPLE(IPEBoundForwarder, TimeDateStamp)
        TEST_RAW_FIELD_SIMPLE(IPEBoundForwarder, OffsetModuleName)
        TEST_RAW_FIELD_SIMPLE(IPEBoundForwarder, Reserved)
    END_TEST_PE_ELEMENT()
}
