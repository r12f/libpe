#include "stdafx.h"
#include "PEParsingTester.h"
#include "PEDosHeaderTester.h"
#include "PENtHeadersTester.h"
#include "PESectionHeaderTester.h"
#include "PESectionTester.h"
#include "PEExportTableTester.h"
#include "PEImportTableTester.h"
#include "PEResourceTableTester.h"
#include "PEExceptionTableTester.h"
#include "PECertificateTableTester.h"
#include "PERelocationTableTester.h"
#include "PEDebugInfoTableTester.h"
#include "PEArchitectureDataTableTester.h"
#include "PETlsTableTester.h"
#include "PELoadConfigTableTester.h"
#include "PEBoundImportTableTester.h"
#include "PEImportAddressTableTester.h"

PEParsingTester::PEParsingTester()
{
    _testers.push_back(new PEDosHeaderTester());
    _testers.push_back(new PENtHeadersTester());
    _testers.push_back(new PESectionHeaderListTester());
    _testers.push_back(new PESectionListTester());
    _testers.push_back(new PEExportTableTester());
    _testers.push_back(new PEImportTableTester());
    _testers.push_back(new PEResourceTableTester());
    _testers.push_back(new PEExceptionTableTester());
    _testers.push_back(new PECertificateTableTester());
    _testers.push_back(new PERelocationTableTester());
    _testers.push_back(new PEDebugInfoTableTester());
    _testers.push_back(new PEArchitectureDataTableTester());
    _testers.push_back(new PETlsTableTester());
    _testers.push_back(new PELoadConfigTableTester());
    _testers.push_back(new PEBoundImportTableTester());
    _testers.push_back(new PEImportAddressTableTester());
}

PEParsingTester::~PEParsingTester()
{
}

void PEParsingTester::RunTest(const file_char_t *peFilePath, const file_char_t *expectedResultFilePath)
{
    ASSERT_NE(peFilePath, nullptr);
    ASSERT_NE(expectedResultFilePath, nullptr);

    // Load PE file
    ASSERT_HRESULT_SUCCEEDED(ParsePEFromDiskFile(peFilePath, &_peFile));
    ASSERT_NE(_peFile, nullptr);

    // Load result file
    FILE *expectedResultFile = nullptr;
    ASSERT_EQ(_tfopen_s(&expectedResultFile, expectedResultFilePath, LIBPE_T("rb")), 0);

    tinyxml2::XMLError xmlLoadError = _expectedResult.LoadFile(expectedResultFile);
    fclose(expectedResultFile);

    ASSERT_EQ(xmlLoadError, tinyxml2::XML_SUCCESS);
    ASSERT_NE(_expectedResult.FirstChildElement(), nullptr);

    // Get root element which contains all the expected parsing result.
    auto rootElement = _expectedResult.FirstChildElement("PE");
    ASSERT_NE(rootElement, nullptr);

    // Run Tests
    for (auto testerIt = _testers.begin(); testerIt != _testers.end(); ++testerIt) {
        (*testerIt)->SetPEFile(_peFile).SetExpectedResult(rootElement).Run();
    }

    return;
}
