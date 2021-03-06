#pragma once

#include <vector>
#include <type_traits>
#include "Object.hpp"

namespace variable {
    using namespace std;

    class Array : public variable::Object {
        public:

            Array(std::vector<Object*> value, string type);
            Array(std::vector<Object*> value);
            Array(std::string type);
            Array();

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
            static Object* checkAll(string expected, Object* obj);
            static Array* convert(Object* obj);
            static Object* assignIndex(int index, Object* array, Object* value);
            static Object* getIndex(int index, Object* array);
            static Object* getDefault(string type);
            static void checkArray(string type, vector<Object*> arr);

            vector<Object*> value;
            string type;
    };
}