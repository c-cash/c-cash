#include "Object.hpp"

#include <iostream>

#include "../parser/Statement.hpp"
#include "../transpiler/Transpiler.hpp"

#include "Integer.hpp"
#include "Double.hpp"
#include "String.hpp"

namespace variable {
    using namespace std;

    Object::Object() {}

    Object* Object::fromLitteral(parser::Statement &stmt) {
        if (stmt.mKind != parser::StatementKind::LITERAL) return nullptr;
        transpiler::Transpiler::fixName(stmt);
        if (stmt.mType.mName == "double") return new variable::Double(stod(stmt.mName));
        else if (stmt.mType.mName == "signed int") return new variable::Integer(stoi(stmt.mName));
        else if (stmt.mType.mName == "string") return new variable::String(stmt.mName);
        return nullptr;
    }

    Object* Object::checkAll(string expected, Object* val) {
            string valT = val->getType();
            if ((expected == "signed int"||expected=="Integer") && valT == "Integer") return val;
            else if ((expected == "double"||expected=="Double") && valT == "Double") return val;
            else if ((expected == "string"||expected=="String") && valT == "String") return val;
            //TODO: bool :D

            // other cases
            // STRING TO INTEGER
            else if (expected == "signed int" && valT == "String") {
                try {
                    return new Integer(stoi(val->getValueString()));
                } catch (exception e) {
                    throw runtime_error("cannot convert string to integer");
                }
            } 
            // STRING TO DOUBLE
            else if (expected == "double" && valT == "String") {
                try {
                    return new Double(stod(val->getValueString()));
                } catch (exception e) {
                    throw runtime_error("cannot convert string to double");
                }
            }
            throw runtime_error("variable types does not match");
    }

    Object* Object::check(Object &other) {
            if (other.getType() == "Object") return &other;
            throw runtime_error("variable types does not match");
    }
}