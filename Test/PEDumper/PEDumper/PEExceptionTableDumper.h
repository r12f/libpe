#pragma once

#include "PEElementDumper.h"

class PEExceptionTableDumper :
    public PEElementDumper
{
public:
    PEExceptionTableDumper() : PEElementDumper("ExceptionTable") {}

protected:
    void DoDump() override;
};

class PEExceptionHandlerEntryDumper :
    public PEElementDumper
{
public:
    PEExceptionHandlerEntryDumper() : PEElementDumper("ExceptionHandlerEntry") {}

    PEExceptionHandlerEntryDumper & SetExceptionHandlerEntry(IPEExceptionHandlerEntry *exceptionHandlerEntry) {
        _exceptionHandlerEntry = exceptionHandlerEntry;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEExceptionHandlerEntry> _exceptionHandlerEntry;
};

class PEExceptionHandlerDumper :
    public PEElementDumper
{
public:
    PEExceptionHandlerDumper() : PEElementDumper("ExceptionHandler") {}

    PEExceptionHandlerDumper & SetExceptionHandler(IPEExceptionHandler *exceptionHandler) {
        _exceptionHandler = exceptionHandler; 
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEExceptionHandler> _exceptionHandler;
};
