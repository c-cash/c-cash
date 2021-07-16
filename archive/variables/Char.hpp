#pragma once

#include <iostream>
#include <memory>
#include "Object.hpp"

namespace variable {
    using namespace std;

    class Char : public variable::Object {
        public:
            Char(char value);
            Char();
            Object* add (Object* other);
            Object* subtract (Object* other);
            Object* multiply (Object* other);
            Object* divide (Object* other);
            Object* modulo (Object* other);
            void incrementation();
            void decrementation();

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
            
            char value;
    };
}