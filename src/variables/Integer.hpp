#pragma once

#include <iostream>
#include "Object.hpp"

typedef variable::Object*(*objectF)(variable::Object* t, std::vector<variable::Object*> args);

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
            Object* modulo (Object* other);

            bool equal (Object* other);
            bool less (Object* other);
            bool lesseq (Object* other);
            bool greater (Object* other);
            bool greatereq (Object* other);
            bool noteq (Object* other);

            void assign(Object* from);

            string getType();
            string toString();
            string getValueString();

            map<string, objectF> getFunctions();

            static Object* check(Object &other);
            
            int value;
    };
}