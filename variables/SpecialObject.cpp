// THIS OBJECT IS USED TO COMMUNICATE BETWEEN FUNCTIONS!!!

#include <iostream>
#include "SpecialObject.hpp"
#include "Object.hpp"

namespace variable {
    using namespace std;

    SpecialObject::SpecialObject(SpecialType value) {
        this->value = value;
    }
    SpecialObject::SpecialObject() {};

    Object* SpecialObject::add (Object* other) {
        throw runtime_error("Cannot add " + this->getType() + " and " + other->getType());
    }
    Object* SpecialObject::subtract (Object* other) {
        throw runtime_error("Cannot subtract " + this->getType() + " and " + other->getType());
    }
    Object* SpecialObject::multiply (Object* other) {
        throw runtime_error("Cannot multiply " + this->getType() + " and " + other->getType());
    }
    Object* SpecialObject::divide (Object* other) {
        throw runtime_error("Cannot divide " + this->getType() + " and " + other->getType());
    }

    bool SpecialObject::equal(Object* other) {
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool SpecialObject::less(Object* other) {
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool SpecialObject::greater(Object* other) {
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool SpecialObject::lesseq(Object* other) {
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool SpecialObject::greatereq(Object* other) {
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool SpecialObject::noteq(Object* other) {
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }

    string SpecialObject::toString() {return "null";}
    string SpecialObject::getValueString() {return "null";}

    string SpecialObject::getType() {return "SpecialObject";}

    Object* SpecialObject::check(Object &other) {
            if (other.getType() == "SpecialObject") return &other;
            throw runtime_error("variable types does not match");
    }

}