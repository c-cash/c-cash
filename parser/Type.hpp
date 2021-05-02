#pragma once

#include <string>
#include <vector>

namespace parser {
    using namespace std;

    enum BUILTIN_TYPE {
        FUNC,
        FUNC_PARAM,
        INT8,
        UINT8,
        INT32,
        UINT32,
        DOUBLE,
        STRING
    };

    class Type {
        public:
            Type(const string &name = "", enum BUILTIN_TYPE type = FUNC) : mName(name), mType(type) {}

            string mName;
            enum BUILTIN_TYPE mType;
            vector<Type> mFiled; //It's for struct
    };
}