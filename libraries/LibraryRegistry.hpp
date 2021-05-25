#pragma once

#include <iostream>
#include <map>
#include "MathLibrary.hpp"

typedef library::Library* (*libDef)();

namespace library {
    static map<string, libDef> libraries = {
        {"math", library::MathLibrary::getInstance}
    };
}