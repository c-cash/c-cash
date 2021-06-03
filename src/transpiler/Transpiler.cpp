#include "Transpiler.hpp"

#include <iostream>
#include <fstream>

namespace transpiler {

    using namespace std;

    map<string, string> specialCases {
        {"return", "return {0}"},
        {"write", "cout << {.<<.}"},
        {"read", "cin >> {.>>.}"},
        {"exit", "exit(1)"},

        {"IF", "if ({0}) {\n{...}}"},
        {"ELSE", "else {\n{...}}"},
        {"ELIF", "else if ({0}) {\n{...}}"}
    };

    map<string, string> connectorMap {
        {"{.<<.}", " << "},
        {"{.>>.}", " >> "},
        {"{.+.}", " + "},
        {"{...}", ""}
    };

    string varnamechars = "abcdefghijklmoprstuvwxyzABCDEFGHIJKLMNOPRSTUVWXYZ";
    string svarnamechars = "!@#$%^&*();',./][{}|";
    map<string, string> vardefmap;
    int clen=1;
    map<string, string> varmap;

    void Transpiler::transpile(string path, map<string, FunctionDefinition> &mFunctions) {
        ofstream fout;
        fout.open(path);

        fout << "// this code is generated from CCash\n// this code is auto generated, so it may be hard to read\n#include <iostream>\nusing namespace std;\n\n";

        for (pair<string, FunctionDefinition> func : mFunctions) {
            cout << "\u001b[32;1mTranspiling function \u001b[33m" << func.second.mName << "\u001b[0m\n";
            vardefmap.clear();
            cout << "\t\u001b[36mLooking for variables to discover type\u001b[0m\n";
            researchVariables(func.second.mStatements);
            string params;

            // function declaration
            int size = func.second.mParameters.size();
            cout << "\t\u001b[36mTranspiling function parameters\u001b[0m\n";
            for (int i=0; i<size; i++) {
                ParameterDefinition param = func.second.mParameters[i];
                vardefmap[param.mName] = param.mType.mName;
                params += param.mType.mName + " " + getNextName(param.mName) + (i == size-1 ? "" : ", ");
            }

            cout << "\t\u001b[36mDiscovering return type\u001b[0m\n";
            string returnType = discoverType(func.second);
            cout << "\t\u001b[36mReturn type found: \u001b[33m" << returnType << "\u001b[0m\n";
            fout << returnType << " " << func.second.mName << "(" << params << ") {\n";

            // statements
            cout << "\t\u001b[36mTranspiling statements\u001b[0m\n";
            int i=0;
            size = func.second.mStatements.size();
            for (Statement stmt : func.second.mStatements) {
                fout << transpileStatement(stmt) + getEnd(stmt);
                i++;
            }

            // close function
            fout << "}\n";
        }
        fout.close();
    }

    string Transpiler::transpileStatement(Statement &stmt) {
        // LITTERAL
        if (stmt.mKind == StatementKind::LITERAL) {
            return transpileLitteral(stmt);
        }
        // VARIABLE DECLARATION
        if (stmt.mKind == StatementKind::VARIABLE_DECLARATION) {
            return transpileVariableDeclaration(stmt);
        }
        // LOGIC CALL
        if (stmt.mKind == StatementKind::LOGIC_CALL) {
            return "(" + transpileOperator(stmt) + ")";
        }
        // OPERATOR CALL
        if (stmt.mKind == StatementKind::OPERATOR_CALL) {
            return transpileOperator(stmt);
        }
        // VARIABLE CALL
        if (stmt.mKind == StatementKind::VARIABLE_CALL) {
            return getNextName(stmt.mName);
        }
        // FUNCTION CALL
        if (stmt.mKind == StatementKind::FUNCTION_CALL) {
            return transpileFunctionCall(stmt);
        }
        return "";
    }

    string Transpiler::transpileLoop(Statement &stmt) {
        if (stmt.mStatements.size() < 1 || stmt.mStatements[0].mType.mName != "func_param" || stmt.mStatements[0].mStatements.size() < 1) throw runtime_error("cannot transpile loop statement");
        string result;
        if (stmt.mStatements[0].mStatements[0].mKind == StatementKind::LITERAL) { // loop with simple number
            string temp = getAutoName();
            result = "for (int " + temp + "; " + temp + "<" + stmt.mStatements[0].mStatements[0].mName + ";" + temp + "++) {\n";

        } else if (stmt.mStatements[0].mStatements[0].mKind == StatementKind::VARIABLE_CALL) {
            string temp = getAutoName();
            result = "for (int " + temp + "; " + temp + "<" + getNextName(stmt.mStatements[0].mStatements[0].mName) + ";" + temp + "++) {\n";
        } else if (stmt.mStatements[0].mStatements[0].mKind == StatementKind::LOGIC_CALL) { // expression inside
            result = "while (" + transpileStatement(stmt.mStatements[0].mStatements[0]) + ") {\n";
        } else if (stmt.mStatements[0].mStatements.size() == 3) { // normal for loop
            result = "for (" + transpileStatement(stmt.mStatements[0].mStatements[0]) + "; " +
            transpileStatement(stmt.mStatements[0].mStatements[1]) + "; " + transpileStatement(stmt.mStatements[0].mStatements[2]) + ") {\n";
        }

        // add body
        int i=0;
        for (Statement val : stmt.mStatements) {
            if (i++==0) continue;
            result += transpileStatement(val) + getEnd(val);
        }
        return result + "}";
    }


    string Transpiler::transpileVariableDeclaration(Statement &stmt) {
        if (stmt.mStatements.size() == 0) return stmt.mType.mName + " " + getNextName(stmt.mName); // declaration without value
        return stmt.mType.mName + " " + getNextName(stmt.mName) + " = " + transpileStatement(stmt.mStatements[0]) + "";
    }

