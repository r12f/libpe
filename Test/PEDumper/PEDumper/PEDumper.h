#pragma once

#include "PEElementDumper.h"

class PEDumper
{
public:
    PEDumper();
    ~PEDumper();

    void Dump(const file_char_t *peFile, const file_char_t *resultFile);

private:
    std::vector<PEElementDumper *> _dumpers;
    tinyxml2::XMLDocument _xmlDoc;
};