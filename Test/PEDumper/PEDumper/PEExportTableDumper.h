#pragma once

#include "PEElementDumper.h"

class PEExportTableDumper :
    public PEElementDumper
{
public:
    PEExportTableDumper() : PEElementDumper("ExportTable") {}

protected:
    void DoDump() override;
};

class PEExportFunctionDumper :
    public PEElementDumper
{
public:
    PEExportFunctionDumper() : PEElementDumper("ExportFunction") {}

    PEExportFunctionDumper & SetExportFunction(LibPEPtr<IPEExportFunction> exportFunction) {
        _exportFunction = exportFunction;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEExportFunction> _exportFunction;
};

