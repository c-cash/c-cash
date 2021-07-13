#pragma once

#include "FunctionDefinition.hpp"
#include "Type.hpp"
#include "Statement.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace parser {
    using namespace std;

    class ClassDefinition {
        public:
            string mName;
            vector<FunctionDefinition> mFunctions;

            void DebugPrint() const;
    };
    
}