#pragma once

#include <map>
#include "../parser/FunctionDefinition.hpp"
#include "../parser/ClassDefinition.hpp"

namespace optimalizer {
    using namespace std;
    using namespace parser;

    class Optimalizer {
        public:
            Optimalizer();
            void optimize(map<string, FunctionDefinition> &aFunctions, map<string, ClassDefinition> &aClasses);
    };
}