    string Transpiler::transpileOperator(Statement &stmt) {
        vector<string> temp;
        for (Statement s : stmt.mStatements) {
            temp.emplace_back(transpileStatement(s));
        }
        string v = stmt.mName;
        if (stmt.mName == "and") v = " && ";
        else if (stmt.mName == "or") v = " || ";
        return Transpiler::connect<string>(temp,v);
    }

    string Transpiler::transpileLitteral(Statement &stmt) {
        if (stmt.mType.mName == "string") {
            unescape(stmt.mName);
            return "\"" + stmt.mName + "\"";
        } else if (stmt.mType.mName == "char") {
            unescape(stmt.mName);
            return "'" + stmt.mName + "'";
        } else {
            return stmt.mName;
        }
    }

    string Transpiler::transpileFunctionCall(Statement &stmt) {
        if (stmt.mName == "LOOP") { // this function is a loop :D
            return transpileLoop(stmt);
        } else if (specialCases.find(stmt.mName) != end(specialCases)) { // special case
            string special = specialCases[stmt.mName];
            int i=0;
            // normal values
            vector<string> args;
            for (Statement arg : stmt.mStatements) {
                string t = transpileStatement(arg);

                if (arg.mKind == StatementKind::VARIABLE_DECLARATION || arg.mKind == StatementKind::FUNCTION_CALL) {
                    t += getEnd(arg);
                }

                if (Transpiler::replace(special, "{" + to_string(i) + "}", t) == 0) {
                    args.emplace_back(t);
                }
                i++;
            }
            // connected values
            for (pair<string, string> c : connectorMap) {
                Transpiler::replace(special, c.first, Transpiler::connect<string>(args, c.second));
            }

            return special;
        } else { // normal case
            string result = stmt.mName + "(";
            int i=0;
            int size = stmt.mStatements.size();
            for (Statement arg : stmt.mStatements) {
                result += transpileStatement(arg) + (i == size-1 ? "" : ", ");
                i++;
            }
            return result + ")";
        }
    }

    string Transpiler::discoverType(FunctionDefinition &func) {
        if (func.mName == "main") return "int";
        Statement* firstReturn = findFirstReturn(func.mStatements);
        if (firstReturn == nullptr || firstReturn->mStatements.size() <= 0) return "void";
        Statement* firstLitteral = findFirstTyped(*firstReturn);
        if (firstLitteral == nullptr) return "void";
        else {return firstLitteral->mType.mName;};
    }

    void Transpiler::researchVariables(vector<Statement> &stmt) {
        for (Statement s : stmt) {
            if (s.mKind == StatementKind::VARIABLE_DECLARATION) {
                vardefmap[s.mName] = s.mType.mName;
            } else {
                researchVariables(s.mStatements);
            }
        }
    }

    Statement* Transpiler::findFirstReturn(vector<Statement> &stmt) {
        for (Statement &s : stmt) {
            if (s.mKind == StatementKind::FUNCTION_CALL && s.mName == "return") return &s;
            else if (s.mStatements.size() <= 0) continue;
            else {
                Statement* ret = findFirstReturn(s.mStatements);
                if (ret == nullptr) continue;
                else return ret;
            }
        }
        return nullptr;
    }

    Statement* Transpiler::findFirstTyped(Statement &stmt) {
        if (stmt.mStatements.size() == 0) return nullptr;
        for (Statement &s : stmt.mStatements) {
            if (s.mKind == StatementKind::LITERAL) return &s;
            else if (s.mKind == StatementKind::VARIABLE_CALL) {
                Statement* st = new Statement();
                st->mKind = StatementKind::VARIABLE_DECLARATION;
                st->mName = s.mName;
                st->mType = vardefmap[s.mName];
                return st;
            }
            else return findFirstTyped(s);
        }
        return nullptr;
    }

    int Transpiler::replace(string &s, string from, string to) {
        size_t position = s.find(from);
        int changed = 0;

        while(position != string::npos) {
            s.replace(position, from.size(), to);
            changed++;
            position = s.find(from, position + to.size());
        }
        return changed;
    }

    bool Transpiler::replaceOne(string &s, string from, string to) {
        size_t position = s.find(from);
        int changed = 0;

        if(position != string::npos) {
            s.replace(position, from.size(), to);
            changed++;
            position = s.find(from, position + to.size());
        }
        return changed;
    }

    template<typename T>
    string Transpiler::connect(vector<T> &v, string c) {
        string r;
        int size = v.size();
        for (int i=0; i<size; i++) {
            r += v[i] + (i == size-1 ? "" : c);
        }
        return r;
    }

    string Transpiler::getAutoName() {
        return getNextName(getNextRandom(svarnamechars));
    }

    string Transpiler::getNextName(string current) {
        return current;
        // this part is temporary disabled
        map<string, string>::iterator index = varmap.find(current);
        if (index == varmap.end()) {
           string newName = getNextRandom(varnamechars);
           varmap[current] = newName;
           return newName;
        } else {
            return index->second;
        }
    }

    string Transpiler::getNextRandom(string l) {
         // generate new name
            string newName;
            int value = clen;
            int size = l.size();
            while(value > 0) {
                unsigned int remainder = value % size;
                value /= size;
                newName += l[remainder];
            }
            ++clen;
            return newName;
    }
    
    string Transpiler::getEnd(Statement &stmt) {
        return (stmt.mName == "LOOP" || stmt.mName == "IF" || stmt.mName == "ELSE" || stmt.mName == "ELIF" ? "\n" : ";\n");
    }

    void Transpiler::unescape(string &s) {
        replace(s, "\n", "\\n");
        replace(s, "\t", "\\t");
    }
}
