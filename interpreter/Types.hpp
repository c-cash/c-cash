#pragma once 

#include <map>
#include <string>

namespace interpreter {
    using namespace std;

    class Variables {
        public:
            map<string, double> doubleVarTab;
            map<string, int> intVarTab;
            map<string, unsigned int> uintVarTab;
            map<string, char> charVarTab;
            map<string, unsigned char> ucharVarTab;
    };
}