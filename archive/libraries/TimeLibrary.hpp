#pragma once

#include <iostream>
#include <time.h>

#include "Library.hpp"
#include "../variables/Double.hpp"

namespace library {
    using namespace variable;

    class TimeLibrary : public Library {
        public:
            void linkGlobally(Scope &s);
            void linkNamespaced(string name, Scope &scope);

            static Library* getInstance();
    };
}