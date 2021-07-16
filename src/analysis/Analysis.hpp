#pragma once

#include <map>
#include "../parser/FunctionDefinition.hpp"
#include "../parser/ClassDefinition.hpp"


namespace analysis {
    using namespace std;
    using namespace parser;

    class Analysis {
        public:
            Analysis();
            void analize(map<string, FunctionDefinition> &aFunctions, map<string, ClassDefinition> &aClasses);
    };
}