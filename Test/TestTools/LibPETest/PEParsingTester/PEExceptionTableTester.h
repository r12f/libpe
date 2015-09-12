#pragma once

#include "PEElementTester.h"

class PEExceptionTableTester :
    public PEElementTester
{
public:
    PEExceptionTableTester() : PEElementTester("ExceptionTable") {}

protected:
    void DoTest() override;
};

class PEExceptionHandlerEntryTester :
    public PEElementTester
{
public:
    PEExceptionHandlerEntryTester() : PEElementTester("ExceptionHandlerEntry") {}

    PEExceptionHandlerEntryTester & SetExceptionHandlerEntry(IPEExceptionHandlerEntry *exceptionHandlerEntry) {
        _exceptionHandlerEntry = exceptionHandlerEntry;
        return *this;
    }

protected:
    void DoTest() override;

private:
    LibPEPtr<IPEExceptionHandlerEntry> _exceptionHandlerEntry;
};

class PEExceptionHandlerTester :
    public PEElementTester
{
public:
    PEExceptionHandlerTester() : PEElementTester("ExceptionHandler") {}

    PEExceptionHandlerTester & SetExceptionHandler(IPEExceptionHandler *exceptionHandler) {
        _exceptionHandler = exceptionHandler;
        return *this;
    }

protected:
    void DoTest() override;

private:
    LibPEPtr<IPEExceptionHandler> _exceptionHandler;
};
