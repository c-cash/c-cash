#include <iostream>
#include "Double.hpp"
#include "Object.hpp"
#include "String.hpp"
#include "Integer.hpp"

namespace variable {
    using namespace std;

    Double::Double(double value) {
        this->value = value;
    }
    Double::Double() {};

    // ADDITION
    Object* Double::add (Object* other) {
        string otherType = other->getType();
        if (otherType == "Integer") return new Integer(value + stoi(other->getValueString()));
        else if (otherType == "Double") return new Double(value + stod(other->getValueString()));
        else if (otherType == "String") return new String(to_string(value) + other->getValueString());
        throw runtime_error("Cannot add " + this->getType() + " and " + other->getType());
    }
    // SUBTRACTION
    Object* Double::subtract (Object* other) {
        string otherType = other->getType();
        if (otherType == "Integer") return new Integer(value - stoi(other->getValueString()));
        else if (otherType == "Double") return new Double(value - stod(other->getValueString()));
        throw runtime_error("Cannot subtract " + this->getType() + " and " + other->getType());
    }
    // MULTIPLICATION
    Object* Double::multiply (Object* other) {
        string otherType = other->getType();
        if (otherType == "Integer") return new Integer(value * stoi(other->getValueString()));
        else if (otherType == "Double") return new Double(value * stod(other->getValueString()));
        throw runtime_error("Cannot multiply " + this->getType() + " and " + other->getType());
    }
    // DIVISION
    Object* Double::divide (Object* other) {
        string otherType = other->getType();
        if (otherType == "Integer") return new Integer(value / stoi(other->getValueString()));
        else if (otherType == "Double") return new Double(value / stod(other->getValueString()));
        throw runtime_error("Cannot divide " + this->getType() + " and " + other->getType());
    }

    bool Double::equal(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer" || otherT == "Double") return getValueString() == other->getValueString();
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Double::less(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer") return value < stoi(other->getValueString());
        else if (otherT == "Double") return value < stod(other->getValueString());
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Double::greater(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer") return value > stoi(other->getValueString());
        else if (otherT == "Double") return value > stod(other->getValueString());
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Double::lesseq(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer") return value <= stoi(other->getValueString());
        else if (otherT == "Double") return value <= stod(other->getValueString());
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Double::greatereq(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer") return value >= stoi(other->getValueString());
        else if (otherT == "Double") return value >= stod(other->getValueString());
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Double::noteq(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer") return value != stoi(other->getValueString());
        else if (otherT == "Double") return value != stod(other->getValueString());
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }

    string Double::toString() {return to_string(value); }
    string Double::getValueString() {return to_string(value);}

    string Double::getType() {return "Boolean";}

    Object* Double::check(Object &other) {
            if (other.getType() == "Boolean") return &other;
            throw runtime_error("variable types does not match");
    }

}