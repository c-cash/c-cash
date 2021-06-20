#include "Char.hpp"
#include "Long.hpp"
#include "Double.hpp"
#include "Object.hpp"
#include "Integer.hpp"
#include "String.hpp"

#include <iostream>
#include <map>

namespace variable {
    using namespace std;

    Char::Char(char value) {
        this->value = value;
    }
    Char::Char() {}

    // ADDITION
    Object* Char::add (Object* other) {
        string otherType = other->getType();
        if (otherType == "Char") return new Char(value + static_cast<Char*>(other)->value);
        else if (otherType == "Integer") return new Integer(value + static_cast<Integer*>(other)->value);
        else if (otherType == "Long") return new Long(value + static_cast<Long*>(other)->value);
        else if (otherType == "String") return new String(to_string(value) + other->getValueString());
        throw runtime_error("Cannot add " + this->getType() + " and " + other->getType());
    }
    // SUBTRACTION
    Object* Char::subtract (Object* other) {
        string otherType = other->getType();
        if (otherType == "Char") return new Char(value - static_cast<Char*>(other)->value);
        else if (otherType == "Integer") return new Integer(value - static_cast<Integer*>(other)->value);
        else if (otherType == "Long") return new Long(value - static_cast<Long*>(other)->value);
        throw runtime_error("Cannot subtract " + this->getType() + " and " + other->getType());
    }
    // MULTIPLICATION
    Object* Char::multiply (Object* other) {
        string otherType = other->getType();
        if (otherType == "Char") return new Char(value * static_cast<Char*>(other)->value);
        else if (otherType == "Integer") return new Integer(value * static_cast<Integer*>(other)->value);
        else if (otherType == "Long") return new Long(value * static_cast<Long*>(other)->value);
        else if (otherType == "String") 
            return new String([&]()->string {string t=other->getValueString();string r; for (int i=0; i<value; i++) r+=t; return r;}());
        throw runtime_error("Cannot multiply " + this->getType() + " and " + other->getType());
    }
    // DIVISION
    Object* Char::divide (Object* other) {
        string otherType = other->getType();
        if (otherType == "Char") return new Char(value / static_cast<Char*>(other)->value);
        else if (otherType == "Integer") return new Integer(value / static_cast<Integer*>(other)->value);
        else if (otherType == "Long") return new Long(value / static_cast<Long*>(other)->value);
        throw runtime_error("Cannot divide " + this->getType() + " and " + other->getType());
    }
    // MODULO
    Object* Char::modulo (Object* other) {
        string otherType = other->getType();
        if (otherType == "Char") return new Char(value % static_cast<Char*>(other)->value);
        else if (otherType == "Integer") return new Integer(value % static_cast<Integer*>(other)->value);
        else if (otherType == "Long") return new Long(value % static_cast<Long*>(other)->value);
        throw runtime_error("Cannot use modulo on " + this->getType() + " and " + other->getType());
    }
    //INCREMENTATION
    void Char::incrementation() {
        ++value;
    }
    //DEINCREMATATION
    void Char::decrementation() {
        --value;
    }

    bool Char::equal(Object* other) {
        string otherT = other->getType();
        if (otherT == "Char" || otherT == "Long" || otherT == "Integer") return getValueString() == other->getValueString();
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Char::less(Object* other) {
        string otherT = other->getType();
        if (otherT == "Char" || otherT == "Long" || otherT == "Integer") return value < stoi(other->getValueString());
        else if (otherT == "Double") return value < static_cast<Char*>(other)->value;
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Char::greater(Object* other) {
        string otherT = other->getType();
        if (otherT == "Char" || otherT == "Long" || otherT == "Integer") return value > stoi(other->getValueString());
        else if (otherT == "Double") return value > static_cast<Char*>(other)->value;
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Char::lesseq(Object* other) {
        string otherT = other->getType();
        if (otherT == "Char" || otherT == "Long" || otherT == "Integer") return value <= stoi(other->getValueString());
        else if (otherT == "Double") return value <= static_cast<Char*>(other)->value;
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Char::greatereq(Object* other) {
        string otherT = other->getType();
        if (otherT == "Char" || otherT == "Long" || otherT == "Integer") return value >= stoi(other->getValueString());
        else if (otherT == "Double") return value >= static_cast<Char*>(other)->value;
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Char::noteq(Object* other) {
        string otherT = other->getType();
        if (otherT == "Char" || otherT == "Long" || otherT == "Integer") return value != stoi(other->getValueString());
        else if (otherT == "Double") return value != static_cast<Char*>(other)->value;
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }

    void Char::assign(Object* from) {
        string t = from->getType();
        char v;
        if (t == "Char") v = static_cast<Char*>(from)->value;
        else if (t == "Integer") v = ((char) static_cast<Integer*>(from)->value);
        else throw runtime_error("Cannot assign to char");
        this->value = v;
    }

    string Char::toString() {return to_string(value); }
    string Char::getValueString() {return to_string((int) value);}

    string Char::getType() {return "Char";}

    Object* Char::check(Object &other) {
        if (other.getType() == "Char") return &other;
        throw runtime_error("variable types does not match");
    }

    map<string, objectF> Char::getFunctions() {
        return {};
    }
}