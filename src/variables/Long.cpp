#include "Long.hpp"
#include "Double.hpp"
#include "String.hpp"
#include "Object.hpp"

#include <iostream>
#include <cmath>
#include <map>

namespace variable {
    using namespace std;

    Long::Long(long long value) {
        this->value = value;
    }
    Long::Long() {}

    // ADDITION
    Object* Long::add (Object* other) {
        string otherType = other->getType();
        if (otherType == "Integer" || otherType == "Long") return new Long(value + static_cast<Long*>(other)->value);
        else if (otherType == "Double") return new Double(1.0 * value + static_cast<Double*>(other)->value);
        else if (otherType == "String") return new String(to_string(value) + other->getValueString());
        throw runtime_error("Cannot add " + this->getType() + " and " + other->getType());
    }
    // SUBTRACTION
    Object* Long::subtract (Object* other) {
        string otherType = other->getType();
        if (otherType == "Integer" || otherType == "Long") return new Long(value - static_cast<Long*>(other)->value);
        else if (otherType == "Double") return new Double(1.0 * value - stod(other->getValueString()));
        throw runtime_error("Cannot subtract " + this->getType() + " and " + other->getType());
    }
    // MULTIPLICATION
    Object* Long::multiply (Object* other) {
        string otherType = other->getType();
        if (otherType == "Integer" || otherType == "Long") return new Long(value * static_cast<Long*>(other)->value);
        else if (otherType == "Double") return new Double(1.0 * value * stod(other->getValueString()));
        else if (otherType == "String") 
            return new String([&]()->string {string t=other->getValueString();string r; for (int i=0; i<value; i++) r+=t; return r;}());
        throw runtime_error("Cannot multiply " + this->getType() + " and " + other->getType());
    }
    // DIVISION
    Object* Long::divide (Object* other) {
        string otherType = other->getType();
        if (otherType == "Integer" || otherType == "Long") return new Long(value / static_cast<Long*>(other)->value);
        else if (otherType == "Double") return new Double(1.0 * value / static_cast<Double*>(other)->value);
        throw runtime_error("Cannot divide " + this->getType() + " and " + other->getType());
    }
    // MODULO
    Object* Long::modulo (Object* other) {
        string otherType = other->getType();
        if (otherType == "Integer" || otherType == "Long") return new Long(value % static_cast<Long*>(other)->value);
        else if (otherType == "Double") return new Double(std::fmod(1.0 * value, static_cast<Double*>(other)->value));
        throw runtime_error("Cannot use modulo on " + this->getType() + " and " + other->getType());
    }
    //INCREMENTATION
    void Long::incrementation() {
        ++value;
    }
    //DEINCREMATATION
    void Long::decrementation() {
        --value;
    }

    bool Long::equal(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer" || otherT == "Double" || otherT == "Long") return getValueString() == other->getValueString();
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Long::less(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer" || otherT == "Long") return value < stoll(other->getValueString());
        else if (otherT == "Double") return value < stod(other->getValueString());
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Long::greater(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer" || otherT == "Long") return value > stoll(other->getValueString());
        else if (otherT == "Double") return value > stod(other->getValueString());
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Long::lesseq(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer" || otherT == "Long") return value <= stoll(other->getValueString());
        else if (otherT == "Double") return value <= stod(other->getValueString());
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Long::greatereq(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer" || otherT == "Long") return value >= stoll(other->getValueString());
        else if (otherT == "Double") return value >= stod(other->getValueString());
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Long::noteq(Object* other) {
        string otherT = other->getType();
        if (otherT == "Integer" || otherT == "Long") return value != stoll(other->getValueString());
        else if (otherT == "Double") return value != stod(other->getValueString());
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }

    void Long::assign(Object* from) {
        string t = from->getType();
        if (t != "Integer" && t != "Double" && t != "Long") throw runtime_error("Cannot assign to long");
        this->value = stoll(from->getValueString());
    }

    string Long::toString() {return to_string(value); }
    string Long::getValueString() {return to_string(value);}

    string Long::getType() {return "Long";}

    Object* Long::check(Object &other) {
            if (other.getType() == "Long") return &other;
            throw runtime_error("variable types does not match");
    }

    map<string, objectF> Long::getFunctions() {
        return {};
    }
}