#include "stdafx.h"
#include "PELoadConfigTableDumper.h"

void PELoadConfigTableDumper::DoDump()
{
    DumpBasicInformation(_loadConfigTable);

    BEGIN_DUMP_PE_ELEMENT(_loadConfigTable)
        DUMP_RAW_FIELD_SIMPLE(Size)
        DUMP_RAW_FIELD_SIMPLE(TimeDateStamp)
        DUMP_RAW_FIELD_SIMPLE(MajorVersion)
        DUMP_RAW_FIELD_SIMPLE(MinorVersion)
        DUMP_RAW_FIELD_SIMPLE(GlobalFlagsClear)
        DUMP_RAW_FIELD_SIMPLE(GlobalFlagsSet)
        DUMP_RAW_FIELD_SIMPLE(CriticalSectionDefaultTimeout)
        DUMP_RAW_FIELD_SIMPLE(DeCommitFreeBlockThreshold)
        DUMP_RAW_FIELD_SIMPLE(DeCommitTotalFreeThreshold)
        DUMP_RAW_FIELD_SIMPLE(LockPrefixTable)
        DUMP_RAW_FIELD_SIMPLE(MaximumAllocationSize)
        DUMP_RAW_FIELD_SIMPLE(VirtualMemoryThreshold)
        DUMP_RAW_FIELD_SIMPLE(ProcessAffinityMask)
        DUMP_RAW_FIELD_SIMPLE(ProcessHeapFlags)
        DUMP_RAW_FIELD_SIMPLE(CSDVersion)
        DUMP_RAW_FIELD_SIMPLE(Reserved1)
        DUMP_RAW_FIELD_SIMPLE(EditList)
        DUMP_RAW_FIELD_SIMPLE(SecurityCookie)
        DUMP_RAW_FIELD_SIMPLE(SEHandlerTable)
        DUMP_RAW_FIELD_SIMPLE(SEHandlerCount)
        DUMP_RAW_FIELD_SIMPLE(GuardCFCheckFunctionPointer)
        DUMP_RAW_FIELD_SIMPLE(Reserved2)
        DUMP_RAW_FIELD_SIMPLE(GuardCFFunctionTable)
        DUMP_RAW_FIELD_SIMPLE(GuardCFFunctionCount)
        DUMP_RAW_FIELD_SIMPLE(GuardFlags)
    END_DUMP_PE_ELEMENT()
}
