#pragma once

#include "PEElementTester.h"

class PETlsTableTester :
    public PEElementTester
{
public:
    PETlsTableTester() : PEElementTester("TlsTable") {}

protected:
    void DoTest() override;
    void TestTlsTable(IPETlsTable *tlsTable);
};
