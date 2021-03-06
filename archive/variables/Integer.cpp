#include "Integer.hpp"
#include "Long.cpp"
#include "Double.hpp"
#include "String.hpp"
#include "Object.hpp"

#include <iostream>
#include <cmath>
#include <map>

#include <memory>

namespace variable {
    using namespace std;

    Integer::Integer(uint value, bool ui) {
        if (ui) {
            this->ui = true;
            this->value = value;
        } else {
            this->value = value;
        }
    }
    Integer::Integer(uint value) {
        this->value = value;
    }
    Integer::Integer() {}

    // ADDITION
    Object* Integer::add (Object* other) {
        string otherType = other->getType();
        if (otherType == "Integer") return new Integer(value + static_cast<Integer*>(other)->value, this->ui);
        else if (otherType == "Long") return new Long(value + static_cast<Long*>(other)->value, this->ui);
        else if (otherType == "Double") return new Double(1.0 * value + static_cast<Double*>(other)->value);
        else if (otherType == "String") return new String(to_string(value) + other->getValueString());
        throw runtime_error("Cannot add " + this->getType() + " and " + other->getType());
    }
    // SUBTRACTION
    Object* Integer::subtract (Object* other) {
        string otherType = other->getType();
        if (otherType == "Integer") return new Integer(value - static_cast<Integer*>(other)->value, this->ui);
        else if (otherType == "Long") return new Long(value - static_cast<Long*>(other)->value, this->ui);
        else if (otherType == "Double") return new Double(1.0 * value - static_cast<Double*>(other)->value);
        throw runtime_error("Cannot subtract " + this->getType() + " and " + other->getType());
    }
    // MULTIPLICATION
    Object* Integer::multiply (Object* other) {
        string otherType = other->getType();
        if (otherType == "Integer") return new Integer(value * static_cast<Integer*>(other)->value, this->ui);
        else if (otherType == "Long") return new Long(value * static_cast<Long*>(other)->value, this->ui);
        else if (otherType == "Double") return new Double(1.0 * value * static_cast<Double*>(other)->value);
        else if (otherType == "String") 
            return new String([&]()->string {string t=other->getValueString();string r; for (int i=0; i<value; i++) r+=t; return r;}());
        throw runtime_error("Cannot multiply " + this->getType() + " and " + other->getType());
    }
    // DIVISION
    Object* Integer::divide (Object* other) {
        string otherType = other->getType();
        if (otherType == "Integer") return new Integer(value / static_cast<Integer*>(other)->value, this->ui);
        else if (otherType == "Long") return new Long(value / static_cast<Long*>(other)->value, this->ui);
        else if (otherType == "Double") return new Double(1.0 * value / static_cast<Double*>(other)->value);
        throw runtime_error("Cannot divide " + this->getType() + " and " + other->getType());
    }
    // MODULO
    Object* Integer::modulo (Object* other) {
        string otherType = other->getType();
        if (otherType == "Integer") return new Integer(value % static_cast<Integer*>(other)->value, this->ui);
        else if (otherType == "Long") return new Long(value % static_cast<Long*>(other)->value, this->ui);
        else if (otherType == "Double") return new Double(std::fmod(1.0 * value, static_cast<Double*>(other)->value));
        throw runtime_error("Cannot use modulo on " + this->getType() + " and " + other->getType());
    }
    //INCREMENTATION
    void Integer::incrementation() {
        ++value;
    }
    //DEINCREMATATION
    void Integer::decrementation() {
        --value;
    }

    bool Integer::equal(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer" || otherT == "Double" || otherT == "Long") return getValueString() == other->getValueString();
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Integer::less(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer" || otherT == "Long") return value < stoi(other->getValueString());
        else if (otherT == "Double") return value < static_cast<Integer*>(other)->value;
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Integer::greater(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer" || otherT == "Long") return value > stoi(other->getValueString());
        else if (otherT == "Double") return value > static_cast<Integer*>(other)->value;
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Integer::lesseq(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer" || otherT == "Long") return value <= stoi(other->getValueString());
        else if (otherT == "Double") return value <= static_cast<Integer*>(other)->value;
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Integer::greatereq(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer" || otherT == "Long") return value >= stoi(other->getValueString());
        else if (otherT == "Double") return value >= static_cast<Integer*>(other)->value;
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Integer::noteq(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer" || otherT == "Long") return value != stoi(other->getValueString());
        else if (otherT == "Double") return value != static_cast<Integer*>(other)->value;
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }

    void Integer::assign(Object* from) {
        string t = from->getType();
        uint v = 0;
        if (t == "Integer") v = static_cast<Integer*>(from)->getSignedValue();
        else if (t == "Double") v = static_cast<Double*>(from)->value;
        else throw runtime_error("Cannot assign to int");
        this->value = v;
    }

    string Integer::toString() { if (this->ui) return to_string(value); else return to_string((int) value); }
    string Integer::getValueString() { if (this->ui) return to_string(value); else return to_string((int) value); }

    int Integer::getSignedValue() { if (this->ui) return value; else return (int) value; }

    string Integer::getType() {return "Integer";}

    Object* Integer::check(Object &other) {
        if (other.getType() == "Integer") return &other;
        throw runtime_error("variable types does not match");
    }

    map<string, objectF> Integer::getFunctions() {
        return {};
    }
}