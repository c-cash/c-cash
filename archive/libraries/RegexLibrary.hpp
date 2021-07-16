#pragma once

#include <iostream>

#include "Library.hpp"
#include "../variables/String.hpp"

namespace library {
    using namespace variable;

    class RegexLibrary : public Library {
        public:
            void linkGlobally(Scope &s);
            void linkNamespaced(string name, Scope &scope);

            static Library* getInstance();
    };
}