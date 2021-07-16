// THIS OBJECT IS USED TO COMMUNICATE BETWEEN FUNCTIONS!!!

#include <iostream>
#include "SpecialObject.hpp"
#include "Object.hpp"

namespace variable {
    using namespace std;

    SpecialObject::SpecialObject(SpecialType value, Object* var) {
        this->value = value;
        this->var = var;
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
    Object* SpecialObject::modulo (Object* other) {
        throw runtime_error("Cannot use modulo on " + this->getType() + " and " + other->getType());
    }
    //INCREMENTATION
    void SpecialObject::incrementation() {
        throw runtime_error("Cannot use incrementation on " + this->getType());
    }
    //DEINCREMATATION
    void SpecialObject::decrementation() {
        throw runtime_error("Cannot use decrementation on " + this->getType());
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

    void SpecialObject::assign(Object* from) {
        throw runtime_error("Cannot assign to SpecialObject type");
    }
    void SpecialObject::assign(SpecialType type) {
        this->value = type;
    }

    string SpecialObject::toString() {
        switch (this->value) {
            case SpecialType::RETURN: 
                return "return";
                break;
            case SpecialType::BREAK:
                return "break";
                break;
            case SpecialType::CONTINUE:
                return "continue";
                break;
            default:
                return "null";
                break;
        }
    }
    string SpecialObject::getValueString() {return toString();}

    string SpecialObject::getType() {return "SpecialObject";}

    Object* SpecialObject::check(Object &other) {
            if (other.getType() == "SpecialObject") return &other;
            throw runtime_error("variable types does not match");
    }

    map<string, objectF> SpecialObject::getFunctions() {
        return {};
    }

}