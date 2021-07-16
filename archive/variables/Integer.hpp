#pragma once

#include <iostream>
#include <memory>
#include "Object.hpp"

namespace variable {
    using namespace std;

    class Integer : public variable::Object {
        public:
            Integer(uint value, bool ui);
            Integer(uint value);
            Integer();
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

            int getSignedValue();

            map<string, objectF> getFunctions();

            static Object* check(Object &other);
            
            uint value;
            bool ui {false};
    };
}