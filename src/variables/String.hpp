#pragma once

#include <iostream>
#include "Object.hpp"
#include "Array.hpp"

namespace variable {
    using namespace std;

    class String : public variable::Object {
        public:
            String(string value);
            String();

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

            Array* c_arr();

            map<string, objectF> getFunctions();

            static Object* check(Object &other);

            string value;
    };
}