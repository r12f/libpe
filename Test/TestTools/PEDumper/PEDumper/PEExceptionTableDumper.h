#pragma once

#include "PEElementDumper.h"

class PEExceptionTableDumper :
    public PEElementDumper
{
public:
    PEExceptionTableDumper() : PEElementDumper("ExceptionTable") {}

    PEExceptionTableDumper & SetDumpElement(IPEExceptionTable *exceptionTable) {
        _exceptionTable = exceptionTable;
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEExceptionTable> _exceptionTable;
};

class PEExceptionHandlerEntryDumper :
    public PEElementDumper
{
public:
    PEExceptionHandlerEntryDumper() : PEElementDumper("ExceptionHandlerEntry") {}

    PEExceptionHandlerEntryDumper & SetDumpElement(IPEExceptionHandlerEntry *exceptionHandlerEntry) {
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

    PEExceptionHandlerDumper & SetDumpElement(IPEExceptionHandler *exceptionHandler) {
        _exceptionHandler = exceptionHandler; 
        return *this;
    }

protected:
    void DoDump() override;

private:
    LibPEPtr<IPEExceptionHandler> _exceptionHandler;
};
