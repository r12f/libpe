#include "stdafx.h"
#include "PEArchitectureDataTableDumper.h"

void PEArchitectureDataTableDumper::DoDump()
{
    LibPEPtr<IPEArchitectureDataTable> architectureDataTable;
    if (FAILED(GetPEFile()->GetArchitectureDataTable(&architectureDataTable))) {
        return;
    }

    // According to PE and COFF specification 8.3, the architecture data table is reserved and must be 0,
    // so here we do nothing to dump it.
    // But still we need to dump a empty node into result files for triggering parsing test in LibPETest.
}
