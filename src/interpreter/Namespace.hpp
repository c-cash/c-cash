#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "../variables/Object.hpp"

typedef std::vector<variable::Object*>(*builtinF)(std::vector<variable::Object*> args);

namespace interpreter {
    using namespace std;
    using namespace variable;

    class Namespace {
        public:
            map<string, builtinF> functions;
            void addBuiltin(string name, builtinF); // normal builtin
    };
}