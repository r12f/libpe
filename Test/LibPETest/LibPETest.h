#pragma once

class LibPETest : public testing::Test
{
protected:
    // Set up code for each test
    virtual void SetUp();

    // Clear code for each test
    virtual void TearDown();

    file_t _testDataDir;
};
