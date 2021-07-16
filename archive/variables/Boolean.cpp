#include <iostream>
#include "Boolean.hpp"
#include "Object.hpp"

namespace variable {
    using namespace std;

    Boolean::Boolean(bool value) {
        this->value = value;
    }
    Boolean::Boolean() {};

    Object* Boolean::add (Object* other) {
        throw runtime_error("Cannot add " + this->getType() + " and " + other->getType());
    }
    Object* Boolean::subtract (Object* other) {
        throw runtime_error("Cannot subtract " + this->getType() + " and " + other->getType());
    }
    Object* Boolean::multiply (Object* other) {
        throw runtime_error("Cannot multiply " + this->getType() + " and " + other->getType());
    }
    Object* Boolean::divide (Object* other) {
        throw runtime_error("Cannot divide " + this->getType() + " and " + other->getType());
    }
    Object* Boolean::modulo (Object* other) {
        throw runtime_error("Cannot use modulo on " + this->getType() + " and " + other->getType());
    }
    //INCREMENTATION
    void Boolean::incrementation() {
        throw runtime_error("Cannot use incrementation on " + this->getType());
    }
    //DEINCREMATATION
    void Boolean::decrementation() {
        throw runtime_error("Cannot use decrementation on " + this->getType());
    }

    bool Boolean::equal(Object* other) {
        string otherT = other->getType();
        if (otherT == "Boolean" || otherT == "Integer") return getValueString() == other->getValueString();
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Boolean::less(Object* other) {
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Boolean::greater(Object* other) {
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Boolean::lesseq(Object* other) {
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Boolean::greatereq(Object* other) {
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Boolean::noteq(Object* other) {
        string otherT = other->getType();
        if (otherT == "Boolean" || otherT == "Integer") return getValueString() != other->getValueString();
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }

    void Boolean::assign(Object* from) {
        string t = from->getType();
        if (t != "Integer" && t != "Boolean") throw runtime_error("Cannot assign to boolean");
        this->value = stoi(from->getValueString());
    }

    string Boolean::toString() {return value ? "true" : "false"; }
    string Boolean::getValueString() {return value ? "1" : "0";}

    string Boolean::getType() {return "Boolean";}

    Object* Boolean::check(Object &other) {
            if (other.getType() == "String") return &other;
            throw runtime_error("variable types does not match");
    }

    map<string, objectF> Boolean::getFunctions() {
        return {};
    }

}