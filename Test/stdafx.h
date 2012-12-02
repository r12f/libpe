#pragma once

#pragma warning(disable: 6387)
#pragma warning(disable: 6386)

#ifndef WINVER
#define WINVER 0x0601
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0603
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cassert>
#include <string>
#include <map>
#include <vector>
#include <list>

#include <windows.h>
#include <WinNT.h>

#include "LibPE.h"