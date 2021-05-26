#pragma once

#include <iostream>

#include "Library.hpp"
#include "../variables/Double.hpp"

namespace library {
    using namespace variable;

    class MathLibrary : public Library {
        public:
            void linkGlobally(Scope &s);
            void linkNamespaced(string name, Namespace &ns);

            static Library* getInstance();
    };
}