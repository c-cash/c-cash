#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>

#include "Library.hpp"
#include "../variables/Double.hpp"

namespace library {
    using namespace variable;

    class FileLibrary : public Library {
        public:
            void linkGlobally(Scope &s);
            void linkNamespaced(string name, Scope &scope);

            static Library* getInstance();
    };
}