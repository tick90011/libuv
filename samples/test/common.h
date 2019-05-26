#pragma once
#include "uv.h"

#ifdef _WIN32
#define PIPENAME "\\\\?\\pipe\\echo.sock"
#else
#define PIPENAME "/tmp/echo.sock"
#endif
