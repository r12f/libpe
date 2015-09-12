#pragma once

#include "PEElementTester.h"

class PEBoundImportTableTester :
    public PEElementTester
{
public:
    PEBoundImportTableTester() : PEElementTester("BoundImportTable") {}

protected:
    void DoTest() override;
};

class PEBoundImportModuleTester :
    public PEElementTester
{
public:
    PEBoundImportModuleTester() : PEElementTester("BoundImportModule") {}

    PEBoundImportModuleTester & SetBoundImportModule(IPEBoundImportModule *boundImportModule) {
        _boundImportModule = boundImportModule;
        return *this;
    }

protected:
    void DoTest() override;
    void TestBoundImportModule(IPEBoundImportModule *boundImportModule);

private:
    LibPEPtr<IPEBoundImportModule> _boundImportModule;
};

class PEBoundForwarderTester :
    public PEElementTester
{
public:
    PEBoundForwarderTester() : PEElementTester("BoundForwarder") {}

    PEBoundForwarderTester & SetBoundForwarder(IPEBoundForwarder *boundForwarder) {
        _boundForwarder = boundForwarder;
        return *this;
    }

protected:
    void DoTest() override;
    void TestBoundForwarder(IPEBoundForwarder *boundForwarder);

private:
    LibPEPtr<IPEBoundForwarder> _boundForwarder;
};
