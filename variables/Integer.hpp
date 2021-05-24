#pragma once

#include <iostream>
#include "Object.hpp"

namespace variable {
    using namespace std;

    class Integer : public variable::Object {
        public:
            Integer(int value);
            Integer();
            Object* add (Object* other);
            Object* subtract (Object* other);
            Object* multiply (Object* other);
            Object* divide (Object* other);

            bool equal (Object* other);
            bool less (Object* other);
            bool lesseq (Object* other);
            bool greater (Object* other);
            bool greatereq (Object* other);
            bool noteq (Object* other);

            string getType();
            string toString();
            string getValueString();

            static Object* check(Object &other);
            
            int value;
    };
}