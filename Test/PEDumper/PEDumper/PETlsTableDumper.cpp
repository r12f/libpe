#include "stdafx.h"
#include "PETlsTableDumper.h"

void PETlsTableDumper::DoDump()
{
    DumpBasicInformation(_tlsTable);

    BEGIN_DUMP_PE_ELEMENT(_tlsTable)
        DUMP_RAW_FIELD_SIMPLE(StartAddressOfRawData)
        DUMP_RAW_FIELD_SIMPLE(EndAddressOfRawData)
        DUMP_RAW_FIELD_SIMPLE(AddressOfIndex)
        DUMP_RAW_FIELD_SIMPLE(AddressOfCallBacks)
        DUMP_RAW_FIELD_SIMPLE(SizeOfZeroFill)
        DUMP_RAW_FIELD_SIMPLE(Characteristics)
        DUMP_RAW_FIELD_SIMPLE(Reserved0)
        DUMP_RAW_FIELD_SIMPLE(Alignment)
        DUMP_RAW_FIELD_SIMPLE(Reserved1)
        DUMP_API_RESULT_SIMPLE(GetCallbackCount)
    END_DUMP_PE_ELEMENT()
}
