#include "Object.hpp"

#include <iostream>
#include <cmath>

#include "../parser/Statement.hpp"
#include "../transpiler/Transpiler.hpp"

#include "Integer.hpp"
#include "Double.hpp"
#include "String.hpp"
#include "Boolean.hpp"
#include "Long.hpp"

namespace variable {
    using namespace std;

    Object::Object() {}

    Object* Object::fromLitteral(parser::Statement &stmt) {
        if (stmt.mKind != parser::StatementKind::LITERAL) return nullptr;
        transpiler::Transpiler::fixName(stmt);
        // other
        if (stmt.mType.mName == "double") return new variable::Double(stod(stmt.mName));
        else if (stmt.mType.mName == "signed int") return new variable::Integer(stoi(stmt.mName));
        else if (stmt.mType.mName == "string") return new variable::String(stmt.mName);
        return nullptr;
    }

    Object* Object::getDefault(string &type) {
        if (type == "signed int") return new Integer(0);
        else if (type == "double") return new Double(0);
        else if (type == "string") return new String("");
        else if (type == "bool") return new Boolean(false);
        
        throw runtime_error("this type of variable needs assignment after delaration");
    }

    Object* Object::checkAll(string expected, Object* val) {
            string valT = val->getType();
            // if array then it will throw error somewhere else
            if (valT.substr(0,5)=="Array") return val;

            if ((expected == "signed int"||expected=="Integer") && valT == "Integer") return val;
            else if ((expected == "double"||expected=="Double") && valT == "Double") return val;
            else if ((expected == "string"||expected=="String") && valT == "String") return val;
            else if ((expected == "bool"||expected=="Boolean") && valT == "Boolean") return val;
            else if ((expected == "long"||expected=="Long") && valT == "Long") return val;

            // other cases

            // LONG
            if ((expected == "long"||expected=="Long") && valT == "Integer") return new Long(stoll(val->getValueString()));

            // BOOLEAN
            else if ((expected == "bool"||expected=="boolean") && valT == "Integer") return new Boolean(stoi(val->getValueString()));
            // DOUBLE AND INT
            else if ((expected == "double"||expected=="Double") && valT == "Integer") return new Double(stod(val->getValueString()));
            else if ((expected == "signed int"||expected=="Integer") && valT == "Double") return new Integer(floor(stod(val->getValueString())));
            // STRING TO INTEGER
            else if ((expected == "signed int"||expected=="Integer") && valT == "String") {
                try {
                    return new Integer(stoi(val->getValueString()));
                } catch (exception e) {
                    throw runtime_error("cannot convert string to integer");
                }
            } 
            // STRING TO DOUBLE
            else if ((expected == "double"||expected=="Double") && valT == "String") {
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