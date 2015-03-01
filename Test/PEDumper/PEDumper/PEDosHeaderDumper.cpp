#include "stdafx.h"
#include "PEDosHeaderDumper.h"

void PEDosHeaderDumper::DoDump()
{
    LibPEPtr<IPEDosHeader> dosHeader;
    if (FAILED(GetPEFile()->GetDosHeader(&dosHeader))) {
        return;
    }

    DumpBasicInformation(dosHeader);

    BEGIN_DUMP_PE_ELEMENT(dosHeader)
        DUMP_RAW_FIELD_SIMPLE(Magic)
        DUMP_RAW_FIELD_SIMPLE(Cblp)
        DUMP_RAW_FIELD_SIMPLE(Cp)
        DUMP_RAW_FIELD_SIMPLE(Crlc)
        DUMP_RAW_FIELD_SIMPLE(Cparhdr)
        DUMP_RAW_FIELD_SIMPLE(Minalloc)
        DUMP_RAW_FIELD_SIMPLE(Maxalloc)
        DUMP_RAW_FIELD_SIMPLE(Ss)
        DUMP_RAW_FIELD_SIMPLE(Sp)
        DUMP_RAW_FIELD_SIMPLE(Csum)
        DUMP_RAW_FIELD_SIMPLE(Ip)
        DUMP_RAW_FIELD_SIMPLE(Cs)
        DUMP_RAW_FIELD_SIMPLE(Lfarlc)
        DUMP_RAW_FIELD_SIMPLE(Ovno)
        DUMP_RAW_FIELD_ARRAY(Res)
        DUMP_RAW_FIELD_SIMPLE(Oemid)
        DUMP_RAW_FIELD_SIMPLE(Oeminfo)
        DUMP_RAW_FIELD_ARRAY(Res2)
        DUMP_RAW_FIELD_SIMPLE(Lfanew)
    END_DUMP_PE_ELEMENT()
}
