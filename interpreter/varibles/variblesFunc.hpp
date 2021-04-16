#pragma once

#include "../../parser/Statement.hpp"

namespace interpreter {
    using namespace std;
    using namespace parser;

    class VariblesFunc {
        public:
            double doubleValue(Statement &variable);
            char charValue(Statement &variable);
            unsigned char ucharValue(Statement &variable);
            int intValue(Statement &variable);
            unsigned int uintValue(Statement &variable);
    };
}