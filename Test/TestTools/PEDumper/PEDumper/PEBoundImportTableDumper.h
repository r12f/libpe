#pragma once

#include "PEElementDumper.h"

class PEBoundImportTableDumper :
    public PEElementDumper
{
public:
    PEBoundImportTableDumper() : PEElementDumper("BoundImportTable") {}

    PEBoundImportTableDumper & SetDumpElement(IPEBoundImportTable *boundImportTable) {
        _boundImportTable = boundImportTable;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEBoundImportTable> _boundImportTable;
};

class PEBoundImportModuleDumper :
    public PEElementDumper
{
public:
    PEBoundImportModuleDumper() : PEElementDumper("BoundImportModule") {}

    PEBoundImportModuleDumper & SetDumpeElement(IPEBoundImportModule *boundImportModule) {
        _boundImportModule = boundImportModule;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEBoundImportModule> _boundImportModule;
};

class PEBoundForwarderDumper :
    public PEElementDumper
{
public:
    PEBoundForwarderDumper() : PEElementDumper("BoundForwarder") {}

    PEBoundForwarderDumper & SetDumpElement(IPEBoundForwarder *boundForwarder) {
        _boundForwarder = boundForwarder; 
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEBoundForwarder> _boundForwarder;
};
