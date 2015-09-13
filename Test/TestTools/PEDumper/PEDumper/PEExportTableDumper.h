#pragma once

#include "PEElementDumper.h"

class PEExportTableDumper :
    public PEElementDumper
{
public:
    PEExportTableDumper() : PEElementDumper("ExportTable") {}

    PEExportTableDumper & SetDumpElement(_In_ IPEExportTable *exportTable) {
        _exportTable = exportTable;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEExportTable> _exportTable;
};

class PEExportFunctionDumper :
    public PEElementDumper
{
public:
    PEExportFunctionDumper() : PEElementDumper("ExportFunction") {}

    PEExportFunctionDumper & SetDumpElement(_In_ IPEExportFunction *exportFunction) {
        _exportFunction = exportFunction;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEExportFunction> _exportFunction;
};

