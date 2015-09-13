#include "stdafx.h"
#include "PEResourceTableTester.h"

void PEResourceTableTester::DoTest()
{
    LibPEPtr<IPEResourceTable> resourceTable;
    GetPEFile()->GetResourceTable(&resourceTable);
    ASSERT_EQ(resourceTable != nullptr, GetExpectedResult() != nullptr);

    if (resourceTable == nullptr) {
        return;
    }

    RunTestLoop([&, this]() {
        TestBasicInformation(resourceTable);
    });

    LibPEPtr<IPEResourceDirectory> rootResourceDirectory;
    ASSERT_HRESULT_SUCCEEDED(resourceTable->GetRootDirectory(&rootResourceDirectory));
    ASSERT_NE(rootResourceDirectory, nullptr);

    PEResourceDirectoryTester()
        .SetResourceDirectory(rootResourceDirectory)
        .SetPEFile(GetPEFile())
        .SetExpectedResult(GetExpectedResult())
        .Run();
}

void PEResourceDirectoryTester::DoTest()
{
    RunTestLoop([&, this]() {
        TestBasicInformation(_resourceDirectory);
        TestResourceDirectory(_resourceDirectory);
    });

    UINT32 resourceDirectoryEntryCount = _resourceDirectory->GetEntryCount();
    EXPECT_EQ(GetExpectedResult()->UnsignedAttribute("resource-directory-entry-count"), resourceDirectoryEntryCount);

    tinyxml2::XMLElement *resourceDirectoryEntryExpectedResult = nullptr;
    for (UINT32 resourceDirectoryEntryIndex = 0; resourceDirectoryEntryIndex < resourceDirectoryEntryCount; ++resourceDirectoryEntryIndex) {
        LibPEPtr<IPEResourceDirectoryEntry> resourceDirectoryEntry;
        ASSERT_HRESULT_SUCCEEDED(_resourceDirectory->GetEntryByIndex(resourceDirectoryEntryIndex, &resourceDirectoryEntry));
        ASSERT_NE(resourceDirectoryEntry, nullptr);

        PEResourceDirectoryEntryTester()
            .SetResourceDirectoryEntry(resourceDirectoryEntry)
            .SetPEFile(GetPEFile())
            .SetExpectedResult(GetExpectedResult(), resourceDirectoryEntryExpectedResult, &resourceDirectoryEntryExpectedResult)
            .Run();

        ASSERT_NE(resourceDirectoryEntryExpectedResult, nullptr);
    }

    ASSERT_TRUE(PEResourceDirectoryEntryTester().IsLastExpectedResult(resourceDirectoryEntryExpectedResult));
}

void PEResourceDirectoryTester::TestResourceDirectory(_In_ IPEResourceDirectory *resourceDirectory)
{
    BEGIN_TEST_PE_ELEMENT(resourceDirectory)
        TEST_RAW_FIELD_SIMPLE(IPEResourceDirectory, Characteristics)
        TEST_RAW_FIELD_SIMPLE(IPEResourceDirectory, TimeDateStamp)
        TEST_RAW_FIELD_SIMPLE(IPEResourceDirectory, MajorVersion)
        TEST_RAW_FIELD_SIMPLE(IPEResourceDirectory, MinorVersion)
        TEST_RAW_FIELD_SIMPLE(IPEResourceDirectory, NumberOfNamedEntries)
        TEST_RAW_FIELD_SIMPLE(IPEResourceDirectory, NumberOfIdEntries)
    END_TEST_PE_ELEMENT()
}

void PEResourceDirectoryEntryTester::DoTest()
{
    RunTestLoop([&, this]() {
        TestBasicInformation(_resourceDirectoryEntry);
        TestResourceDirectoryEntry(_resourceDirectoryEntry);
    });

    if (_resourceDirectoryEntry->IsEntryDirectory()) {
        LibPEPtr<IPEResourceDirectory> resourceDirectory;
        ASSERT_HRESULT_SUCCEEDED(_resourceDirectoryEntry->GetDirectory(&resourceDirectory));
        ASSERT_NE(resourceDirectory, nullptr);

        PEResourceDirectoryTester()
            .SetResourceDirectory(resourceDirectory)
            .SetPEFile(GetPEFile())
            .SetExpectedResult(GetExpectedResult())
            .Run();
    }
    else if (_resourceDirectoryEntry->IsEntryDataEntry()) {
        LibPEPtr<IPEResourceDataEntry> resourceDataEntry;
        ASSERT_HRESULT_SUCCEEDED(_resourceDirectoryEntry->GetDataEntry(&resourceDataEntry));
        ASSERT_NE(resourceDataEntry, nullptr);

        PEResourceDataEntryTester()
            .SetResourceDataEntry(resourceDataEntry)
            .SetPEFile(GetPEFile())
            .SetExpectedResult(GetExpectedResult())
            .Run();
    }
}

void PEResourceDirectoryEntryTester::TestResourceDirectoryEntry(_In_ IPEResourceDirectoryEntry *resourceDirectoryEntry)
{
    BEGIN_TEST_PE_ELEMENT(resourceDirectoryEntry)
        TEST_RAW_FIELD_SIMPLE(IPEResourceDirectoryEntry, NameOffset)
        TEST_RAW_FIELD_SIMPLE(IPEResourceDirectoryEntry, NameIsString)
        TEST_RAW_FIELD_SIMPLE(IPEResourceDirectoryEntry, Name)
        TEST_RAW_FIELD_SIMPLE(IPEResourceDirectoryEntry, Id)
        TEST_RAW_FIELD_SIMPLE(IPEResourceDirectoryEntry, OffsetToDirectory)
        TEST_RAW_FIELD_SIMPLE(IPEResourceDirectoryEntry, OffsetToDirectory)
        TEST_RAW_FIELD_SIMPLE(IPEResourceDirectoryEntry, DataIsDirectory)
        TEST_API_RESULT_SIMPLE(IPEResourceDirectoryEntry, IsNameId)
        TEST_API_RESULT_SIMPLE(IPEResourceDirectoryEntry, GetId)
        TEST_API_RESULT_SIMPLE(IPEResourceDirectoryEntry, IsNameString)
        // TEST_API_RESULT_STRING(IPEResourceDirectoryEntry, GetName)
        TEST_API_RESULT_SIMPLE(IPEResourceDirectoryEntry, IsEntryDirectory)
        TEST_API_RESULT_SIMPLE(IPEResourceDirectoryEntry, IsEntryDataEntry)
    END_TEST_PE_ELEMENT()
}

void PEResourceDataEntryTester::DoTest()
{
    RunTestLoop([&, this]() {
        TestBasicInformation(_resourceDataEntry);
        TestResourceDataEntry(_resourceDataEntry);
    });
}

void PEResourceDataEntryTester::TestResourceDataEntry(_In_ IPEResourceDataEntry *resourceDataEntry)
{
    BEGIN_TEST_PE_ELEMENT(resourceDataEntry)
        TEST_RAW_FIELD_SIMPLE(IPEResourceDataEntry, OffsetToData)
        TEST_RAW_FIELD_SIMPLE(IPEResourceDataEntry, Size)
        TEST_RAW_FIELD_SIMPLE(IPEResourceDataEntry, CodePage)
        TEST_RAW_FIELD_SIMPLE(IPEResourceDataEntry, Reserved)
    END_TEST_PE_ELEMENT()
}

void PEResourceTester::DoTest()
{
    RunTestLoop([&, this]() {
        TestBasicInformation(_resource);
    });
}
