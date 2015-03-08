#pragma once

#include "PEElementTester.h"

class PEParsingTester
{
public:
    PEParsingTester();
    ~PEParsingTester();
    
    void RunTest(const file_char_t *peFilePath, const file_char_t *expectedResultFilePath);

private:
    std::vector<PEElementTester *> _testers;
    LibPEPtr<IPEFile> _peFile;
    tinyxml2::XMLDocument _expectedResult;
};