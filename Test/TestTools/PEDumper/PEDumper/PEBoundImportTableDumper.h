#pragma once

#include "PEElementDumper.h"

class PEBoundImportTableDumper :
    public PEElementDumper
{
public:
    PEBoundImportTableDumper() : PEElementDumper("BoundImportTable") {}

    PEBoundImportTableDumper & SetDumpElement(_In_ IPEBoundImportTable *boundImportTable) {
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

    PEBoundImportModuleDumper & SetDumpeElement(_In_ IPEBoundImportModule *boundImportModule) {
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

    PEBoundForwarderDumper & SetDumpElement(_In_ IPEBoundForwarder *boundForwarder) {
        _boundForwarder = boundForwarder; 
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEBoundForwarder> _boundForwarder;
};
