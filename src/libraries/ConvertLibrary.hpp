#pragma once

#include <iostream>
#include <string>

#include "Library.hpp"

namespace library {
    using namespace variable;

    class ConvertLibrary : public Library {
        public:
            void linkGlobally(Scope &s);
            void linkNamespaced(string name, Scope &scope);

            static Library* getInstance();
    };
}