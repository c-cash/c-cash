#include <iostream>
#include <vector>
#include "Array.hpp"
#include "Object.hpp"
#include "Integer.hpp"

template<typename Base, typename T>
inline bool instanceof(const T*) {
   return std::is_base_of<Base, T>::value;
}

namespace variable {
    using namespace std;

    Array::Array(vector<Object*> value, string type) {
        if (value.size() == 0) {
            Array* a = convert(getDefault(type));
            this->value = a->value;
            this->type = a->type;
            return;
        }
        this->value = value;
        this->type = value[0]->getType();
        checkArray(type, value);
    }
    Array::Array(vector<Object*> value) {
        this->value = value;
        this->type = value[0]->getType();
        checkArray(type, value);
    }
    Array::Array(string type) {
        this->type = type;
    };
    Array::Array() {};

    Object* Array::add (Object* other) {
        throw runtime_error("Cannot add " + this->getType() + " and " + other->getType());
    }
    Object* Array::subtract (Object* other) {
        throw runtime_error("Cannot subtract " + this->getType() + " and " + other->getType());
    }
    Object* Array::multiply (Object* other) {
        throw runtime_error("Cannot multiply " + this->getType() + " and " + other->getType());
    }
    Object* Array::divide (Object* other) {
        throw runtime_error("Cannot divide " + this->getType() + " and " + other->getType());
    }
    Object* Array::modulo (Object* other) {
        throw runtime_error("Cannot use modulo on " + this->getType() + " and " + other->getType());
    }
    //INCREMENTATION
    void Array::incrementation() {
        throw runtime_error("Cannot use incrementation on " + this->getType());
    }
    //DEINCREMATATION
    void Array::decrementation() {
        throw runtime_error("Cannot use decrementation on " + this->getType());
    }

    bool Array::equal(Object* other) {
        Array* arr = convert(other);
        if (type != arr->type) throw runtime_error("Array types does not match");
        if (value.size() != arr->value.size()) throw runtime_error("Array sizes does not match");
        return std::equal(value.begin(), value.end(), arr->value.begin());
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Array::less(Object* other) {
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Array::greater(Object* other) {
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Array::lesseq(Object* other) {
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Array::greatereq(Object* other) {
        throw runtime_error("cannot compare " + this->getType() + " and " + other->getType());
    }
    bool Array::noteq(Object* other) {
        return !equal(other);
    }

    void Array::assign(Object* from) {
        Array* arr = convert(from);
        checkArray(type, arr->value);
        this->value = arr->value;
    }

    string Array::toString() {
        string r = "[";
        size_t s = value.size();
        size_t i = 0;
        for (Object* o : value) {
            r += o->toString() + (i != s-1 ? ", " : "");
            ++i;
        }
        r += "]";
        return r;
    }
    string Array::getValueString() {
        return toString();
    }

    string Array::getType() {return "Array<" + type + ">";}

    Object* Array::checkAll(string expected, Object* object) {
        Array* arr = convert(object);
        if (arr->value.size() == 0) return object;
        vector<Object*> objects;
        for (Object* o : arr->value) objects.emplace_back(Object::checkAll(expected, o));
        return new Array(objects);
        //return nullptr;
    }

    Object* Array::getDefault(string type) {
        if (type == "signed int") return new Array("Integer");
        else if (type == "double") return new Array("Double");
        else if (type == "string") return new Array("String");
        else if (type == "bool") return new Array("Boolean");
        
        throw runtime_error("this type of variable needs assignment after delaration");
    }

    Object* Array::check(Object &other) {
        if (other.getType().substr(0, 5) == "Array") return &other;
        throw runtime_error("variable types does not match");
    }

    Array* Array::convert(Object* obj) {
        Array* arr = nullptr;
        if (obj->getType().substr(0,5) != "Array") throw runtime_error("cannot convert from " + obj->getType() + " to array");
        try {
            arr = static_cast<Array*>(obj);
        } catch (exception e) {
            throw runtime_error("cannot convert from " + obj->getType() + " to array");
        }
        return arr;
    }

    Object* Array::assignIndex(size_t index, Object* array, Object* value) {
        Array* arr = convert(array);
        if (index < 0 || arr->value.size()-1 < index) throw runtime_error("index " + to_string(index) + " is outside of the array size");
        if (value->getType() != arr->type) throw runtime_error("cannot assign " + value->getType() + " to the array of " + arr->type);
        arr->value[index] = value;
        return array;
    }

    Object* Array::getIndex(size_t index, Object* array) {
        Array* arr = convert(array);
        if (index < 0 || arr->value.size()-1 < index) throw runtime_error("index " + to_string(index) + " is outside of the array size");
        return arr->value[index];
    }

    void Array::checkArray(string type, vector<Object*> arr) {
        for (Object* o : arr) {
            if (o->getType() != type) throw runtime_error("invalid type in array: " + o->getType());
        }
    }

    map<string, objectF> Array::getFunctions() {
        map<string, objectF> functions;
        
        functions["add"] = [](Object* t, vector<Object*> &arg) -> Object*{
            Array* arr = Array::convert(t);
            Array:checkArray(arr->type, arg);
            arr->value.insert(arr->value.end(), arg.begin(), arg.end());
            return nullptr;
        };

        functions["size"] = [](Object* t, vector<Object*> &arg) -> Object*{
            if (arg.size() != 0) throw runtime_error("'size' function doesn't take any arguments");
            return new Integer(convert(t)->value.size());
        };

        return functions;
    }

}