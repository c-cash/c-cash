#pragma once

#include <iostream>
#include <map>

#include "MathLibrary.hpp"
#include "RandomLibrary.hpp"
#include "FileLibrary.hpp"

typedef library::Library* (*libDef)();

namespace library {
    static map<string, libDef> libraries = {
        {"math", library::MathLibrary::getInstance},
        {"random", library::RandomLibrary::getInstance},
        {"file", library::FileLibrary::getInstance}
    };
}