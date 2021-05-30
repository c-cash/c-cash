#pragma once

#include <iostream>
#include <map>

#include "MathLibrary.hpp"
#include "RandomLibrary.hpp"

typedef library::Library* (*libDef)();

namespace library {
    static map<string, libDef> libraries = {
        {"math", library::MathLibrary::getInstance},
        {"random", library::RandomLibrary::getInstance}
    };
}