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
#include "Char.hpp"

namespace variable {
    using namespace std;

    Object::Object() {}

    Object* Object::fromLitteral(parser::Statement &stmt) {
        if (stmt.mKind != parser::StatementKind::LITERAL) return nullptr;
        // other
        if (stmt.mType.mName == "double") return new variable::Double(stod(stmt.mName));
        else if (stmt.mType.mName == "signed int") return new variable::Integer(stoi(stmt.mName));
        else if (stmt.mType.mName == "string") return new variable::String(stmt.mName);
        else if (stmt.mType.mName == "char") return new variable::Char(stmt.mName[0]);
        return nullptr;
    }

    Object* Object::getDefault(string &type) {
        if (type == "signed int") return new Integer(0);
        else if (type == "double") return new Double(0);
        else if (type == "string") return new String("");
        else if (type == "bool") return new Boolean(false);
        else if (type == "char") return new Char();
        
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
            else if (expected == "char" && valT == "Char") return val;
            
            // other cases

            // CHAR TO INT
            else if ((expected == "signed int"||expected=="Integer") && valT == "Char") return new Integer((int)static_cast<Char*>(val)->value);
            // STRING TO CHAR
            else if (expected == "char" && valT == "String") {
                String* temp = static_cast<String*>(val);
                if (temp->value.size() != 1) throw runtime_error("Cannot assign string to char");
                return new Char(temp->value[0]);
            }

            // LONG
            else if ((expected == "long"||expected=="Long") && valT == "Integer") return new Long(stoll(val->getValueString()));

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