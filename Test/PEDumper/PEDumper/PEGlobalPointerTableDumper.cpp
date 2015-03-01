#include "stdafx.h"
#include "PEGlobalPointerTableDumper.h"

void PEGlobalPointerTableDumper::DoDump()
{
    LibPEPtr<IPEGlobalPointerTable> globalPointerTable;
    if (FAILED(GetPEFile()->GetGlobalPointerTable(&globalPointerTable))) {
        return;
    }

    DumpBasicInformation(globalPointerTable);

    BEGIN_DUMP_PE_ELEMENT(globalPointerTable)
        DUMP_RAW_FIELD_SIMPLE(GlobalPointerRVA)
    END_DUMP_PE_ELEMENT()
}
