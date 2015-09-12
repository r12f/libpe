#pragma once

#include "PEElementTester.h"

class PEImportTableTester :
    public PEElementTester
{
public:
    PEImportTableTester() : PEElementTester("ImportTable") {}

protected:
    void DoTest() override;
};

class PEImportModuleTester :
    public PEElementTester
{
public:
    PEImportModuleTester() : PEElementTester("ImportModule") {}

    PEImportModuleTester & SetImportModule(IPEImportModule *importModule) {
        _importModule = importModule;
        return *this;
    }

protected:
    void DoTest() override;
    void TestImportModule(IPEImportModule *importModule);

private:
    LibPEPtr<IPEImportModule> _importModule;
};

class PEImportFunctionTester :
    public PEElementTester
{
public:
    PEImportFunctionTester() : PEElementTester("ImportFunction") {}

    PEImportFunctionTester & SetImportFunction(IPEImportFunction *importFunction) {
        _importFunction = importFunction;
        return *this;
    }

protected:
    void DoTest() override;
    void TestImportFunction(IPEImportFunction *importFunction);

private:
    LibPEPtr<IPEImportFunction> _importFunction;
};
