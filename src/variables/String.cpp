#include <iostream>
#include "String.hpp"
#include "Object.hpp"

namespace variable {
    using namespace std;

    String::String(string value) {
        this->value = value;
    }
    String::String() {};

    Object* String::add (Object* other) {
        string otherType = other->getType();
        if (otherType == "String") return new String(value + other->getValueString());
        throw runtime_error("Cannot add " + this->getType() + " and " + other->getType());
    }
    Object* String::subtract (Object* other) {
        throw runtime_error("Cannot subtract " + this->getType() + " and " + other->getType());
    }
    Object* String::multiply (Object* other) {
        throw runtime_error("Cannot multiply " + this->getType() + " and " + other->getType());
    }
    Object* String::divide (Object* other) {
        throw runtime_error("Cannot divide " + this->getType() + " and " + other->getType());
    }
    Object* String::modulo (Object* other) {
        throw runtime_error("Cannot use modulo on " + this->getType() + " and " + other->getType());
    }

    bool String::equal(Object* other) {
        if (other->getType() == "String")
            return value == other->getValueString();
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool String::less(Object* other) {
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool String::greater(Object* other) {
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool String::lesseq(Object* other) {
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool String::greatereq(Object* other) {
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool String::noteq(Object* other) {
       if (other->getType() == "String")
            return value != other->getValueString();
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }

    void String::assign(Object* from) {
        this->value = from->getValueString();
    }

    string String::toString() {return value;}
    string String::getValueString() {return value;}

    string String::getType() {return "String";}

    Object* String::check(Object &other) {
            if (other.getType() == "String") return &other;
            throw runtime_error("variable types does not match");
    }

    map<string, objectF> String::getFunctions() {
        return {};
    }

}