#include <iostream>

#include "../variables/Integer.hpp"
#include "../variables/Double.hpp"

#include "MathLibrary.hpp"
#include <cmath>

namespace library {

    void MathLibrary::linkGlobally(Scope &s) {
        // (F)round function
        s.functions["round"] = [](vector<Object*> args) -> Object* {
            if (args.size() != 1) throw runtime_error("round function takes exactly one argument");
            string t = args[0]->getType();
            if (t == "Integer" || t == "Double") return {new Integer(std::round(stod(args[0]->getValueString())))};
            throw runtime_error("You can round function only double or integer");
        };
        // (F)floor function
        s.functions["floor"] = [](vector<Object*> args) -> Object*{
            if (args.size() != 1) throw runtime_error("floor function takes exactly one argument");
            string t = args[0]->getType();
            if (t == "Integer" || t == "Double") return {new Integer(std::floor(stod(args[0]->getValueString())))};
            throw runtime_error("You can floor function only double or integer");
        };
        // (F)ceil function
        s.functions["ceil"] = [](vector<Object*> args) -> Object*{
            if (args.size() != 1) throw runtime_error("ceil function takes exactly one argument");
            string t = args[0]->getType();
            if (t == "Integer" || t == "Double") return {new Integer(std::ceil(stod(args[0]->getValueString())))};
            throw runtime_error("You can floor function only double or integer");
        };
        // (F)power function
        s.functions["pow"] = [](vector<Object*> args) -> Object*{
            if (args.size() != 2) throw runtime_error("pow function takes exactly two arguments");
            string t = args[0]->getType();
            string t2 = args[1]->getType();
            if ((t == "Integer" || t == "Double") && (t2 == "Integer" || t2 == "Double"))
                return {new Double(std::pow(stod(args[0]->getValueString()), stod(args[1]->getValueString())))};
            throw runtime_error("You can use power function only with doubles or integers");
        };
        // (F)sqrt function
        s.functions["sqrt"] = [](vector<Object*> args) -> Object*{
            if (args.size() != 1) throw runtime_error("sqrt function takes exactly one argument");
            string t = args[0]->getType();
            if (t == "Integer" || t == "Double") return {new Double(std::sqrt(stod(args[0]->getValueString())))};
            throw runtime_error("You can use sqrt function only with double or integer");
        };
        // (F)nthroot function
        s.functions["nthroot"] = [](vector<Object*> args) -> Object*{
            if (args.size() != 2) throw runtime_error("nthroot function takes exactly two arguments");
            string t = args[0]->getType();
            string t2 = args[1]->getType();
            if ((t == "Integer" || t == "Double") && (t2 == "Integer" || t2 == "Double"))
                return {new Double(std::pow(stod(args[0]->getValueString()), 1.0/stod(args[1]->getValueString())))};
            throw runtime_error("You can use nthroot function only with doubles or integers");
        };
        // (F)cos function
        s.functions["cos"] = [](vector<Object*> args) -> Object*{
            if (args.size() != 1) throw runtime_error("cos function takes exactly one argument");
            string t = args[0]->getType();
            if (t == "Integer" || t == "Double")
                return {new Double(std::cos(stod(args[0]->getValueString())))};
            throw runtime_error("You can use cos function only with doubles or integers");
        };
        // (F)sin function
        s.functions["sin"] = [](vector<Object*> args) -> Object*{
            if (args.size() != 1) throw runtime_error("sin function takes exactly one argument");
            string t = args[0]->getType();
            if (t == "Integer" || t == "Double")
                return {new Double(std::sin(stod(args[0]->getValueString())))};
            throw runtime_error("You can use sin function only with doubles or integers");
        };
        // (F)cos function
        s.functions["acos"] = [](vector<Object*> args) -> Object*{
            if (args.size() != 1) throw runtime_error("acos function takes exactly one argument");
            string t = args[0]->getType();
            if (t == "Integer" || t == "Double")
                return {new Double(std::acos(stod(args[0]->getValueString())))};
            throw runtime_error("You can use acos function only with doubles or integers");
        };
        // (F)sin function
        s.functions["asin"] = [](vector<Object*> args) -> Object*{
            if (args.size() != 1) throw runtime_error("asin function takes exactly one argument");
            string t = args[0]->getType();
            if (t == "Integer" || t == "Double")
                return {new Double(std::asin(stod(args[0]->getValueString())))};
            throw runtime_error("You can use asin function only with doubles or integers");
        };
        // (F)tan function
        s.functions["tan"] = [](vector<Object*> args) -> Object*{
            if (args.size() != 1) throw runtime_error("tan function takes exactly one argument");
            string t = args[0]->getType();
            if (t == "Integer" || t == "Double")
                return {new Double(std::tan(stod(args[0]->getValueString())))};
            throw runtime_error("You can use tan function only with doubles or integers");
        };
        //(F)atan function
        s.functions["atan"] = [](vector<Object*> args) -> Object*{
            if (args.size() != 1) throw runtime_error("atan function takes exactly one argument");
            string t = args[0]->getType();
            if (t == "Integer" || t == "Double")
                return {new Double(std::atan(stod(args[0]->getValueString())))};
            throw runtime_error("You can use atan function only with doubles or integers");
        };
        // (F)atant function
        s.functions["atant"] = [](vector<Object*> args) -> Object*{
            if (args.size() != 2) throw runtime_error("atant function takes exactly two arguments");
            string t = args[0]->getType();
            string t2 = args[1]->getType();
            if (t == "Integer" || t == "Double")
                return {new Double(std::atan2(stod(args[0]->getValueString()), stod(args[1]->getValueString())))};
            throw runtime_error("You can use atant function only with doubles or integers");
        };
        // (F)log function
        s.functions["log"] = [](vector<Object*> args) -> Object*{
            if (args.size() != 1) throw runtime_error("log function takes exactly one argument");
            string t = args[0]->getType();
            if (t == "Integer" || t == "Double")
                return {new Double(std::log(stod(args[0]->getValueString())))};
            throw runtime_error("You can use log function only with doubles or integers");
        };
        // (F)abs function
        s.functions["abs"] = [](vector<Object*> args) -> Object*{
            if (args.size() != 1) throw runtime_error("abs function takes exactly one argument");
            string t = args[0]->getType();
            if (t == "Integer" || t == "Double")
                return {new Double(std::abs(stod(args[0]->getValueString())))};
            throw runtime_error("You can use abs function only with doubles or integers");
        };
        // (F)deg function
        s.functions["deg"] = [](vector<Object*> args) -> Object*{
            if (args.size() != 1) throw runtime_error("deg function takes exactly one argument");
            string t = args[0]->getType();
            if (t == "Integer" || t == "Double")
                return {new Double(stod(args[0]->getValueString())*(180.0/3.14159265))};
            throw runtime_error("You can use deg function only with doubles or integers");
        };
        // (F)rad function
        s.functions["rad"] = [](vector<Object*> args) -> Object*{
            if (args.size() != 1) throw runtime_error("rad function takes exactly one argument");
            string t = args[0]->getType();
            if (t == "Integer" || t == "Double")
                return {new Double(stod(args[0]->getValueString())*(3.14159265/180.0))};
            throw runtime_error("You can use rad function only with doubles or integers");
        };
        // (F)max function
        s.functions["max"] = [](vector<Object*> args) -> Object*{
            vector<Object*> a = args;
            if (args.size() == 1 && args[0]->getType().substr(0, 5) == "Array") {a = Array::convert(args[0])->value;}
            if (a.size() < 1) throw runtime_error("max function takes exactly two arguments");
            double md = numeric_limits<double>::min();
            int mi = numeric_limits<int>::min();
            for (Object* e : a) {
                string t = e->getType();
                int iv = stoi(e->getValueString());
                double dv = stod(e->getValueString());
                if (t == "Integer") {if (iv > mi) mi = std::max(mi, iv);}
                else if (t == "Double") {if (dv > md) md = std::fmax(md, dv);}
                else {std::cout << t; throw runtime_error("You can use max function only with doubles or integers");}
            }
            if (md >= mi) return {new Double(md)};
            else return {new Integer(mi)};
        };
        // (F)min function
        s.functions["min"] = [](vector<Object*> args) -> Object*{
            vector<Object*> a = args;
            if (args.size() == 1 && args[0]->getType().substr(0, 5) == "Array") {a = Array::convert(args[0])->value;}
            if (a.size() < 1) throw runtime_error("min function takes exactly two arguments");
            double md = numeric_limits<double>::max();
            int mi = numeric_limits<int>::max();
            for (Object* e : a) {
                string t = e->getType();
                int iv = stoi(e->getValueString());
                double dv = stod(e->getValueString());
                if (t == "Integer") {if (iv < mi) mi = std::max(mi, iv);}
                else if (t == "Double") {if (dv < md) md = std::fmax(md, dv);}
                else throw runtime_error("You can use min function only with doubles or integers");
            }
            if (md <= mi) return {new Double(md)};
            else return {new Integer(mi)};
        };
    }

    void MathLibrary::linkNamespaced(string name, Namespace &ns) {
        // TODO: use linkglobally to get all functions and link them to the namespace
    }

    Library* MathLibrary::getInstance() {
        return new MathLibrary();
    }
}