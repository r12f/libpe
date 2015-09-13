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

    PEBoundImportModuleTester & SetBoundImportModule(_In_ IPEBoundImportModule *boundImportModule) {
        _boundImportModule = boundImportModule;
        return *this;
    }

protected:
    void DoTest() override;
    void TestBoundImportModule(_In_ IPEBoundImportModule *boundImportModule);

private:
    LibPEPtr<IPEBoundImportModule> _boundImportModule;
};

class PEBoundForwarderTester :
    public PEElementTester
{
public:
    PEBoundForwarderTester() : PEElementTester("BoundForwarder") {}

    PEBoundForwarderTester & SetBoundForwarder(_In_ IPEBoundForwarder *boundForwarder) {
        _boundForwarder = boundForwarder;
        return *this;
    }

protected:
    void DoTest() override;
    void TestBoundForwarder(_In_ IPEBoundForwarder *boundForwarder);

private:
    LibPEPtr<IPEBoundForwarder> _boundForwarder;
};
