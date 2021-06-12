#pragma once

#include <iostream>

#include "../parser/Statement.hpp"
#include <map>

namespace variable {
    using namespace std;

    class Object {
        public:
            Object();
            ~Object() {};
            virtual Object* add (Object* other) = 0;
            virtual Object* subtract (Object* other) = 0;
            virtual Object* divide (Object* other) = 0;
            virtual Object* multiply (Object* other) = 0;
            virtual Object* modulo (Object* other) = 0;
            virtual void incrementation() = 0;
            virtual void decrementation() = 0;

            virtual bool equal(Object* other) = 0;
            virtual bool less(Object* other) = 0;
            virtual bool lesseq(Object* other) = 0;
            virtual bool greater(Object* other) = 0;
            virtual bool greatereq(Object* other) = 0;
            virtual bool noteq(Object* other) = 0;
            virtual void assign(Object* from) = 0;

            virtual string toString() = 0;
            virtual string getValueString() = 0;
            virtual string getType() = 0;

            virtual map<string, Object*(*)(variable::Object* t, std::vector<variable::Object*> &args)> getFunctions() = 0;
            
            static Object* getDefault(string &type);
            static Object* fromLitteral(parser::Statement &stmt);
            static Object* checkAll(string expected, Object* var);
            static Object* check(Object &other);
    };

    typedef Object*(*objectF)(Object* t, std::vector<Object*> &args);
}