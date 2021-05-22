#include "./Transpiler.hpp"

#include <iostream>
#include <fstream>

namespace transpiler {

    using namespace std;

    map<string, string> specialCases {
        {"return", "return {0}"},
        {"write", "cout << {.<<.}"}
    };

    map<string, string> connectorMap {
        {"{.<<.}", " << "},
        {"{.>>.}", " >> "},
        {"{.+.}", " + "}
    };

    void Transpiler::transpile(string path, map<string, FunctionDefinition> &mFunctions) {
        ofstream fout;
        fout.open(path);

        fout << "#include <iostream>\nusing namespace std;\n\n";

        for (pair<string, FunctionDefinition> func : mFunctions) {
            string returnType = discoverType(func.second);
            string params;
            // function declaration
            int size = func.second.mParameters.size();
            for (int i=0; i<size; i++) {
                ParameterDefinition param = func.second.mParameters[i];
                params += param.mType.mName + " " + param.mName + (i == size-1 ? "" : ", ");
            }
            fout << returnType << " " << func.second.mName << "(" << params << ") {\n";

            // statements
            for (Statement stmt : func.second.mStatements) {
                fout << transpileStatement(stmt);
            }

            // close function
            fout << "}";
        }
        fout.close();
    }

    string Transpiler::transpileStatement(Statement &stmt) {
        // LITTERAL
        if (stmt.mKind == StatementKind::LITERAL) {
            return transpileLitteral(stmt);
        }
        // FUNCTION CALL
        if (stmt.mKind == StatementKind::FUNCTION_CALL) {
            return transpileFunctionCall(stmt);
        }
        return "";
    }

    string Transpiler::transpileLitteral(Statement &stmt) {
        if (stmt.mType.mName == "string") {
            return "\"" + stmt.mName + "\"";
        } else if (stmt.mType.mName == "char") {
            return "'" + stmt.mName + "'";
        } else {
            return stmt.mName;
        }
    }

    string Transpiler::transpileFunctionCall(Statement &stmt) {
        if (specialCases.find(stmt.mName) != end(specialCases)) { // special case
            string special = specialCases[stmt.mName];
            int i=0;
            // normal values
            vector<string> args;
            for (Statement arg : stmt.mStatements) {
                string t = transpileStatement(arg);
                args.emplace_back(t);
                Transpiler::replace(special, "{" + to_string(i) + "}", t);
                i++;
            }
            // connected values
            for (pair<string, string> c : connectorMap) {
                Transpiler::replace(special, c.first, Transpiler::connect(args, c.second));
            }
            return special + ";\n";
        } else { // normal case
            string result = stmt.mName + "(";
            int i=0;
            int size = stmt.mStatements.size();
            for (Statement arg : stmt.mStatements) {
                result += transpileStatement(arg) + (i == size-1 ? "" : ", ");
                i++;
            }
            return result + ");\n";
        }
    }

    string Transpiler::discoverType(FunctionDefinition &func) {
        if (func.mName == "main") return "int";
        return "???";
    }

    void Transpiler::replace(string &s, string from, string to) {
        size_t position = s.find(from);

        while(position != string::npos) {
            s.replace(position, from.size(), to);
            position = s.find(from, position + to.size());
        }
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
    
}
