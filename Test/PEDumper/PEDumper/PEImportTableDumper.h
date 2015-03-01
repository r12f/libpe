#pragma once

#include "PEElementDumper.h"

class PEImportTableDumper :
    public PEElementDumper
{
public:
    PEImportTableDumper() : PEElementDumper("ImportTable") {}

protected:
    void DoDump() override;
};

class PEImportModuleDumper :
    public PEElementDumper
{
public:
    PEImportModuleDumper() : PEElementDumper("ImportModule") {}

    PEImportModuleDumper & SetImportModule(IPEImportModule *importModule) {
        _importModule = importModule;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEImportModule> _importModule;
};

class PEImportFunctionDumper :
    public PEElementDumper
{
public:
    PEImportFunctionDumper() : PEElementDumper("ImportFunction") {}

    PEImportFunctionDumper & SetImportFunction(IPEImportFunction *importFunction) {
        _importFunction = importFunction; 
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEImportFunction> _importFunction;
};
