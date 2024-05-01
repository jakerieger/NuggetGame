#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include <map>
#include <chrono>

#include "Types.h"

#ifdef _WIN32
    #define STRDUP _strdup
#else
    #include <cstring>
    #define STRDUP strdup
#endif