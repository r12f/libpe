#pragma once

#include "PEElementDumper.h"

class PEBoundImportTableDumper :
    public PEElementDumper
{
public:
    PEBoundImportTableDumper() : PEElementDumper("BoundImportTable") {}

protected:
    void DoDump() override;
};

class PEBoundImportModuleDumper :
    public PEElementDumper
{
public:
    PEBoundImportModuleDumper() : PEElementDumper("BoundImportModule") {}

    PEBoundImportModuleDumper & SetBoundImportModule(IPEBoundImportModule *boundImportModule) {
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

    PEBoundForwarderDumper & SetBoundForwarder(IPEBoundForwarder *boundForwarder) {
        _boundForwarder = boundForwarder; 
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEBoundForwarder> _boundForwarder;
};
