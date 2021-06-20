#pragma once

#include <iostream>
#include <string>

#include "Library.hpp"
#include "../variables/Double.hpp"

namespace library {
    using namespace variable;

    class AlgorithmsLibrary : public Library {
        public:
            void linkGlobally(Scope &s);
            void linkNamespaced(string name, Scope &scope);

            static Library* getInstance();
    };
}