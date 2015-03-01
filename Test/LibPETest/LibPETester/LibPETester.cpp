#include "stdafx.h"
#include "LibPETester.h"
#include "PEDosHeaderTester.h"
#include "PENtHeadersTester.h"
#include "PESectionHeaderTester.h"

LibPETester::LibPETester()
{
    _testers.push_back(new PEDosHeaderTester());
    _testers.push_back(new PENtHeadersTester());
    _testers.push_back(new PESectionHeaderListTester());
}

LibPETester::~LibPETester()
{
}

void LibPETester::RunTest(const file_char_t *peFilePath, const file_char_t *expectedResultFilePath)
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
        (*testerIt)->Run(_peFile, rootElement);
    }

    return;
}
