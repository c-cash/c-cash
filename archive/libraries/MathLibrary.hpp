#pragma once

#include "Library.hpp"
#include "../variables/Double.hpp"

#include <iostream>
#include <cmath>

namespace library {
    using namespace variable;

    class MathLibrary : public Library {
        public:
            void linkGlobally(Scope &s);
            void linkNamespaced(string name, Scope &scope);

            static Library* getInstance();
    };
}