#include "stdafx.h"
#include "PEGlobalPointerTableDumper.h"

void PEGlobalPointerTableDumper::DoDump()
{
    DumpBasicInformation(_globalPointerTable);

    BEGIN_DUMP_PE_ELEMENT(_globalPointerTable)
        DUMP_RAW_FIELD_SIMPLE(GlobalPointerRVA)
    END_DUMP_PE_ELEMENT()
}
