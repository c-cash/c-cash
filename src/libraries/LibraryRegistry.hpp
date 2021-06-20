#pragma once

#include <iostream>
#include <map>

#include "MathLibrary.hpp"
#include "RandomLibrary.hpp"
#include "FileLibrary.hpp"
#include "TimeLibrary.hpp"
#include "StringLibrary.hpp"
#include "RegexLibrary.hpp"
#include "AlgorithmsLibrary.hpp"

typedef library::Library* (*libDef)();

namespace library {
    static map<string, libDef> libraries = {
        {"math", library::MathLibrary::getInstance},
        {"random", library::RandomLibrary::getInstance},
        {"file", library::FileLibrary::getInstance},
        {"time", library::TimeLibrary::getInstance},
        {"string", library::StringLibrary::getInstance},
        {"regex", library::RegexLibrary::getInstance},
        {"algorithms", library::AlgorithmsLibrary::getInstance}
    };
}