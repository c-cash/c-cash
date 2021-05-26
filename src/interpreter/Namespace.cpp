#include <iostream>

#include "Namespace.hpp"

namespace interpreter {
    using namespace std;

    void Namespace::addBuiltin(string name, builtinF f) {
        functions[name] = f;
    }
}