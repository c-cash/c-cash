#pragma once

#include "Type.hpp"
#include "Statement.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace parser {
    using namespace std;

    class ParameterDefinition {
        public:
            string mName; //Emty -> no name given
            Type mType;
            bool isArray { false };

            void DebugPrint(size_t indent) const;
    };

    class FunctionDefinition {
        public:
            string mName;
            vector<ParameterDefinition> mParameters;
            vector<Statement> mStatements;

            void DebugPrint() const;
    };
    
}