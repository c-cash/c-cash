#pragma once

#include "../interpreter/Functions.hpp"

namespace library {
    using namespace interpreter;

    class Library {
        public:

            virtual void linkGlobally(Scope &s) = 0;
            virtual void linkNamespaced(string name, Scope &scope) = 0;
    };
}