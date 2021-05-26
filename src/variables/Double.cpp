#include "Double.hpp"
#include "Object.hpp"
#include "String.hpp"
#include "Integer.hpp"

#include <iostream>
#include <cmath>

namespace variable {
    using namespace std;

    Double::Double(double value) {
        this->value = value;
    }
    Double::Double() {};

    // ADDITION
    Object* Double::add (Object* other) {
        string otherType = other->getType();
        if (otherType == "Integer") return new Double(value + stod(other->getValueString()));
        else if (otherType == "Double") return new Double(value + stod(other->getValueString()));
        else if (otherType == "String") return new String(to_string(value) + other->getValueString());
        throw runtime_error("Cannot add " + this->getType() + " and " + other->getType());
    }
    // SUBTRACTION
    Object* Double::subtract (Object* other) {
        string otherType = other->getType();
        if (otherType == "Integer") return new Double(value - stod(other->getValueString()));
        else if (otherType == "Double") return new Double(value - stod(other->getValueString()));
        throw runtime_error("Cannot subtract " + this->getType() + " and " + other->getType());
    }
    // MULTIPLICATION
    Object* Double::multiply (Object* other) {
        string otherType = other->getType();
        if (otherType == "Integer") return new Double(value * stod(other->getValueString()));
        else if (otherType == "Double") return new Double(value * stod(other->getValueString()));
        throw runtime_error("Cannot multiply " + this->getType() + " and " + other->getType());
    }
    // DIVISION
    Object* Double::divide (Object* other) {
        string otherType = other->getType();
        if (otherType == "Integer") return new Double(value / stod(other->getValueString()));
        else if (otherType == "Double") return new Double(value / stod(other->getValueString()));
        throw runtime_error("Cannot divide " + this->getType() + " and " + other->getType());
    }
    // DIVISION
    Object* Double::modulo (Object* other) {
        string otherType = other->getType();
        if (otherType == "Integer") return new Double(std::fmod(value, stod(other->getValueString())));
        else if (otherType == "Double") return new Double(std::fmod(value, stod(other->getValueString())));
        throw runtime_error("Cannot use modulo on " + this->getType() + " and " + other->getType());
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

    void Double::assign(Object* from) {
        string t = from->getType();
        if (t != "Integer" && t != "Double") throw runtime_error("Cannot assign to double");
        this->value = stod(from->getValueString());
    }

    string Double::toString() {
        // generate string value
        string v = to_string(value);
        int len = v.size();
        // remove zeros from end
        for (int i=v.size()-1; i>0; i--) {
            if (v[i] == '0') len--;
            else break;
        }
        if (v[len-1] == '.') len--;
        return v.substr(0, len);
    }
    string Double::getValueString() {return toString();}

    string Double::getType() {return "Double";}

    Object* Double::check(Object &other) {
            if (other.getType() == "Double") return &other;
            throw runtime_error("variable types does not match");
    }

}