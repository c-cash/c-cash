#include <iostream>
#include "Integer.hpp"
#include "Double.hpp"
#include "String.hpp"
#include "Object.hpp"

namespace variable {
    using namespace std;

    Integer::Integer(int value) {
        this->value = value;
    }
    Integer::Integer() {}

    // ADDITION
    Object* Integer::add (Object* other) {
        string otherType = other->getType();
        if (otherType == "Integer") return new Integer(value + stoi(other->getValueString()));
        else if (otherType == "Double") return new Double(value + stod(other->getValueString()));
        else if (otherType == "String") return new String(to_string(value) + other->getValueString());
        throw runtime_error("Cannot add " + this->getType() + " and " + other->getType());
    }

    // SUBTRACTION
    Object* Integer::subtract (Object* other) {
        string otherType = other->getType();
        if (otherType == "Integer") return new Integer(value - stoi(other->getValueString()));
        else if (otherType == "Double") return new Double(value - stod(other->getValueString()));
        throw runtime_error("Cannot subtract " + this->getType() + " and " + other->getType());
    }
    // MULTIPLICATION
    Object* Integer::multiply (Object* other) {
        string otherType = other->getType();
        if (otherType == "Integer") return new Integer(value * stoi(other->getValueString()));
        else if (otherType == "Double") return new Double(value * stod(other->getValueString()));
        else if (otherType == "String") 
            return new String([&]()->string {string t=other->getValueString();string r; for (int i=0; i<value; i++) r+=t; return r;}());
        throw runtime_error("Cannot multiply " + this->getType() + " and " + other->getType());
    }
    Object* Integer::divide (Object* other) {
        string otherType = other->getType();
        if (otherType == "Integer") return new Integer(value / stoi(other->getValueString()));
        else if (otherType == "Double") return new Double(value / stod(other->getValueString()));
        throw runtime_error("Cannot divide " + this->getType() + " and " + other->getType());
    }

    bool Integer::equal(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer" || otherT == "Double") return getValueString() == other->getValueString();
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Integer::less(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer") return value < stoi(other->getValueString());
        else if (otherT == "Double") return value < stod(other->getValueString());
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Integer::greater(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer") return value > stoi(other->getValueString());
        else if (otherT == "Double") return value > stod(other->getValueString());
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Integer::lesseq(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer") return value <= stoi(other->getValueString());
        else if (otherT == "Double") return value <= stod(other->getValueString());
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Integer::greatereq(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer") return value >= stoi(other->getValueString());
        else if (otherT == "Double") return value >= stod(other->getValueString());
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Integer::noteq(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer") return value != stoi(other->getValueString());
        else if (otherT == "Double") return value != stod(other->getValueString());
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }

    string Integer::toString() {return to_string(value); }
    string Integer::getValueString() {return to_string(value);}

    string Integer::getType() {return "Integer";}

    Object* Integer::check(Object &other) {
            if (other.getType() == "Integer") return &other;
            throw runtime_error("variable types does not match");
    }
}