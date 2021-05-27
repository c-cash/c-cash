#pragma once

#include <iostream>
#include "Object.hpp"

typedef variable::Object*(*objectF)(variable::Object* t, std::vector<variable::Object*> args);

namespace variable {
    using namespace std;

    enum SpecialType {
        RETURN,
        BREAK,
        CONTINUE,
        NONE
    };

    class SpecialObject : public variable::Object {
        public:

            SpecialObject(SpecialType value, Object* var);
            SpecialObject();

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
            void assign(SpecialType type);

            string getType();
            string toString();
            string getValueString();

            map<string, objectF> getFunctions();

            static Object* check(Object &other);

            SpecialType value;
            Object* var;
    };
}