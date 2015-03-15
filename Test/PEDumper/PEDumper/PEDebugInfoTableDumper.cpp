#include "stdafx.h"
#include "PEDebugInfoTableDumper.h"

void PEDebugInfoTableDumper::DoDump()
{
    DumpBasicInformation(_debugInfoTable);

    BEGIN_DUMP_PE_ELEMENT(_debugInfoTable)
        DUMP_RAW_FIELD_SIMPLE(Characteristics)
        DUMP_RAW_FIELD_SIMPLE(TimeDateStamp)
        DUMP_RAW_FIELD_SIMPLE(MajorVersion)
        DUMP_RAW_FIELD_SIMPLE(MinorVersion)
        DUMP_RAW_FIELD_SIMPLE(Type)
        DUMP_RAW_FIELD_SIMPLE(SizeOfData)
        DUMP_RAW_FIELD_SIMPLE(AddressOfRawData)
        DUMP_RAW_FIELD_SIMPLE(PointerToRawData)
    END_DUMP_PE_ELEMENT()
}
