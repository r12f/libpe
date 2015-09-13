#include "stdafx.h"
#include "PETlsTableTester.h"

void PETlsTableTester::DoTest()
{
    LibPEPtr<IPETlsTable> tlsTable;
    GetPEFile()->GetTlsTable(&tlsTable);
    ASSERT_EQ(tlsTable != nullptr, GetExpectedResult() != nullptr);

    if (tlsTable == nullptr) {
        return;
    }

    RunTestLoop([&, this]() {
        TestBasicInformation(tlsTable);
        TestTlsTable(tlsTable);
    });
}

void PETlsTableTester::TestTlsTable(_In_ IPETlsTable *tlsTable)
{
    BEGIN_TEST_PE_ELEMENT(tlsTable)
        TEST_RAW_FIELD_SIMPLE(IPETlsTable, StartAddressOfRawData)
        TEST_RAW_FIELD_SIMPLE(IPETlsTable, EndAddressOfRawData)
        TEST_RAW_FIELD_SIMPLE(IPETlsTable, AddressOfIndex)
        TEST_RAW_FIELD_SIMPLE(IPETlsTable, AddressOfCallBacks)
        TEST_RAW_FIELD_SIMPLE(IPETlsTable, SizeOfZeroFill)
        TEST_RAW_FIELD_SIMPLE(IPETlsTable, Characteristics)
        TEST_RAW_FIELD_SIMPLE(IPETlsTable, Reserved0)
        TEST_RAW_FIELD_SIMPLE(IPETlsTable, Alignment)
        TEST_RAW_FIELD_SIMPLE(IPETlsTable, Reserved1)
        TEST_API_RESULT_SIMPLE(IPETlsTable, GetCallbackCount)
    END_TEST_PE_ELEMENT()
}
