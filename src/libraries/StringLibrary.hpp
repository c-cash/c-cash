#pragma once

#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>

#include "Library.hpp"
#include "../variables/String.hpp"
#include "../transpiler/Transpiler.hpp"

namespace library {
    using namespace variable;

    class StringLibrary : public Library {
        public:
            void linkGlobally(Scope &s);
            void linkNamespaced(string name, Scope &scope);

            static Library* getInstance();
    };
}