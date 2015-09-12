#pragma once

#include "PEElementTester.h"

class PEExportTableTester :
    public PEElementTester
{
public:
    PEExportTableTester() : PEElementTester("ExportTable") {}

protected:
    void DoTest() override;
    void TestExportTable(IPEExportTable *exportTable);
};

class PEExportFunctionTester :
    public PEElementTester
{
public:
    PEExportFunctionTester() : PEElementTester("ExportFunction") {}

    PEExportFunctionTester & SetExportFunction(IPEExportFunction *exportFunction) {
        _exportFunction = exportFunction;
        return *this;
    }

protected:
    void DoTest() override;
    void TestExportFunction(IPEExportFunction *exportFunction);

private:
    LibPEPtr<IPEExportFunction> _exportFunction;
};
