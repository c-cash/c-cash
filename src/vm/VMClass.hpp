#pragma once

#include <iostream>

namespace vm {
    using namespace std;

    class VMClass {
        public:
            std::string name{"Undefined"};
            VMClass(string name);
    };
}