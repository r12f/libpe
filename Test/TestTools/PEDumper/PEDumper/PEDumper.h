#pragma once

class PEDumper
{
    typedef void (*PEElementDumpFunc)(_In_ IPEFile *, tinyxml2::XMLElement *);

public:
    PEDumper();

    void Dump(const file_char_t *peFile, const file_char_t *resultFile);

private:
    std::vector<PEElementDumpFunc> _dumpers;
    tinyxml2::XMLDocument _xmlDoc;
};