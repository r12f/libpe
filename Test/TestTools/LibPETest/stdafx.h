#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <vector>
#include <string>
#include <fstream>
#include <functional>

#include "LibPE.h"
using namespace LibPE;

#ifdef LIBPE_WINOS
#include <Shlwapi.h>
#endif

#include "gtest/gtest.h"
#include "tinyxml2.h"

#pragma warning(disable:4127)


