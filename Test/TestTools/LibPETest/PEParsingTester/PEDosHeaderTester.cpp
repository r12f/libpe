#include "stdafx.h"
#include "PEDosHeaderTester.h"

void PEDosHeaderTester::DoTest()
{
    // DosHeader is a mandatory structure for PE files.
    LibPEPtr<IPEDosHeader> dosHeader;
    ASSERT_HRESULT_SUCCEEDED(GetPEFile()->GetDosHeader(&dosHeader));
    ASSERT_NE(dosHeader, nullptr);

    RunTestLoop([&, this]() {
        TestBasicInformation(dosHeader);
        TestDosHeader(dosHeader);
    });
}

void PEDosHeaderTester::TestDosHeader(_In_ IPEDosHeader *dosHeader)
{
    BEGIN_TEST_PE_ELEMENT(dosHeader)
        TEST_RAW_FIELD_SIMPLE(IPEDosHeader, Magic)
        TEST_RAW_FIELD_SIMPLE(IPEDosHeader, Cblp)
        TEST_RAW_FIELD_SIMPLE(IPEDosHeader, Cp)
        TEST_RAW_FIELD_SIMPLE(IPEDosHeader, Crlc)
        TEST_RAW_FIELD_SIMPLE(IPEDosHeader, Cparhdr)
        TEST_RAW_FIELD_SIMPLE(IPEDosHeader, Minalloc)
        TEST_RAW_FIELD_SIMPLE(IPEDosHeader, Maxalloc)
        TEST_RAW_FIELD_SIMPLE(IPEDosHeader, Ss)
        TEST_RAW_FIELD_SIMPLE(IPEDosHeader, Sp)
        TEST_RAW_FIELD_SIMPLE(IPEDosHeader, Csum)
        TEST_RAW_FIELD_SIMPLE(IPEDosHeader, Ip)
        TEST_RAW_FIELD_SIMPLE(IPEDosHeader, Cs)
        TEST_RAW_FIELD_SIMPLE(IPEDosHeader, Lfarlc)
        TEST_RAW_FIELD_SIMPLE(IPEDosHeader, Ovno)
        TEST_RAW_FIELD_ARRAY(IPEDosHeader, Res)
        TEST_RAW_FIELD_SIMPLE(IPEDosHeader, Oemid)
        TEST_RAW_FIELD_SIMPLE(IPEDosHeader, Oeminfo)
        TEST_RAW_FIELD_ARRAY(IPEDosHeader, Res2)
        TEST_RAW_FIELD_SIMPLE(IPEDosHeader, Lfanew)
    END_TEST_PE_ELEMENT()
}
